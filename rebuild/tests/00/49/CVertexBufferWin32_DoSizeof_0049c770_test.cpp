#include <stdio.h>

struct CVertexBufferWin32 {
    int m_field0;
    int m_field4;
    int m_dataSize;
};

int __fastcall DoSizeof(CVertexBufferWin32 *this_)
{
    return this_->m_dataSize + 0x18;
}

int main()
{
    CVertexBufferWin32 vb;
    vb.m_field0 = 0x11111111;
    vb.m_field4 = 0x22222222;
    vb.m_dataSize = 100;

    CVertexBufferWin32 *p = &vb;
    int r;
    // this=ecx thiscall/fastcall, no stack args, no ret N
    __asm {
        mov ecx, p
        call DoSizeof
        mov  r, eax
    }
    if (r != 100 + 0x18) { printf("FAIL r=%d\n", r); return 1; }

    vb.m_dataSize = 0;
    __asm {
        mov ecx, p
        call DoSizeof
        mov  r, eax
    }
    if (r != 0x18) { printf("FAIL2 r=%d\n", r); return 1; }

    printf("OK_0x0049c770 r=%d\n", 100 + 0x18);
    return 0;
}