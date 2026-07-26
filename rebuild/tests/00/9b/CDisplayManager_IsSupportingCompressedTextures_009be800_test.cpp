#include <cstdio>

typedef int (__stdcall *PFN)(int obj, int f60, int f5c, int f1c4, int z, int three, int fourcc);

struct CDisplayManager {
    char pad0[0x54];
    void*** obj54;
    char pad1[0x5c-0x58];
    int f5c;
    int f60;
    char pad2[0x1c4-0x64];
    int f1c4;
};

char __fastcall CDisplayManager_IsSupportingCompressedTextures(CDisplayManager* self)
{
    void*** o = self->obj54;
    return ((PFN)(*o)[10])((int)o, self->f60, self->f5c, self->f1c4, 0, 3, 0x31545844) >= 0;
}

static int g_ret;
static int g_seen[7];

static int __stdcall stub(int obj, int f60, int f5c, int f1c4, int z, int three, int fourcc)
{
    g_seen[0]=obj; g_seen[1]=f60; g_seen[2]=f5c; g_seen[3]=f1c4;
    g_seen[4]=z; g_seen[5]=three; g_seen[6]=fourcc;
    return g_ret;
}

int main()
{
    static void* vtbl[16];
    vtbl[10] = (void*)&stub;
    void** vtp = vtbl;

    CDisplayManager dm;
    dm.obj54 = &vtp;   /* *(*obj54) == vtbl */
    dm.f5c  = 0xAAAA;
    dm.f60  = 0xBBBB;
    dm.f1c4 = 0xCCCC;

    g_ret = 0;
    if (!CDisplayManager_IsSupportingCompressedTextures(&dm)) { std::printf("FAIL: 0 should be true\n"); return 1; }

    g_ret = -1;
    if (CDisplayManager_IsSupportingCompressedTextures(&dm)) { std::printf("FAIL: -1 should be false\n"); return 1; }

    g_ret = 7;
    if (!CDisplayManager_IsSupportingCompressedTextures(&dm)) { std::printf("FAIL: 7 should be true\n"); return 1; }

    if (g_seen[0] != (int)dm.obj54) { std::printf("FAIL: arg0 obj\n"); return 1; }
    if (g_seen[1] != 0xBBBB) { std::printf("FAIL: arg1 f60\n"); return 1; }
    if (g_seen[2] != 0xAAAA) { std::printf("FAIL: arg2 f5c\n"); return 1; }
    if (g_seen[3] != 0xCCCC) { std::printf("FAIL: arg3 f1c4\n"); return 1; }
    if (g_seen[4] != 0) { std::printf("FAIL: arg4\n"); return 1; }
    if (g_seen[5] != 3) { std::printf("FAIL: arg5\n"); return 1; }
    if (g_seen[6] != 0x31545844) { std::printf("FAIL: arg6 fourcc\n"); return 1; }

    std::printf("CDisplayManager_009be800_TEST PASS\n");
    return 0;
}