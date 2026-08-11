#include <cstdio>

struct CCharString
{
	void* p;
};

struct CRegion
{
	void* m0;
	void SetMiniMapRegionExitTextOffsetX(CCharString* out, float unused);
};

// Stub the __cdecl helper: return a fixed sentinel so we can observe the swap.
static void* g_newval = (void*)0xABCD1234;
void* Region_ExitTextOffsetX_Alloc(void* old)
{
	(void)old;
	return g_newval;
}

void CRegion::SetMiniMapRegionExitTextOffsetX(CCharString* out, float unused)
{
	void* old = this->m0;
	this->m0 = Region_ExitTextOffsetX_Alloc(old);
	out->p = old;
	(void)unused;
}

int main()
{
	CRegion r;
	r.m0 = (void*)0x11112222;
	CCharString out;
	out.p = 0;

	r.SetMiniMapRegionExitTextOffsetX(&out, 3.5f);

	bool ok = (r.m0 == (void*)0xABCD1234) && (out.p == (void*)0x11112222);
	if (ok)
		printf("PASS_SETMINIMAPREGIONEXITTEXTOFFSETX\n");
	else
		printf("FAIL m0=%p out=%p\n", r.m0, out.p);
	return ok ? 0 : 1;
}