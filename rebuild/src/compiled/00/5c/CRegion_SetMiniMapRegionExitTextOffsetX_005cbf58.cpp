#pragma optimize("s",on)
// CRegion::SetMiniMapRegionExitTextOffsetX  @ 0x005cbf58
// __fastcall void(CRegion*, CCharString*, float)
//
// push esi; mov esi,ecx; push edi; mov edi,[esi]; push edi; call 0x4292c0
// mov [esi],eax; mov eax,[esp+0x10]; pop ecx; mov [eax],edi; ...; ret 8
//
// Pattern: swap self->m0 with the value produced by the __cdecl helper
// (which takes the old self->m0), and hand the OLD self->m0 back through
// the first out-parameter. The float argument is unused.

// __cdecl helper at 0x4292c0: takes one pointer, returns a pointer.
void* Region_ExitTextOffsetX_Alloc(void* old);

struct CCharString
{
	void* p;
};

struct CRegion
{
	void* m0;

	void SetMiniMapRegionExitTextOffsetX(CCharString* out, float unused);
};

void CRegion::SetMiniMapRegionExitTextOffsetX(CCharString* out, float unused)
{
	void* old = this->m0;
	this->m0 = Region_ExitTextOffsetX_Alloc(old);
	out->p = old;
	(void)unused;
}