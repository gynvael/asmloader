; Use nasm (netwide assembler) to compile this.
[bits 64]

  call print_hello
  db "Hello World!", 0xa, 0

print_hello:
  call [rbx+3*8]
  add rsp, 8

  push 0
  call [rbx]

