[bits 64]
[org 0xcdcdcdcdcdcdc000]  ; Used for pseudo-relocations.

start:
  lea rbx, [rel jump_table]    ; Wrapped API.
  lea rcx, [rel import_table]  ; Platform-specific API.
  jmp real_start

; Note that Windows uses the funny calling convention that requires the caller
; to allocate space on the stack for the 4 arguments even if registers are used
; to actually pass them. This space is sometimes used by the callee functions to
; store the received arguments for various reasons, so it's important to
; allocate it.
; https://msdn.microsoft.com/en-us/library/ms235286.aspx

stub_exit:
  mov rcx, [rsp+8]
  sub rsp, 0x20  ; It's exit, so technically this shouldn't matter. But in the
                 ; rare case of somebody using local variables with an atexit-
                 ; -registered function we'll include it here as well.
                 ; Probably it will never happen though.
  jmp [rel import_exit]
  ; Should not return.

stub_putchar:
  push rcx
  mov rcx, [rsp+8]
  sub rsp, 0x20
  call [rel import_putchar]
  add rsp, 0x20
  pop rcx
  ret

stub_getchar:
  sub rsp, 0x20
  call [rel import_getchar]
  add rsp, 0x20
  ret

stub_printf:
  push qword [rel import_printf]
  jmp vaarg_converter

stub_scanf:
  push qword [rel import_scanf]
  jmp vaarg_converter

; Expects function-to-be-called's address on the stack.
vaarg_converter:
  ; Order of arguments: rcx, rdx, r8, r9, <stack>
  mov [rel saved_rcx], rcx
  mov [rel saved_rdx], rdx
  mov [rel saved_r8], r8
  mov [rel saved_r9], r9

  pop rdx  ; Function to be called.
  pop rax  ; Return address.
  mov [rel saved_ret], rax
  mov rax, rdx

  ; Since printf is a vararg, we don't really know how many arguments are there.
  ; However, the calling convention requires that there is space for exactly
  ; four arguments on the stack. If this condition is not met, the callee might
  ; overwrite some variables of the calling function. Since there is no (nice)
  ; way to prevent this, a way around it is to save the 3 (fmt parameter is
  ; required) values somewhere, and restore them after function returns.
  mov rcx, [rsp+8*0]
  mov rdx, [rsp+8*1]
  mov r8, [rsp+8*2]
  mov r9, [rsp+8*3]
  mov [rel saved_stack_slot_1], rdx
  mov [rel saved_stack_slot_2], r8
  mov [rel saved_stack_slot_3], r9

  call rax

  mov rdx, [rel saved_stack_slot_1]
  mov r8, [rel saved_stack_slot_2]
  mov r9, [rel saved_stack_slot_3]
  mov [rsp+8*1], rdx
  mov [rsp+8*2], r8
  mov [rsp+8*3], r9

  mov rcx, [rel saved_rcx]
  mov rdx, [rel saved_rdx]
  mov r8, [rel saved_r8]
  mov r9, [rel saved_r9]
  jmp [rel saved_ret]

; Attempt to move out the data part to another cache-line.
times (0x100 - ($ - start)) db 0x00

jump_table:
  dq stub_exit
  dq stub_putchar
  dq stub_getchar
  dq stub_printf
  dq stub_scanf

import_table:
  import_exit:     dq 0xc0dec0dec0de0000
  import_putchar:  dq 0xc0dec0dec0de0001
  import_getchar:  dq 0xc0dec0dec0de0002
  import_printf:   dq 0xc0dec0dec0de0003
  import_scanf:    dq 0xc0dec0dec0de0004

storage_area:
  saved_rcx: dq 0
  saved_rdx: dq 0
  saved_r8: dq 0
  saved_r9: dq 0
  saved_ret: dq 0
  saved_stack_slot_1: dq 0
  saved_stack_slot_2: dq 0
  saved_stack_slot_3: dq 0
  
; Align to STUB_SIZE.
times (0x200 - ($ - start)) db 0x0
real_start:


