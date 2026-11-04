#include "fable_render_state_manager.h"
#include <stdio.h>
#include <string.h>
CRenderStateManager* FableCurrentRenderStateManager_013cb508;
static unsigned long storage[0x3a3c/4],expected[0x3a3c/4];
static CRenderStateManager::CStateInfo states[256],expectedStates[256];
static unsigned calls,errors,mutation;
static CRenderStateManager* Object(){return reinterpret_cast<CRenderStateManager*>(storage);}
static void Put(unsigned long* base,unsigned offset,unsigned long value){base[offset/4]=value;}
static void Check(){if(memcmp(storage,expected,sizeof(storage)) || memcmp(states,expectedStates,sizeof(states)))++errors;}
void CRenderStateManager::SetupRenderStates(){
 if(this!=Object() || calls++!=0 || FableCurrentRenderStateManager_013cb508!=Object())++errors;Check();
 if(mutation&1){Put(storage,0x2810,7);Put(expected,0x2810,7);}
 if(mutation&2)FableCurrentRenderStateManager_013cb508=0;
 reinterpret_cast<unsigned char*>(expected)[0x3a35]=4;Put(expected,0x2814,1);
}
void CRenderStateManager::SetDefaultRenderStates(){
 if(this!=Object() || calls++!=1)++errors;Check();
 if(mutation&4){Put(storage,0x2814,0x20);Put(expected,0x2814,0x20);}
}
void CRenderStateManager::ForceResendRenderStatesToHardware(){
 if(this!=Object() || calls++!=2)++errors;Check();
 if(mutation&8){Put(storage,0x280c,13);Put(expected,0x280c,13);}
}
int main(){
 const long counts[]={-1,0,1,2,17,255,256};unsigned scenarios=0;
 for(unsigned c=0;c<7;++c)for(unsigned duplicate=0;duplicate<2;++duplicate)for(mutation=0;mutation<16;++mutation){
  memset(storage,0xa5,sizeof(storage));memset(states,0xb7,sizeof(states));
  for(unsigned i=0;i<256;++i)Put(storage,0x2408+4*i,reinterpret_cast<unsigned long>(&states[duplicate?0:i]));
  Put(storage,0x2810,counts[c]);memcpy(expected,storage,sizeof(storage));memcpy(expectedStates,states,sizeof(states));
  Put(expected,0x2808,0);Put(expected,0x280c,0);Put(expected,4,0x12340000);
  for(long j=0;j<counts[c];++j)expectedStates[duplicate?0:j].StateType=0;
  Put(expected,0x2810,0);calls=errors=0;FableCurrentRenderStateManager_013cb508=0;
  Object()->Init(reinterpret_cast<IDirect3DDevice9*>(0x12340000));Check();++scenarios;
  if(errors || calls!=3 || FableCurrentRenderStateManager_013cb508!=((mutation&2)?0:Object())){
   printf("RENDER_STATE_INIT_FAIL scenario=%u errors=%u calls=%u\n",scenarios,errors,calls);return 1;}
 }
 printf("RENDER_STATE_INIT_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
