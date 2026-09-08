#include "engine/CScriptThing.h"
extern int g_def_004aa980;
// Virtual-slot model of the pointee's vtable (slot 6 = GetPos); CScriptThing.h only exposes a plain __vftable.
struct CScriptThingVtbl {
  virtual void* d0(){return 0;}
  virtual void* d1(){return 0;}
  virtual void* d2(){return 0;}
  virtual void* d3(){return 0;}
  virtual void* d4(){return 0;}
  virtual void* d5(){return 0;}
  virtual void* tgt();
};
struct CScriptThing_Methods : CScriptThing { void* GetPos(); };
void* CScriptThing_Methods::GetPos(){ CScriptThingVtbl* p=(CScriptThingVtbl*)(void*)this->PImp_Data; if(!p) return &g_def_004aa980; return p->tgt(); }
