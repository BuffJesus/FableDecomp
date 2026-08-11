#include <stdio.h>

struct CTCElectrocutedEffect {
    char pad[0x30];
    int f30;
};

static int g_helper_called = 0;
static int g_helper_arg = -999;
static CTCElectrocutedEffect* g_base_self = 0;

void __stdcall helper_6d7f20(int a) { g_helper_called++; g_helper_arg = a; }
void __fastcall base_6d7e70(CTCElectrocutedEffect* self) { g_base_self = self; }

void __fastcall CTCElectrocutedEffect_OnInitialActivate(CTCElectrocutedEffect* self);

int main() {
    CTCElectrocutedEffect obj;
    obj.f30 = 0x12345;
    CTCElectrocutedEffect_OnInitialActivate(&obj);
    if (obj.f30 == 0 && g_helper_called == 1 && g_helper_arg == 0 && g_base_self == &obj) {
        printf("006d8770_TEST PASS\n");
    } else {
        printf("FAIL f30=%d hc=%d ha=%d bs=%p self=%p\n", obj.f30, g_helper_called, g_helper_arg, g_base_self, &obj);
    }
    return 0;
}