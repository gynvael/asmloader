#pragma once

unsigned char STUB_x86_64_mswin_stub[] = {
    0x48, 0x8d, 0x1d, 0xf9, 0x00, 0x00, 0x00, 0x48, 
    0x8d, 0x0d, 0x1a, 0x01, 0x00, 0x00, 0xe9, 0xed, 
    0x01, 0x00, 0x00, 0x48, 0x8b, 0x4c, 0x24, 0x08, 
    0x48, 0x83, 0xec, 0x20, 0xff, 0x25, 0x06, 0x01, 
    0x00, 0x00, 0x51, 0x48, 0x8b, 0x4c, 0x24, 0x08, 
    0x48, 0x83, 0xec, 0x20, 0xff, 0x15, 0xfe, 0x00, 
    0x00, 0x00, 0x48, 0x83, 0xc4, 0x20, 0x59, 0xc3, 
    0x48, 0x83, 0xec, 0x20, 0xff, 0x15, 0xf6, 0x00, 
    0x00, 0x00, 0x48, 0x83, 0xc4, 0x20, 0xc3, 0xff, 
    0x35, 0xf3, 0x00, 0x00, 0x00, 0xeb, 0x08, 0xff, 
    0x35, 0xf3, 0x00, 0x00, 0x00, 0xeb, 0x00, 0x48, 
    0x89, 0x0d, 0xf2, 0x00, 0x00, 0x00, 0x48, 0x89, 
    0x15, 0xf3, 0x00, 0x00, 0x00, 0x4c, 0x89, 0x05, 
    0xf4, 0x00, 0x00, 0x00, 0x4c, 0x89, 0x0d, 0xf5, 
    0x00, 0x00, 0x00, 0x5a, 0x58, 0x48, 0x89, 0x05, 
    0xf4, 0x00, 0x00, 0x00, 0x48, 0x89, 0xd0, 0x48, 
    0x8b, 0x0c, 0x24, 0x48, 0x8b, 0x54, 0x24, 0x08, 
    0x4c, 0x8b, 0x44, 0x24, 0x10, 0x4c, 0x8b, 0x4c, 
    0x24, 0x18, 0x48, 0x89, 0x15, 0xdf, 0x00, 0x00, 
    0x00, 0x4c, 0x89, 0x05, 0xe0, 0x00, 0x00, 0x00, 
    0x4c, 0x89, 0x0d, 0xe1, 0x00, 0x00, 0x00, 0xff, 
    0xd0, 0x48, 0x8b, 0x15, 0xc8, 0x00, 0x00, 0x00, 
    0x4c, 0x8b, 0x05, 0xc9, 0x00, 0x00, 0x00, 0x4c, 
    0x8b, 0x0d, 0xca, 0x00, 0x00, 0x00, 0x48, 0x89, 
    0x54, 0x24, 0x08, 0x4c, 0x89, 0x44, 0x24, 0x10, 
    0x4c, 0x89, 0x4c, 0x24, 0x18, 0x48, 0x8b, 0x0d, 
    0x7c, 0x00, 0x00, 0x00, 0x48, 0x8b, 0x15, 0x7d, 
    0x00, 0x00, 0x00, 0x4c, 0x8b, 0x05, 0x7e, 0x00, 
    0x00, 0x00, 0x4c, 0x8b, 0x0d, 0x7f, 0x00, 0x00, 
    0x00, 0xff, 0x25, 0x81, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x13, 0xc0, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 
    0x22, 0xc0, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 
    0x38, 0xc0, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 
    0x47, 0xc0, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 
    0x4f, 0xc0, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 
    0x00, 0x00, 0xde, 0xc0, 0xde, 0xc0, 0xde, 0xc0, 
    0x01, 0x00, 0xde, 0xc0, 0xde, 0xc0, 0xde, 0xc0, 
    0x02, 0x00, 0xde, 0xc0, 0xde, 0xc0, 0xde, 0xc0, 
    0x03, 0x00, 0xde, 0xc0, 0xde, 0xc0, 0xde, 0xc0, 
    0x04, 0x00, 0xde, 0xc0, 0xde, 0xc0, 0xde, 0xc0, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
