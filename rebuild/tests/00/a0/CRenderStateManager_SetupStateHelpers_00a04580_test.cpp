#include "fable_render_state_manager.h"
D3DRENDERSTATETYPE FableRenderWrapStateTypes_01396da8[8];
#include <stdio.h>
#include <string.h>
struct CRenderStateManagerTestAccess {
 static void Run(CRenderStateManager* p,unsigned kind,CRenderStateManager::CStateInfo& state,unsigned long type,long stage){
  if(kind==1)p->SetupRenderState(state,static_cast<D3DRENDERSTATETYPE>(type));
  else if(kind==2)p->SetupTextureStageState(state,static_cast<D3DTEXTURESTAGESTATETYPE>(type),stage);
  else p->SetupSamplerStageState(state,static_cast<D3DSAMPLERSTATETYPE>(type),stage);
 }
};
int main(){unsigned long storage[0x3a3c/4],expected[0x3a3c/4];CRenderStateManager::CStateInfo state,wanted;
 const unsigned counts[]={0,1,127,255};const long stages[]={-1,0,1,255,256,65535,(-2147483647-1)};const unsigned long types[]={0,1,255,0x7fffffff,0xffffffff};unsigned scenarios=0;
 for(unsigned kind=1;kind<=3;++kind)for(unsigned c=0;c<4;++c)for(unsigned s=0;s<7;++s)for(unsigned t=0;t<5;++t)for(unsigned pattern=0;pattern<4;++pattern){
  memset(storage,0x35+pattern,sizeof(storage));memset(&state,0x87+pattern,sizeof(state));storage[0x2810/4]=counts[c];memcpy(expected,storage,sizeof(storage));wanted=state;
  wanted.CurrentState=0;wanted.DesiredState=0;wanted.DirtyListFlag=false;wanted.StateType=static_cast<unsigned char>(kind);wanted.StateParam=types[t];if(kind>1)wanted.TextureStage=static_cast<unsigned char>(stages[s]);
  expected[0x2408/4+counts[c]]=reinterpret_cast<unsigned long>(&state);expected[0x2810/4]=counts[c]+1;
  CRenderStateManagerTestAccess::Run(reinterpret_cast<CRenderStateManager*>(storage),kind,state,types[t],stages[s]);++scenarios;
  if(memcmp(storage,expected,sizeof(storage)) || memcmp(&state,&wanted,sizeof(state)))return 1;
 }
 printf("RENDER_STATE_SETUP_HELPERS_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
