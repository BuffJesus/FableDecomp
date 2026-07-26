typedef int (__stdcall *PFN)(int obj, int f60, int f5c, int f1c4, int z, int three, int fourcc);

struct CDisplayManager {
    char pad0[0x54];
    void*** obj54;        // +0x54: object whose vtable holds the query method
    char pad1[0x5c-0x58];
    int f5c;              // +0x5c
    int f60;              // +0x60
    char pad2[0x1c4-0x64];
    int f1c4;             // +0x1c4
};

/* Returns bool (>=0). VC7.1 emits a 1-byte return; modeled as char for exact bytes. */
char __fastcall CDisplayManager_IsSupportingCompressedTextures(CDisplayManager* self)
{
    void*** o = self->obj54;
    return ((PFN)(*o)[10])((int)o, self->f60, self->f5c, self->f1c4, 0, 3, 0x31545844) >= 0;
}