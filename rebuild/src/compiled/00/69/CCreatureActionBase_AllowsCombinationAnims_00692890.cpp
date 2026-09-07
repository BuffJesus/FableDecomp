#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureActionBase; extern int __fastcall sub(void* p);
struct CCreatureActionBase_Methods : CCreatureActionBase {
    char M();
};
char CCreatureActionBase_Methods::M(){ unsigned int r=(unsigned int)sub(this->PAnimEntry_Data); return (char)((~(r>>3))&1); }