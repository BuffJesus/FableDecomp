#include <cstdio>

struct CMatrix3x4 { int tag; };

static CMatrix3x4* g_m;
static void* g_sub;
static void* g_p;
static int g_v;

extern "C" void __fastcall FrameEndCallee(CMatrix3x4* matrix, void* sub, void* p, int v)
{
    g_m = matrix; g_sub = sub; g_p = p; g_v = v;
}

struct Mesh {
    char pad0[0x58];
    int field58;
    char pad1[0x94 - 0x58 - 4];
    char field94[0x100];
};

bool __fastcall GetTransformFrameEnd(Mesh* self, void* edx, CMatrix3x4& matrix)
{
    char* base = (char*)self;
    FrameEndCallee(&matrix, base + 0x94, base + 0xbc, self->field58);
    return true;
}

int main()
{
    Mesh m;
    m.field58 = 0x1234;
    CMatrix3x4 mat; mat.tag = 7;
    bool r = GetTransformFrameEnd(&m, 0, mat);
    char* base = (char*)&m;
    if (r && g_m == &mat && g_sub == base + 0x94 && g_p == base + 0xbc && g_v == 0x1234)
        printf("PARITY_OK\n");
    else
        printf("PARITY_FAIL\n");
    return 0;
}