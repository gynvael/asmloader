**Usage**:
`asmloader <file.bin>`

What does this do (in short):
1. It allocates read/write/execute memory.
2. Loads the specified file into it.
3. And redirects the CPU (EIP/RIP) to it (i.e. executes the binary code that  was in the file).
+  Plus it provides a very minor API set.

The `file.bin` should be pure machine code without any headers. The code will be loaded to a random page address + 0x200. In EBX/RBX register there will be a pointer to an array of functions:

  0. `exit`
  1. `putchar`
  2. `getchar`
  3. `printf`
  4. `scanf`

For this set of APIs please use a standard cdecl calling convention regardless of the platform (i.e. arguments to the stack, caller cleans the stack afterwards). Note that this includes the 64-bit platforms which usually make use of the fastcall convention (but asmloader has a set of wrappers to make the API consistent).

If you would like to use the platform's default calling convention on 64-bits, note that the RCX register contains the same API table minus the converting wrappers, so just do:

  `MOV RBX, RCX`

And you're set to go. Note that on 32-bit x86 cdecl is used all around anyway, so this is not required (and ECX contains the same value as EBX).

For the record, platform-specific calling conventions:
  * Windows/Linux 32-bit: cdecl
  * Windows       64-bit: fastcall (Windows x64)
  * Linux         64-bit: fastcall (System V AMD64 ABI)
 
See http://en.wikipedia.org/wiki/X86_calling_conventions for details.

In the current version it's platform specific which registers are volatile (i.e. scratch registers) and which are non-volatile (i.e. callee-preserved registers) when using the built in API (though this may change in future versions).

For exiting please use the exit function. Returning (via RET instruction) from the shellcode yields undefined behavior, i.e. it might work on some platforms/version of asmloader, and might not work on others.

If you don't call exit, the program will crash.

Have fun :)


Changelog:

* 0.0.2 -> 0.1.0:
  Added 64-bit support (Windows and Linux). Refactoring all around.

* 0.0.1 -> 0.0.2:
  Added a warning in case source code file is detected as first argument.

