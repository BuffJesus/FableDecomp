#include <cstdio>

struct Sub { int tag; };

static int g_called = 0;
static int g_edx = 0;
static unsigned char g_stack = 0;
static Sub* g_this = 0;

void __fastcall Sub_Handle(Sub* thisp, int edxArg, unsigned char stackArg)
{
    g_called++;
    g_this = thisp;
    g_edx = edxArg;
    g_stack = stackArg;
}

struct CTCVillage {
    Sub*          f0;
    int           f4;
    char          pad8[4];
    unsigned char f0c;
    unsigned char f0d;
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->f0d) {
        Sub_Handle(self->f0, self->f4, self->f0c);
        self->f0d = 0;
    }
}

int main()
{
    Sub s; s.tag = 77;
    CTCVillage v;
    v.f0 = &s; v.f4 = 0x1234; v.f0c = 0xAB; v.f0d = 1;

    OnInitialActivate(&v);
    if (g_called != 1 || g_this != &s || g_edx != 0x1234 || g_stack != 0xAB || v.f0d != 0) {
        printf("FAIL1\n"); return 1;
    }

    // second call: flag now cleared, must not call
    OnInitialActivate(&v);
    if (g_called != 1) { printf("FAIL2\n"); return 1; }

    printf("VILLAGE_INIT_OK\n");
    return 0;
}