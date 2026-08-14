#pragma optimize("s",on)
// Byte-exact reconstruction of SetMiniMapRegionExitTextOffsetX @ 0x00578e02
// __fastcall void CRegion::SetMiniMapRegionExitTextOffsetX(CCharString* out, float unused)
// (rewritten as __fastcall; this in ecx)

struct CCharString {
    void* p;
};

// callee at 0x4292c0: takes the old pointer, returns a new pointer.
// value-returning cdecl helper (arg pushed, cleaned by caller pop ecx)
extern int __cdecl Helper(void* oldp);

struct CRegion {
    void* field0;
};

void __fastcall SetMiniMapRegionExitTextOffsetX(CRegion* self, void* /*edx*/, CCharString* out, float unused)
{
    void* old = self->field0;
    self->field0 = (void*)Helper(old);
    out->p = old;
}