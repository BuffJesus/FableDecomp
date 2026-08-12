#include <cstdio>

struct CTCStealth {
    char pad_d8[0xd8];
    unsigned int f_d8;
    char pad2[0x1e8 - 0xd8 - 4];
    unsigned int f_1e8;
    unsigned int f_1ec;
};

static int g_base_called = 0;
void __fastcall CTCStealth_base_OnCreate(void* self) { g_base_called = 1; }

void __fastcall CTCStealth_OnCreate(CTCStealth* self);

int main()
{
    CTCStealth obj;
    obj.f_d8 = 0xDEAD;
    obj.f_1e8 = 0xBEEF;
    obj.f_1ec = 0xCAFE;
    CTCStealth_OnCreate(&obj);
    if (obj.f_1e8 == 0x1000 && obj.f_1ec == 0 && obj.f_d8 == 0 && g_base_called == 1)
        printf("STEALTH_ONCREATE_OK\n");
    else
        printf("FAIL %x %x %x %d\n", obj.f_1e8, obj.f_1ec, obj.f_d8, g_base_called);
    return 0;
}