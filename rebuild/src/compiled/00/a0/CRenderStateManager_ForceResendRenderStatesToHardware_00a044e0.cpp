#include "fable_render_state_manager.h"
#include "fable_render_state.h"
void CRenderStateManager::ForceResendRenderStatesToHardware(){
 for(long i=0;i<AllStatesListSize;++i){
  CStateInfo* state=AllStatesList[i];
  unsigned long desired=state->DesiredState;
  state->CurrentState=desired;
  state->DirtyListFlag=false;
  FableRenderStateDevice* device=static_cast<FableRenderStateDevice*>(PD3DDevice);
  switch(state->StateType){
   case 1:device->vtable->SetRenderState(device,state->StateParam,desired);break;
   case 2:device->vtable->SetTextureStageState(device,state->TextureStage,state->StateParam,desired);break;
   case 3:device->vtable->SetSamplerState(device,state->TextureStage,state->StateParam,desired);break;
  }
 }
 StateUpdateListSize=0;
}
