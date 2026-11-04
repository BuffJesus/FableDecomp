#include "fable_render_state_manager.h"
#include "fable_render_state.h"
#include <stdio.h>
#include <string.h>
typedef CRenderStateManager::CStateInfo State;
static unsigned long storage[0x3a3c/4],expected[0x3a3c/4];
static State states[256],expectedStates[256];
static FableRenderStateDeviceVTable table;
static FableRenderStateDevice devices[2];
struct Event{unsigned kind,index,device,stage,param,value,previous;};
static Event plan[256];static unsigned used,calls,errors,mutation,failure;
static void Mutate(unsigned long* object,State* state,unsigned call){
 if(mutation&1)object[1]=reinterpret_cast<unsigned long>(&devices[(call+1)%2]);
 if((mutation&2) && call==0)object[0x280c/4]=0;
 if(mutation&4){state->DesiredState^=0x80;state->CurrentState^=0x40;state->DirtyListFlag=true;}
}
static fable_i32 Dispatch(unsigned kind,FableRenderStateDevice* device,unsigned stage,unsigned param,unsigned value){
 if(calls>=used){++errors;return 0;}
 Event& e=plan[calls];State* state=&states[e.index];
 if(kind!=e.kind || device!=&devices[e.device] || stage!=e.stage || param!=e.param || value!=e.value || state->CurrentState!=e.previous || state->DirtyListFlag)++errors;
 Mutate(storage,state,calls++);return failure?static_cast<fable_i32>(0x80004005):0;
}
static fable_i32 __stdcall Render(FableRenderStateDevice* d,fable_u32 p,fable_u32 v){return Dispatch(1,d,0,p,v);}
static fable_i32 __stdcall Texture(FableRenderStateDevice* d,fable_u32 s,fable_u32 p,fable_u32 v){return Dispatch(2,d,s,p,v);}
static fable_i32 __stdcall Sampler(FableRenderStateDevice* d,fable_u32 s,fable_u32 p,fable_u32 v){return Dispatch(3,d,s,p,v);}
int main(){unsigned scenarios=0;const long counts[]={-1,0,1,2,17,256};const unsigned types[]={0,1,2,3,4,255};
 table.SetRenderState=&Render;table.SetTextureStageState=&Texture;table.SetSamplerState=&Sampler;devices[0].vtable=devices[1].vtable=&table;
 for(unsigned c=0;c<6;++c)for(unsigned type=0;type<6;++type)for(mutation=0;mutation<8;++mutation)for(failure=0;failure<2;++failure)for(unsigned duplicate=0;duplicate<2;++duplicate)for(unsigned equal=0;equal<2;++equal){
  memset(storage,0xa5,sizeof(storage));memset(states,0xb7,sizeof(states));storage[1]=reinterpret_cast<unsigned long>(&devices[0]);storage[0x280c/4]=counts[c];
  for(unsigned i=0;i<256;++i){states[i].StateType=static_cast<unsigned char>(types[(type+i)%6]);states[i].DesiredState=i*7+13;states[i].CurrentState=equal?states[i].DesiredState:0xffffffff;states[i].StateParam=i+19;states[i].TextureStage=static_cast<unsigned char>(i);storage[0x2008/4+i]=reinterpret_cast<unsigned long>(&states[duplicate?0:i]);}
  memcpy(expected,storage,sizeof(storage));memcpy(expectedStates,states,sizeof(states));used=calls=errors=0;
  for(long j=0;j<static_cast<long>(expected[0x280c/4]);++j){unsigned index=duplicate?0:j;State* state=&expectedStates[index];state->DirtyListFlag=false;
   if(state->CurrentState==state->DesiredState)continue;
   if(state->StateType>=1 && state->StateType<=3){Event& e=plan[used];e.kind=state->StateType;e.index=index;e.device=expected[1]==reinterpret_cast<unsigned long>(&devices[0])?0:1;e.stage=e.kind==1?0:state->TextureStage;e.param=state->StateParam;e.value=state->DesiredState;e.previous=state->CurrentState;Mutate(expected,state,used++);}
   state->CurrentState=state->DesiredState;
  }
  expected[0x280c/4]=0;reinterpret_cast<CRenderStateManager*>(storage)->RealiseRenderState();++scenarios;
  if(errors || calls!=used || memcmp(storage,expected,sizeof(storage)) || memcmp(states,expectedStates,sizeof(states))){printf("RENDER_STATE_REALISE_FAIL scenario=%u errors=%u calls=%u expected=%u\n",scenarios,errors,calls,used);return 1;}
 }
 printf("RENDER_STATE_REALISE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
