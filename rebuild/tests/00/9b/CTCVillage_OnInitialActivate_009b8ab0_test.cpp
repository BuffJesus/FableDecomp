#include <cstdio>

struct Sub;

static int g_called = 0;
static int g_a = 0;
static int g_b = 0;

void __fastcall Sub_Do(Sub* self, int a, unsigned char b)
{
    g_called++;
    g_a = a;
    g_b = b;
    (void)self;
}

struct CTCVillage {
    Sub* m0;
    int  m4;
    unsigned char m8;
    unsigned char m9;
    unsigned char ma;
    unsigned char mb;
    unsigned char mC;
    unsigned char mD;
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->mD) {
        Sub_Do((Sub*)self->m0, self->m4, self->mC);
        self->mD = 0;
    }
}

int main()
{
    CTCVillage v;
    v.m0 = (Sub*)0x1234;
    v.m4 = 77;
    v.mC = 42;
    v.mD = 1;
    OnInitialActivate(&v);
    if (g_called == 1 && g_a == 77 && g_b == 42 && v.mD == 0) {
        g_called = 0;
        v.mD = 0;
        OnInitialActivate(&v);
        if (g_called == 0) {
            printf("PARITY_OK\n");
        }
    }
    return 0;
}