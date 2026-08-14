#include <stdio.h>

struct CTCVillage {
    void*        field_0;
    void*        field_4;
    char         pad8[4];
    unsigned char field_C;
    unsigned char field_D;
};

static int g_called = 0;
static void* g_ecx = 0;
static void* g_edx = 0;
static int   g_arg = -1;

void __fastcall Callee_86dd20(void* ecxArg, void* edxArg, unsigned char stackArg)
{
    g_called++;
    g_ecx = ecxArg;
    g_edx = edxArg;
    g_arg = (int)stackArg;
}

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->field_D) {
        Callee_86dd20(self->field_0, self->field_4, (int)self->field_C);
        self->field_D = 0;
    }
}

int main()
{
    // Case 1: flag set -> callee invoked, flag cleared
    CTCVillage v;
    v.field_0 = (void*)0x1111;
    v.field_4 = (void*)0x2222;
    v.field_C = 0x7A;
    v.field_D = 1;
    g_called = 0;
    OnInitialActivate(&v);
    if (g_called != 1) { printf("FAIL called=%d\n", g_called); return 1; }
    if (g_ecx != (void*)0x1111 || g_edx != (void*)0x2222 || g_arg != 0x7A) { printf("FAIL args\n"); return 1; }
    if (v.field_D != 0) { printf("FAIL flag not cleared\n"); return 1; }

    // Case 2: flag clear -> no call, no change
    CTCVillage v2;
    v2.field_0 = 0; v2.field_4 = 0; v2.field_C = 5; v2.field_D = 0;
    g_called = 0;
    OnInitialActivate(&v2);
    if (g_called != 0) { printf("FAIL should not call\n"); return 1; }
    if (v2.field_D != 0) { printf("FAIL flag changed\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}