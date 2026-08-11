#include <cstdio>

static void* g_ecx;
static void* g_edx;
static bool  g_b;
static int   g_called;

void __fastcall helper_4711b3(void* ecx, void* edx, bool b)
{
    g_ecx = ecx; g_edx = edx; g_b = b; g_called = 1;
}

struct CGuiVarTransferBool {
    void* f0;
    void* f4;
    char  pad8[4];
    bool  f0c;
    unsigned char f0d;
    void TransferToGui();
};

void CGuiVarTransferBool::TransferToGui()
{
    helper_4711b3(this->f0, this->f4, this->f0c);
    this->f0d = 0;
}

int main()
{
    CGuiVarTransferBool o;
    o.f0 = (void*)0x11110000;
    o.f4 = (void*)0x22220000;
    o.f0c = true;
    o.f0d = 0x55;   // must be cleared to 0

    o.TransferToGui();

    if (g_called && g_ecx == (void*)0x11110000 && g_edx == (void*)0x22220000 &&
        g_b == true && o.f0d == 0)
        std::printf("0047119a_TEST PASS\n");
    else
        std::printf("FAIL called=%d ecx=%p edx=%p b=%d f0d=%d\n",
                    g_called, g_ecx, g_edx, (int)g_b, o.f0d);
    return 0;
}