#include <cstdio>

struct CGuiVarTransferBool {
    void* f0;
    void* f4;
    char  pad8[4];
    bool  f0c;
    unsigned char f0d;
    void TransferToGui();
};

static void* g_ecx;
static void* g_edx;
static bool  g_b;
static bool  g_called;

void __fastcall helper_4d24f0(void* ecx, void* edx, bool b) {
    g_ecx = ecx; g_edx = edx; g_b = b; g_called = true;
}

extern void __fastcall helper_4d24f0(void* ecx, void* edx, bool b);

void CGuiVarTransferBool::TransferToGui()
{
    helper_4d24f0(this->f0, this->f4, this->f0c);
    this->f0d = 0;
}

int main() {
    CGuiVarTransferBool o;
    int a=1, b=2;
    o.f0 = &a; o.f4 = &b; o.f0c = true; o.f0d = 0x99;
    o.TransferToGui();
    if (g_called && g_ecx==&a && g_edx==&b && g_b==true && o.f0d==0)
        std::printf("004d2620_TEST PASS\n");
    else
        std::printf("FAIL\n");
    return 0;
}