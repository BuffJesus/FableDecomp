#include "rebuild_abi.h"
// CSpellContainerList::AddChild @ 0x006a4dd0
// mov eax,[esp+4]; push eax; call h1; mov ecx,eax; call h2; ret 4
// Wrap the argument via a __cdecl factory, then pass the result (in ecx) to a __fastcall consumer.
struct CSpellNode;
extern "C" CSpellNode *FABLE_STDCALL FableSpellWrap_6a4dd0(void *arg);
extern "C" void FABLE_FASTCALL FableSpellConsume_6a4dd0(CSpellNode *node);
struct CSpellContainerList { void AddChild(void *arg); };
void CSpellContainerList::AddChild(void *arg)
{
    FableSpellConsume_6a4dd0(FableSpellWrap_6a4dd0(arg));
}