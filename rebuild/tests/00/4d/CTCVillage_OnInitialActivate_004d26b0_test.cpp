#include <cstdio>

struct SubObj { int tag; };

static int g_calls = 0;
static int g_lastA1 = 0;
static unsigned char g_lastA2 = 0;

void __fastcall Sub_Call(SubObj* self, int a1, unsigned char a2)
{
    (void)self;
    g_calls++;
    g_lastA1 = a1;
    g_lastA2 = a2;
}

struct CTCVillage {
    SubObj* sub;
    int     field4;
    char    pad8[4];
    unsigned char b_c;
    unsigned char b_d;
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->b_d) {
        Sub_Call(self->sub, self->field4, self->b_c);
        self->b_d = 0;
    }
}

int main()
{
    SubObj s; s.tag = 1;
    CTCVillage v;
    v.sub = &s; v.field4 = 0x1234; v.b_c = 0x7f; v.b_d = 1;

    // flag set -> should call and clear
    OnInitialActivate(&v);
    if (g_calls != 1) { printf("FAIL calls\n"); return 1; }
    if (g_lastA1 != 0x1234) { printf("FAIL a1\n"); return 1; }
    if (g_lastA2 != 0x7f) { printf("FAIL a2\n"); return 1; }
    if (v.b_d != 0) { printf("FAIL flag not cleared\n"); return 1; }

    // flag already clear -> no call
    OnInitialActivate(&v);
    if (g_calls != 1) { printf("FAIL called again\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}