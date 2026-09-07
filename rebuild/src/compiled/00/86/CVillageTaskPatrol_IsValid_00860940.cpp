#include "engine/CVillageTaskPatrol.h"  // retyped onto the PDB layout; byte parity re-verified
extern int __fastcall probe(void*);
bool __fastcall IsValid(CVillageTaskPatrol* self){ return probe(reinterpret_cast<char*>(self)+8) != 0; }