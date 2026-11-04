#include "fable_render_state_manager.h"
void CRenderStateManager::Init(IDirect3DDevice9* device)
{
    StateStackSize=0;
    StateUpdateListSize=0;
    FableCurrentRenderStateManager_013cb508=this;
    PD3DDevice=device;
    for(long i=0;i<AllStatesListSize;++i) AllStatesList[i]->StateType=0;
    AllStatesListSize=0;
    SetupRenderStates();
    BookmarkDummyState.StateType=4;
    CurrentBookmarkMask=1;
    SetDefaultRenderStates();
    ForceResendRenderStatesToHardware();
}
