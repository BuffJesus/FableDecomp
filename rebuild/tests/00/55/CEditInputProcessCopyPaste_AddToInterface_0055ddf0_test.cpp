#include <stdio.h>

struct CEditInputProcessCopyPaste {
    char pad[0x10];
    unsigned char m_flag; /* +0x10 */
};

static int g_base_called = 0;
static CEditInputProcessCopyPaste* g_base_self = 0;

void __fastcall AddToInterfaceBase(CEditInputProcessCopyPaste* self)
{
    g_base_called = 1;
    g_base_self = self;
}

void __fastcall AddToInterface(CEditInputProcessCopyPaste* self)
{
    self->m_flag = 0;
    AddToInterfaceBase(self);
}

int main()
{
    CEditInputProcessCopyPaste obj;
    obj.m_flag = 0xAB;
    CEditInputProcessCopyPaste* p = &obj;
    /* __fastcall shim: this in ecx */
    __asm {
        mov ecx, p
        call AddToInterface
    }
    if (obj.m_flag == 0 && g_base_called == 1 && g_base_self == &obj) {
        printf("OK_0x0055ddf0\n");
        return 0;
    }
    printf("BAD flag=%d called=%d\n", obj.m_flag, g_base_called);
    return 1;
}