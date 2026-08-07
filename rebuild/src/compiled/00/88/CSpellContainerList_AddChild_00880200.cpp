#include "rebuild_abi.h"
// CSpellContainerList::AddChild @ 0x00880200
// mov eax,[esp+4]; push eax; call h1; mov ecx,eax; call h2; ret 4
// Wrap the argument via a __stdcall factory, then pass the result (ecx) to a __fastcall consumer.
struct CSpellNode2;
extern "C" CSpellNode2 *FABLE_STDCALL FableSpellWrap_880200(void *arg);
extern "C" void FABLE_FASTCALL FableSpellConsume_880200(CSpellNode2 *node);
struct CSpellContainerList { void AddChild(void *arg); };
void CSpellContainerList::AddChild(void *arg)
{
    FableSpellConsume_880200(FableSpellWrap_880200(arg));
}