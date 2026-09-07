#include "engine/CSpawnedFuncBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPredicateBase { virtual bool v0(); };
bool __fastcall TestPredicate(CSpawnedFuncBase* self){ CPredicateBase* p = self->PPredicate_Data; if(!p) return true; return p->v0(); }