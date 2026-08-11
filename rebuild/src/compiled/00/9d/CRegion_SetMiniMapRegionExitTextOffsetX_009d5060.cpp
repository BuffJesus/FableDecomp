// CRegion::SetMiniMapRegionExitTextOffsetX @ 009d5060
struct CRegion;

struct Singleton {
    void* method(void* arg);  // __fastcall, ecx=this, pushes arg; at 0x9d4f90
};

extern Singleton g_singleton_13ca828;

struct CRegion {
    void* f0;
};

void __fastcall SetMiniMapRegionExitTextOffsetX(CRegion* self, int /*edx*/, void* arg)
{
    self->f0 = g_singleton_13ca828.method(arg);
}