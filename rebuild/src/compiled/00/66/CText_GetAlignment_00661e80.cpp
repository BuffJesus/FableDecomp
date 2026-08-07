#include "rebuild_abi.h"
// CText::GetAlignment @ 0x00661e80
struct FableAlign_00661e80
{
    char pad[0x15c];
    unsigned char flags;   // +0x15c
    char FABLE_FASTCALL get();
};
char FABLE_FASTCALL FableAlign_00661e80::get()
{
    unsigned char v = this->flags;
    if (v & 4)
        return 1;
    v = (unsigned char)(v >> 3);
    v = (unsigned char)(v & 1);
    return (char)v;
}