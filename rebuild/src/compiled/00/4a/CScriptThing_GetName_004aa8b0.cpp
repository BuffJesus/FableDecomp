// CScriptThing_Methods::GetName  retail 0x004aa8b0
// mov eax,[ecx+4]; test eax,eax; jnz V; mov eax,offset g_def; ret
// V: mov edx,[eax]; mov ecx,eax; jmp/call [edx+4]  (virtual slot 1 on PImp_Data)
extern int g_def_004aa8b0;

#include "engine/CScriptThing.h"  // retyped onto the PDB layout; byte parity re-verified

// The header models the pointee's vtable as a plain `void* __vftable`; the
// virtual-slot dispatch is reproduced through a local vtable view cast from PImp_Data.
struct CScriptThingVirt {
  virtual void* d0(){return 0;}
  virtual void* GetName();
};

struct CScriptThing_Methods : CScriptThing {
  void* GetName();
};

void* CScriptThing_Methods::GetName(){
  CScriptThingVirt* p=(CScriptThingVirt*)this->PImp_Data;
  if(!p) return &g_def_004aa8b0;
  return p->GetName();
}
