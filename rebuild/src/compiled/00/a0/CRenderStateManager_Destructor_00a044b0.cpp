#include "fable_render_state_manager.h"
// Retail 00A044B0 clears both registrations before destroying the base.
CRenderStateManager::~CRenderStateManager() {
    FableCurrentRenderStateManager_013cb508=0;
    PD3DDevice=0;
}
