#include <stddef.h>

// Retail 0x00c19f90 Getter_Field84 (7 bytes): 8b 81 84 00 00 00 c3
//   mov eax, DWORD PTR [ecx+0x84]
//   ret
// __fastcall member: 'this' arrives in ECX, callee does 'ret' (no stack cleanup
// of args -> the only arg is the implicit this in ecx).

struct CObj
{
    unsigned char pad00[0x84];
    unsigned int  field84;   // +0x84
};

typedef char assert_field84_off[(offsetof(struct CObj, field84) == 0x84) ? 1 : -1];

unsigned int __fastcall Getter_Field84(struct CObj* self)
{
    return self->field84;
}