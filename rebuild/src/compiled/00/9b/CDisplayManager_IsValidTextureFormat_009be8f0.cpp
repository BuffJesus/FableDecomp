extern unsigned int g_texFormatTable[];

struct CDispVtbl {
    int (__stdcall *fns[64])(void*, unsigned int, unsigned int, unsigned int, int, int, unsigned int);
};

struct CDisplayManager {
    char pad0[0x54];
    void* dev;        // +0x54
    char pad1[0x5c-0x58];
    unsigned int f5c; // +0x5c
    unsigned int f60; // +0x60
    char pad2[0x1c4-0x64];
    unsigned int f1c4; // +0x1c4
};

struct FmtHolder { unsigned int fmt; };

bool __fastcall CDisplayManager_IsValidTextureFormat(CDisplayManager* self, void* edx, FmtHolder* h)
{
    unsigned int idx = h->fmt;
    unsigned int entry = g_texFormatTable[idx * 9];
    void* dev = self->dev;
    CDispVtbl** vt = (CDispVtbl**)dev;
    int r = (*vt)->fns[0x28/4](dev, self->f60, self->f5c, self->f1c4, 0, 3, entry);
    return r >= 0;
}