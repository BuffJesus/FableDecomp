#include "fable_render_state_manager.h"
CRenderStateManager::CDeviceState::CDeviceState() {}
__declspec(noinline) CRenderStateManager::CRenderStateManager(unsigned long flags)
{
    StateStackSize=0;
    StateUpdateListSize=0;
    AllStatesListSize=0;
    InitFlags=flags;
    BookmarkDummyState.StateType=4;
    CurrentBookmarkMask=1;
}

CRenderManagerCore::CRenderManagerCore(CDisplayManager& display,unsigned long flags)
    : CRenderStateManager(flags), DisplayManager(&display)
{
    for(unsigned i=0;i<8;++i) CurrentlySelectedTextures[i]=0;
}
CRenderManagerCore::~CRenderManagerCore() {
    if(SystemFont.IsValid()) SystemFont.Uninitialise();
}
