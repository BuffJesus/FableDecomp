struct CBase;

// The retail function is a thiscall member of CTCHeroStats whose body never
// touches 'this' (in ecx) -- it only forwards the CBase* event argument
// (passed on the stack, since it is not the implicit 'this') straight
// through to an external notification routine reached via the IAT. Modeled
// here as the __cdecl free routine the compiler actually emits for such a
// body: 'this' is dropped entirely (never read), leaving the sole stack
// parameter at [esp+4], exactly as retail computes it.
extern "C" void (__stdcall *g_HeroStatsEventNotify)(CBase* e);

extern "C" void __cdecl CTCHeroStats_InformOfEvent_b(CBase* e)
{
    g_HeroStatsEventNotify(e);
}