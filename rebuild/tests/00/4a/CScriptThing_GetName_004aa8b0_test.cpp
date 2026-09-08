#include <cstdio>
int g_def_004aa8b0=0;
#include "engine/CScriptThing.h"
struct CScriptThingVirt {
  virtual void* d0(){return 0;}
  virtual void* GetName();
};
struct CScriptThing_Methods : CScriptThing {
  void* GetName();
};
void* CScriptThingVirt::GetName(){ return (void*)0x1234; }
void* CScriptThing_Methods::GetName(){
  CScriptThingVirt* p=(CScriptThingVirt*)this->PImp_Data;
  if(!p) return &g_def_004aa8b0;
  return p->GetName();
}
int main(){ CScriptThingVirt pp; CScriptThing_Methods o; o.PImp_Data=(CScriptThing*)&pp;
 if(o.GetName()!=(void*)0x1234){ std::printf("004aa8b0_TEST FAIL\n"); return 1;}
 CScriptThing_Methods o2; o2.PImp_Data=0; if(o2.GetName()!=&g_def_004aa8b0){ std::printf("004aa8b0_TEST FAIL\n"); return 1;}
 std::printf("004aa8b0_TEST PASS\n"); return 0;}
