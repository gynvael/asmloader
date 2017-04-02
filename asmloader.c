/* 
 * Simplified Assembly Loader v.0.0.3
 * by gynvael.coldwind//vx
 * http://gynvael.coldwind.pl/
 * 
 * -- License Stuff
 *
 * Copyright (C) 2017 by Gynvael Coldwind
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * -- Usage
 *
 * Usage:
 *   asmloader <file.bin>
 *
 * What does this do (in short):
 * 1. It allocates read/write/execute memory.
 * 2. Loads the specified file into it.
 * 3. And redirects the CPU (EIP/RIP) to it (i.e. executes the binary code that
 *    was in the file).
 * +  It provides a very minor API set.
 *
 * File.bin should be pure machine code without any headers.
 * Your code will be loaded to a random page address + 0x200.
 * In EBX/RBX register there will be a pointer to an array of functions:
 *   0 exit
 *   1 putchar
 *   2 getchar
 *   3 printf
 *   4 scanf
 * For this set of APIs please use a standard cdecl calling convention
 * regardless of the platform (i.e. arguments to the stack, caller cleans the
 * stack afterwards). Note that this includes the 64-bit platforms which
 * usually make use of the fastcall convention (but asmloader has a set of
 * wrappers to make the API consistent).
 *
 * If you would like to use the platform's default calling convention on
 * 64-bits, note that the RCX register contains the same API table minus the
 * converting wrappers, so just do:
 *   MOV RBX, RCX
 * And you're set to go. Note that on 32-bit x86 cdecl is used all around
 * anyway, so this is not required (and ECX contains the same value as EBX).
 *
 * For the record, platform-specific calling conventions:
 * Windows/Linux 32-bit: cdecl
 * Windows       64-bit: fastcall (Windows x64)
 * Linux         64-bit: fastcall (System V AMD64 ABI)
 * See http://en.wikipedia.org/wiki/X86_calling_conventions for details.
 *
 * In the current version it's platform specific which registers are volatile
 * (i.e. scratch registers) and which are non-volatile (i.e. callee-preserved
 * registers) when using the built in API (though this may change in future
 * versions).
 *
 * For exiting please use the exit function. Returning (via RET instruction)
 * from the shellcode yields undefined behavior, i.e. it might work on some
 * platforms/version of asmloader, and might not work on others.
 *
 * If you don't call exit, the program will crash.
 *
 * Have fun :)
 *
 * 
 * Changelog:
 *
 * * 0.0.2 -> 0.1.0:
 *   Added 64-bit support (Windows and Linux). Refactoring all around.
 *
 * * 0.0.1 -> 0.0.2:
 *   Added a warning in case source code file is detected as first argument.
 *
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef _WIN32
#  include <windows.h>
#elif defined(__unix__) || __APPLE__
#  include <sys/mman.h>
#endif

#define ARCH_UNKNOWN 0
#define ARCH_X86_32  1
#define ARCH_X86_64_MSWIN 2
#define ARCH_X86_64_LINUX 3

// To avoid dealing with multiple different linkers the stubs are directly
// included.
#include "stubs/x86_64_linux_stub.c"
#include "stubs/x86_64_mswin_stub.c"
#include "stubs/x86_32_stub.c"

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || \
    defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#  if defined(_WIN32) || defined(__WIN32)
#    define ARCH ARCH_X86_64_MSWIN
#  elif defined(__unix__) || defined(__APPLE__)
#    define ARCH ARCH_X86_64_LINUX     
#  else
#    error Unsupported or unrecognized architecture.
#  endif
#endif

#if defined(i386) || defined(__i386) || defined(__i386__) || \
    defined(__IA32__) || defined(_M_IX86) || defined(_X86_) || defined(__X86__)
#  define ARCH ARCH_X86_32
#endif

#ifndef ARCH
#  error Unsupported or unrecognized architecture.
#endif

#define STUB_SIZE 0x200
#define STUB_PAD  1

typedef int (*func_t)();

size_t file_size(FILE *f) {
  size_t pos, len;
  pos = ftell(f);
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  fseek(f, pos, SEEK_SET);
  return len;
}

size_t find_magic_offset(const void *data, size_t data_size,
                         const void *magic, size_t magic_size) {
  assert(magic_size <= data_size);

  const uint8_t *byte_data = (const uint8_t*)data;
  for (size_t i = 0; i <= data_size - magic_size; i++) {
    if (memcmp(byte_data + i, magic, magic_size) != 0) {
      continue;
    }

    return i;
  }

  return -1;
}

void replace_magic(void *data, size_t data_size, 
                   const void *magic, size_t magic_size,
                   const void *replacement) {
  assert(magic_size <= data_size);
  size_t offset = find_magic_offset(data, data_size, magic, magic_size);

  // In general, this function must never fail.
  if (offset == (size_t)-1) {
    fprintf(stderr, "fatal: magic replacement failed; file a bug\n");
    abort();
  }

  uint8_t *byte_data = (uint8_t*)data;
  memcpy(byte_data + offset, replacement, magic_size);
}

void apply_relocs(void *data, size_t data_size, size_t data_addr,
                  size_t reloc_magic, size_t reloc_mask) {
  assert(sizeof(size_t) <= data_size);
  assert((reloc_magic & ~reloc_mask) == 0);

  uint8_t *byte_data = (uint8_t*)data;
  for (size_t i = 0; i <= data_size - sizeof(size_t); i++) {
    size_t value;
    memcpy(&value, byte_data + i, sizeof(size_t));
    if ((value & reloc_mask) != reloc_magic) {
      continue;
    }

    // Apply relocation.
    value = value - reloc_magic + data_addr;
    memcpy(byte_data + i, &value, sizeof(size_t));
  }
}

void prepare_stub(void *dst) {
#if ARCH == ARCH_X86_64_LINUX
  memcpy(dst, STUB_x86_64_linux_stub, sizeof(STUB_x86_64_linux_stub));
#elif ARCH == ARCH_X86_64_MSWIN
  memcpy(dst, STUB_x86_64_mswin_stub, sizeof(STUB_x86_64_mswin_stub));
#endif

#if ARCH == ARCH_X86_64_LINUX || ARCH == ARCH_X86_64_MSWIN
  static size_t func_arr[] = {
    (size_t)exit, (size_t)putchar, (size_t)getchar,
    (size_t)printf, (size_t)scanf
  };

  for (size_t i = 0; i < sizeof(func_arr) / sizeof(*func_arr); i++) {
    size_t magic = 0xc0dec0dec0de0000ULL + i;
    replace_magic(dst, STUB_SIZE, &magic, sizeof(magic), &func_arr[i]);
  }

  apply_relocs(dst, STUB_SIZE, (size_t)dst,
               0xcdcdcdcdcdcdc000ULL, 0xfffffffffffff000ULL);
#elif ARCH == ARCH_X86_32
  static void *func_arr[] = { exit, putchar, getchar, printf, scanf };
  uint32_t func_arr_ptr = (size_t)func_arr;
  memcpy(dst, STUB_x86_32_stub, sizeof(STUB_x86_32_stub));
  replace_magic(dst, STUB_SIZE, "\xCC\xCC\xCC\xCC", 4, &func_arr_ptr);
#endif
}

const char *load_binary(void **dst_data, size_t *dst_size, const char *fname) {
  const char *ret = NULL;
  FILE *f = NULL;
  void *data = NULL;
  size_t size = 0;
  size_t alloc_size;
 
  f = fopen(fname, "rb");
  if(!f) {
    ret = "could not open input file";
    goto err;
  }

  size = file_size(f);
  if(size == 0) {
    ret = "file of 0 size";
    goto err;
  }

  alloc_size = STUB_SIZE + size + STUB_PAD;

#ifdef _WIN32
  data = VirtualAlloc(NULL, alloc_size, MEM_COMMIT | MEM_RESERVE,
                      PAGE_EXECUTE_READWRITE);
#else
  data = mmap(NULL, alloc_size, PROT_EXEC | PROT_READ | PROT_WRITE,
              MAP_ANON | MAP_PRIVATE, -1, 0);
#endif

#ifdef _WIN32
  if(!data) {
#else
  if(data == (void*)-1) {
    data = NULL;
#endif
    ret = "could not allocate memory";
    goto err;
  }

  if(fread((uint8_t*)data + STUB_SIZE, 1, size, f) != size) {
    ret = "read error";
    goto err;
  }

  fclose(f); f = NULL;

  prepare_stub(data);

  // Set last byte to int3.
  ((uint8_t*)data)[size + STUB_SIZE] = 0xCC;

  *dst_size = size;
  *dst_data = data;

  return ret;

err:
  if(f) fclose(f);
#ifdef _WIN32
  if(data) VirtualFree(data, 0, MEM_RELEASE);
#else
  if(data) munmap(data, alloc_size);
#endif
  return ret;
}

bool is_file_ext(const char *filename, const char *ext) {
  const char *fileext;

  // Get the extension.
  fileext = strrchr(filename, '.');
  if(!fileext) {

    // There is no extension. Were we looking for one?
    if(!ext || ext[0] == '\0')
      return true;

    // Yes, and there is none.
    return false;
  }

  fileext += 1; // Skip the dot.

  // Compare.
#ifdef _WIN32
  return (bool)(stricmp(fileext, ext) == 0);  
#elif defined(__unix__) || __APPLE__
  return (bool)(strcasecmp(fileext, ext) == 0);
#endif
}

// It's easier to set a breakpoint if this is a separate function.
int run(func_t func) {
  return func();
}

int main(int argc, char **argv) {
  const char *ret;
  void *data;
  size_t size;
  func_t func;
  int fret;

  // Needs to be synchronized with ARCH_* constants.
  const char *architectures[] = {
    "???",
    "x86-32",
    "x86-64-mswin",
    "x86-64-linux",
  };

  printf("Simplified Assembly Loader v.0.1.0 by gynvael.coldwind//vx\n"
         "Architecture: %s\n", architectures[ARCH]);

  if(argc != 2) {
    puts("usage: asmloader <file.bin>\n"
         "note : use nasm to compile .asm to .bin\n"
         "     : e.g. nasm file.asm -o file.bin\n"
         "     :      asmloader file.bin\n"
         "note : double check the bitness/architecture");
    return 1;
  }

  // A common mistake is to enter the source .asm file as an argument
  // instead of the compiled file. Display a warning in such case.
  if(is_file_ext(argv[1], "asm") ||
     is_file_ext(argv[1], "nasm")) {
    printf("warning: are you sure \"%s\" is the file you want to execute?\n"
           "       : it seems to be a source assembly file, while asmloader\n"
           "       : needs the compiled version (with proper machine code)\n"
           "       : (if your app will crash this is most likely the cause)\n",
           argv[1]);

    // Continue execution.
  }
  
  ret = load_binary(&data, &size, argv[1]);
  if(ret != NULL) {
    printf("error: %s\n", ret);
    return 2;
  }

#if ARCH == ARCH_X86_64_LINUX
  printf("Code loaded at 0x%.16lx (%u bytes)\n",
      (uint64_t)data + STUB_SIZE, (uint32_t)size);
#elif ARCH == ARCH_X86_64_MSWIN
  printf("Code loaded at 0x%.16I64x (%u bytes)\n",
      (uint64_t)data + STUB_SIZE, (uint32_t)size);
#elif ARCH == ARCH_X86_32
  printf("Code loaded at 0x%.8x (%u bytes)\n",
      (uint32_t)data + STUB_SIZE, (uint32_t)size);
#endif
  fflush(stdout);

  func = (func_t)data;
  fret = run(func);

#ifdef _WIN32
  VirtualFree(data, 0, MEM_RELEASE);
#else
  munmap(data, STUB_SIZE + size + STUB_PAD);
#endif

  return fret;
}

