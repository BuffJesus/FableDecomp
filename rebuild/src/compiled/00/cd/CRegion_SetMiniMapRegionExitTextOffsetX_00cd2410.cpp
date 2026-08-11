#pragma optimize("s",on)
struct CRegion;

extern "C" void* __cdecl helper_4292c0(void* arg);

struct CRegion {
    void* f0;
};

CRegion* __fastcall SetMiniMapRegionExitTextOffsetX(CRegion* self)
{
    self->f0 = helper_4292c0(self->f0);
    return self;
}