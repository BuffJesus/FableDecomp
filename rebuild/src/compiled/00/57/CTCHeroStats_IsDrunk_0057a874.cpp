#pragma optimize("s",on)
#include "engine/CTCHeroStats.h"  // retyped onto the PDB layout; byte parity re-verified
struct GBlob { char pad[0xa0]; float thresh; };
struct GHolder { char pad[0xdc]; GBlob* blob; };
extern "C" GHolder* GetGlobal(void);
bool __fastcall CTCHeroStats_IsDrunk(const CTCHeroStats* self)
{
    GHolder* h = GetGlobal();
    return self->BestScoreCoinGolfOakVale >= h->blob->thresh;
}