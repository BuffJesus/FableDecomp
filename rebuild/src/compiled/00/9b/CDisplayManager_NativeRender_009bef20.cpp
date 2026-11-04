#include "fable_display_viewport.h"

bool CDisplayManager::BeginRender()
{
    if(D3DDevice->BeginScene()>=0) {
        PRenderManager->BeginRender();
        CanBeginRender=true;
        return true;
    }
    return false;
}

void CDisplayManager::EndRender()
{
    PRenderManager->EndRender();
    if(D3DDevice->EndScene()>=0) {
        CanBeginRender=false;
        unknown215_[0]=0;
    }
}
