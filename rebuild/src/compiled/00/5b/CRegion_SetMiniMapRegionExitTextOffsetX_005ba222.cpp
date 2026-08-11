#pragma optimize("s",on)
// CRegion::SetMiniMapRegionExitTextOffsetX @ 005ba222
struct CRegion {
    void* f0;
};

extern "C" void* helper_4292c0(void* p);

CRegion* __fastcall SetMiniMapRegionExitTextOffsetX(CRegion* self)
{
    self->f0 = helper_4292c0(self->f0);
    return self;
}