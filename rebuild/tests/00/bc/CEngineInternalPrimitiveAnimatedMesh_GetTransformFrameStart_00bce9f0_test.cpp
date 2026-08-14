#include <cstdio>

struct CMatrix3x4 { char pad[64]; };

struct Mesh {
    char pad[0x58];
    int  m58;
    char pad2[0x88 - 0x5c];
    char m88[0x1c];
    char mA4[0x1c];
};

static int  g_val58;
static void* g_pA4;
static void* g_edx;
static CMatrix3x4* g_mtx;

int __fastcall Callee(CMatrix3x4* mtx, void* edxArg, void* pA4, int val58)
{
    g_mtx = mtx; g_edx = edxArg; g_pA4 = pA4; g_val58 = val58;
    return 7;
}

bool __fastcall GetTransformFrameStart(Mesh* self, void* edxDummy, CMatrix3x4& mtx)
{
    Callee(&mtx, self->m88, self->mA4, self->m58);
    return true;
}

int main()
{
    Mesh m;
    m.m58 = 0x1234;
    CMatrix3x4 mtx;
    bool r = GetTransformFrameStart(&m, 0, mtx);
    if (r && g_mtx == &mtx && g_edx == (void*)m.m88 && g_pA4 == (void*)m.mA4 && g_val58 == 0x1234) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}