#include <cstdio>
#include "engine/CScriptThing.h"
int g_def_004aa980=0;
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
void* CScriptThingVtbl::tgt(){ return (void*)0x1234; }
void* CScriptThing_Methods::GetPos(){ CScriptThingVtbl* p=(CScriptThingVtbl*)(void*)this->PImp_Data; if(!p) return &g_def_004aa980; return p->tgt(); }
int main(){ CScriptThingVtbl pp; CScriptThing_Methods o; o.PImp_Data=(CScriptThing*)(void*)&pp;
 if(o.GetPos()!=(void*)0x1234){ std::printf("004aa980_TEST FAIL\n"); return 1;}
 CScriptThing_Methods o2; o2.PImp_Data=0; if(o2.GetPos()!=&g_def_004aa980){ std::printf("004aa980_TEST FAIL\n"); return 1;}
 std::printf("004aa980_TEST PASS\n"); return 0;}
