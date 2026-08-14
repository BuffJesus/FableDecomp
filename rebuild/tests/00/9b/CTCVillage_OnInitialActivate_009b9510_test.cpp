#include <stdio.h>

struct CTCVillage {
    void* field0;
    void* field4;
    char  pad8[4];
    unsigned char b0c;
    unsigned char b0d;
};

static void* g_self;
static void* g_a1;
static unsigned char g_a2;
static int g_calls;

void __fastcall Callee(void* self, void* a1, unsigned char a2)
{
    g_self = self; g_a1 = a1; g_a2 = a2; g_calls++;
}

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->b0d) {
        Callee(self->field0, self->field4, self->b0c);
        self->b0d = 0;
    }
}

int main()
{
    CTCVillage v;
    v.field0 = (void*)0x1111;
    v.field4 = (void*)0x2222;
    v.b0c = 0x37;
    v.b0d = 1;

    OnInitialActivate(&v);
    if (g_calls != 1) { printf("BAD calls\n"); return 1; }
    if (g_self != (void*)0x1111) { printf("BAD self\n"); return 1; }
    if (g_a1 != (void*)0x2222) { printf("BAD a1\n"); return 1; }
    if (g_a2 != 0x37) { printf("BAD a2\n"); return 1; }
    if (v.b0d != 0) { printf("BAD flag\n"); return 1; }

    // second call: flag now clear -> no call
    OnInitialActivate(&v);
    if (g_calls != 1) { printf("BAD calls2\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}