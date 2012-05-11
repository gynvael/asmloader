; Use nasm (netwide assembler) to compile this.
[bits 32]

  call print_hello
  db "Hello World!", 0xa, 0

print_hello:
  call [ebx+3*4]
  add esp, 4

  push 0
  call [ebx]

