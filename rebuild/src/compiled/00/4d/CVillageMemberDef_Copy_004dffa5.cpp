#include "engine/CVillageMemberDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase;

struct CVillageMemberBase {
  void Duplicate(const CDefClassBase* src);  // 0x431f10, __fastcall
};


void __fastcall CVillageMemberDef_Copy(CVillageMemberDef* self, void* edx, const CDefClassBase* src)
{
  reinterpret_cast<CVillageMemberBase*>(self)->Duplicate(src);
  self->NeedsVillageToFunctionCorrectly = reinterpret_cast<const CVillageMemberDef*>(src)->NeedsVillageToFunctionCorrectly;
}