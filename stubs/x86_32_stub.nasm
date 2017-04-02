[bits 32]
start:
  mov ebx, 0xcccccccc
  mov ecx, ebx
  jmp real_start

; Align to STUB_SIZE.
times (0x200 - ($ - start)) db 0x0
real_start:

