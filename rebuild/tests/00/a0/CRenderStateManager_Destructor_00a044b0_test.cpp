#include "fable_render_state_manager.h"
#include <stdio.h>
#include <string.h>
struct CRenderStateManagerTestAccess {static void*& Device(CRenderStateManager* p){return p->PD3DDevice;}static unsigned long& Flags(CRenderStateManager* p){return p->InitFlags;}static unsigned Offset(){return offsetof(CRenderStateManager,InitFlags);}};
typedef CRenderStateManagerTestAccess A;
static CRenderStateManager* manager;static int failures,calls,mutate;static void* observedVtable;
CRenderStateManager* FableCurrentRenderStateManager_013cb508;
static void check(bool b){if(!b)++failures;}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){
 check(this==manager && !FableCurrentRenderStateManager_013cb508 && !A::Device(manager));++calls;
 void* table=*reinterpret_cast<void**>(this);check(table!=0);if(observedVtable)check(observedVtable==table);observedVtable=table;
 if(mutate){A::Flags(manager)=0x12345678;FableCurrentRenderStateManager_013cb508=reinterpret_cast<CRenderStateManager*>(0x432100);}
}
int main(){unsigned scenarios=0;
 for(unsigned fill=0;fill<256;++fill)for(int initial=0;initial<3;++initial)for(mutate=0;mutate<2;++mutate){
  unsigned long storage[sizeof(CRenderStateManager)/4];unsigned char expected[sizeof(storage)];memset(storage,fill,sizeof(storage));memcpy(expected,storage,sizeof(storage));manager=reinterpret_cast<CRenderStateManager*>(storage);FableCurrentRenderStateManager_013cb508=initial==0?0:initial==1?manager:reinterpret_cast<CRenderStateManager*>(0x112200);calls=0;
  manager->CRenderStateManager::~CRenderStateManager();check(calls==1 && !A::Device(manager));check(FableCurrentRenderStateManager_013cb508==(mutate?reinterpret_cast<CRenderStateManager*>(0x432100):0));
  memset(expected+4,0,4);if(mutate){unsigned long value=0x12345678;memcpy(expected+A::Offset(),&value,4);}check(memcmp(expected+4,reinterpret_cast<char*>(storage)+4,sizeof(storage)-4)==0);++scenarios;
 }
 printf("RENDER_STATE_DESTRUCTOR_BEHAVIOR_%s scenarios=%u failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
