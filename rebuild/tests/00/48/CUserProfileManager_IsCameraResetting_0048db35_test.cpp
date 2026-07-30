#include <cstdio>
#include <cstring>

struct CUserProfileManager {
    char pad0[0xc];
    char m_flag;
    char m_reset;
    char pad1[0xcd - 0x0e];
    char m_resetAlt;
};

char __fastcall IsCameraResetting(CUserProfileManager *self)
{
    if (self->m_flag)
        return self->m_resetAlt;
    return self->m_reset;
}

static char call_it(CUserProfileManager *obj)
{
    char out;
    __asm {
        mov ecx, obj
        call IsCameraResetting
        mov out, al
    }
    return out;
}

int main()
{
    CUserProfileManager o;
    memset(&o, 0, sizeof(o));

    o.m_flag = 1; o.m_reset = 0x11; o.m_resetAlt = 0x42;
    char r1 = call_it(&o);
    if (r1 != 0x42) { printf("FAIL branch1 got %d\n", (int)r1); return 1; }

    o.m_flag = 0; o.m_reset = 0x37; o.m_resetAlt = 0x7e;
    char r2 = call_it(&o);
    if (r2 != 0x37) { printf("FAIL branch2 got %d\n", (int)r2); return 1; }

    o.m_flag = (char)0x80; o.m_reset = 0x01; o.m_resetAlt = 0x5a;
    char r3 = call_it(&o);
    if (r3 != 0x5a) { printf("FAIL branch3 got %d\n", (int)r3); return 1; }

    printf("OK_0x0048db35\n");
    return 0;
}