#include <stdio.h>
#include <string.h>

// Definition of function-under-test (mirrors source.cpp)
struct CPlayerGui {
    char pad_c8[0xc8];
    unsigned char b_c8;   // +0xc8
    unsigned char b_c9;   // +0xc9
    char pad_ca[0xd8-0xc9-1];
    unsigned char b_d8;   // +0xd8
    char pad_d9[0xf0-0xd8-1];
    unsigned int  d_f0;   // +0xf0
    unsigned int  d_f4;   // +0xf4
};

void __fastcall ForceOpen(CPlayerGui *this_)
{
    this_->b_c9 = 0;
    this_->b_c8 = 1;
    this_->b_d8 = 0;
    this_->d_f0 = 0;
    this_->d_f4 = 0;
}

int main()
{
    // Pre-fill with 0xAA so we can see exactly which bytes get written.
    unsigned char buf[0x100];
    memset(buf, 0xAA, sizeof(buf));
    CPlayerGui *p = (CPlayerGui*)buf;

    // Invoke via __fastcall shim (this=ecx).
    void *fn = (void*)&ForceOpen;
    __asm {
        mov ecx, p
        mov eax, fn
        call eax
    }

    int ok = 1;
    if (buf[0xc8] != 1)    ok = 0;
    if (buf[0xc9] != 0)    ok = 0;
    if (buf[0xd8] != 0)    ok = 0;
    if (*(unsigned int*)(buf+0xf0) != 0) ok = 0;
    if (*(unsigned int*)(buf+0xf4) != 0) ok = 0;
    // Untouched neighbour bytes must remain 0xAA.
    if (buf[0xc7] != 0xAA) ok = 0;
    if (buf[0xca] != 0xAA) ok = 0;

    if (ok) {
        printf("OK_0x00434ad0\n");
        return 0;
    }
    printf("FAIL c8=%02x c9=%02x d8=%02x f0=%08x f4=%08x\n",
        buf[0xc8], buf[0xc9], buf[0xd8],
        *(unsigned int*)(buf+0xf0), *(unsigned int*)(buf+0xf4));
    return 1;
}