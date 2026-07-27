#include "fable_render_state.h"

void CRenderStateManagerRealiseView::RealiseRenderState()
{
    for (fable_i32 index = 0; index < StateUpdateListSize280C; ++index)
    {
        CRenderStateInfo* state = StateUpdateList2008[index];
        fable_u32 desired = state->DesiredState;
        state->DirtyListFlag = 0;
        if (desired != state->CurrentState)
        {
            switch (state->StateType)
            {
            case 1:
                PD3DDevice0004->vtable->SetRenderState(
                    PD3DDevice0004,
                    state->StateParam,
                    desired);
                break;
            case 2:
                PD3DDevice0004->vtable->SetTextureStageState(
                    PD3DDevice0004,
                    state->TextureStage,
                    state->StateParam,
                    desired);
                break;
            case 3:
                PD3DDevice0004->vtable->SetSamplerState(
                    PD3DDevice0004,
                    state->TextureStage,
                    state->StateParam,
                    desired);
                break;
            }
            state->CurrentState = state->DesiredState;
        }
    }
    StateUpdateListSize280C = 0;
}
