#include <cstdio>

unsigned int g_texFormatTable[64*9];

struct CDispVtbl {
    int (__stdcall *fns[64])(void*, unsigned int, unsigned int, unsigned int, int, int, unsigned int);
};

struct CDisplayManager {
    char pad0[0x54];
    void* dev;
    char pad1[0x5c-0x58];
    unsigned int f5c;
    unsigned int f60;
    char pad2[0x1c4-0x64];
    unsigned int f1c4;
};

struct FmtHolder { unsigned int fmt; };

int g_lastEntry;
int __stdcall stub_fn(void* dev, unsigned int a, unsigned int b, unsigned int c, int d, int e, unsigned int entry){
    g_lastEntry = (int)entry;
    return (int)entry - 100; // >=0 when entry>=100
}

bool __fastcall CDisplayManager_IsValidTextureFormat(CDisplayManager* self, void* edx, FmtHolder* h)
{
    unsigned int idx = h->fmt;
    unsigned int entry = g_texFormatTable[idx * 9];
    void* dev = self->dev;
    CDispVtbl** vt = (CDispVtbl**)dev;
    int r = (*vt)->fns[0x28/4](dev, self->f60, self->f5c, self->f1c4, 0, 3, entry);
    return r >= 0;
}

int main(){
    static CDispVtbl vt;
    vt.fns[0x28/4] = stub_fn;
    CDispVtbl* pvt = &vt;

    CDisplayManager dm;
    dm.dev = &pvt;
    dm.f5c = 11; dm.f60 = 22; dm.f1c4 = 33;

    g_texFormatTable[5*9] = 150;
    FmtHolder h; h.fmt = 5;
    bool ok = CDisplayManager_IsValidTextureFormat(&dm, 0, &h);
    if(!ok){ std::printf("FAIL ok\n"); return 1; }
    if(g_lastEntry != 150){ std::printf("FAIL entry %d\n", g_lastEntry); return 1; }

    g_texFormatTable[3*9] = 50;
    h.fmt = 3;
    bool ok2 = CDisplayManager_IsValidTextureFormat(&dm, 0, &h);
    if(ok2){ std::printf("FAIL ok2\n"); return 1; }

    std::printf("CDisplayManager_009be8f0_TEST PASS\n");
    return 0;
}