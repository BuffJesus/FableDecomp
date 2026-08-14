// CEngineLandscapeMap::ReleaseDefaultPoolSurfaces  0x00bdc040
// this-call void(CEngineLandscapeMap*)

struct ISurface {
    // vtable slot 0 is a __fastcall virtual taking one int arg
    virtual void Slot0(int a) = 0;
};

struct CEngineLandscapeMap {
    char pad0[4];
    ISurface* surface;   // +0x04
    char pad1[0x5d - 0x08];
    unsigned char flag;  // +0x5d
};

void __fastcall ReleaseDefaultPoolSurfaces(CEngineLandscapeMap* self)
{
    if (self->flag) {
        ISurface* s = self->surface;
        if (s) {
            s->Slot0(1);
        }
        self->surface = 0;
    }
}