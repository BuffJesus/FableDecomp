#include "rebuild_abi.h"
// CGameScriptInterface::IsHeroInProjectile @ 0x00890670
// mov eax,ds:g; mov ecx,[eax+0x1c]; call h1; mov ecx,eax; jmp h2
// Resolve a subsystem from a global singleton, then tail-call a query on it.
struct CSubA; struct CSubB;
extern void *g_singleton_890670;
extern "C" CSubB *FABLE_FASTCALL FableProjH1_890670(CSubA *a);
extern "C" bool FABLE_FASTCALL FableProjH2_890670(CSubB *b);
extern "C" bool FABLE_FASTCALL FableIsHeroInProjectile_890670()
{
    CSubA *a = *(CSubA**)((char*)g_singleton_890670 + 0x1c);
    return FableProjH2_890670(FableProjH1_890670(a));
}