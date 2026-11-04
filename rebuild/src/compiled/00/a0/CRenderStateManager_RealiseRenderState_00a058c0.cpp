#include "fable_render_state.h"

void CRenderStateManager::RealiseRenderState()
{
    for (fable_i32 index = 0; index < StateUpdateListSize; ++index)
    {
        CStateInfo* state = StateUpdateList[index];
        fable_u32 desired = state->DesiredState;
        state->DirtyListFlag = 0;
        if (desired != state->CurrentState)
        {
            FableRenderStateDevice* device=static_cast<FableRenderStateDevice*>(PD3DDevice);
            switch (state->StateType)
            {
            case 1:
                device->vtable->SetRenderState(
                    device,
                    state->StateParam,
                    desired);
                break;
            case 2:
                device->vtable->SetTextureStageState(
                    device,
                    state->TextureStage,
                    state->StateParam,
                    desired);
                break;
            case 3:
                device->vtable->SetSamplerState(
                    device,
                    state->TextureStage,
                    state->StateParam,
                    desired);
                break;
            }
            state->CurrentState = state->DesiredState;
        }
    }
    StateUpdateListSize = 0;
}
