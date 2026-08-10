#include <cstdio>

struct CCameraSub;

static int g_reset_called = 0;
static void* g_reset_this = 0;

void __fastcall CCameraSub_Reset(CCameraSub* self);

struct CEngine {
    char pad0[0x1f8];
    unsigned char flag_1f8;
    char pad1[0x280 - 0x1f8 - 1];
    CCameraSub* sub_280;
};

void __fastcall CCameraSub_Reset(CCameraSub* self) {
    g_reset_called++;
    g_reset_this = self;
}

void __fastcall ChangeCameraAngle(CEngine* self)
{
    CCameraSub_Reset(self->sub_280);
    self->flag_1f8 = 0;
}

int main() {
    CEngine e;
    e.flag_1f8 = 0x7f;
    e.sub_280 = (CCameraSub*)(void*)0x12345678;
    ChangeCameraAngle(&e);
    if (g_reset_called == 1 &&
        g_reset_this == (void*)0x12345678 &&
        e.flag_1f8 == 0) {
        printf("009fd640_TEST PASS\n");
    } else {
        printf("FAIL called=%d this=%p flag=%d\n", g_reset_called, g_reset_this, (int)e.flag_1f8);
    }
    return 0;
}