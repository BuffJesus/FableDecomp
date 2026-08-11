#include <cstdio>

struct CTCActivationReceptorBase {
    char pad0[0x0c];
    unsigned char f0c;
    char pad1[0x20 - 0x0d];
    unsigned char f20;
    char pad2[0x28 - 0x21];
    void* f28;
};

static int g_helper_calls = 0;
void* __fastcall helper_49d870(CTCActivationReceptorBase* self)
{
    (void)self;
    ++g_helper_calls;
    return (void*)0xDEADBEEF;
}

void __fastcall Activate(CTCActivationReceptorBase* self)
{
    self->f0c = 1;
    if (self->f20)
        self->f28 = helper_49d870(self);
}

int main()
{
    CTCActivationReceptorBase a;
    a.f0c = 0; a.f20 = 0; a.f28 = (void*)0x11;
    Activate(&a);
    bool okA = (a.f0c == 1) && (a.f28 == (void*)0x11) && (g_helper_calls == 0);

    CTCActivationReceptorBase b;
    b.f0c = 0; b.f20 = 1; b.f28 = (void*)0x22;
    Activate(&b);
    bool okB = (b.f0c == 1) && (b.f28 == (void*)0xDEADBEEF) && (g_helper_calls == 1);

    if (okA && okB)
        printf("007f76e0_TEST PASS\n");
    else
        printf("FAIL A=%d B=%d\n", okA, okB);
    return 0;
}