#pragma optimize("s", on)
#include "fable_nui_manager.h"
#include "fable_nui_component.h"

void NUISystem::CFrontEndManager::Draw(CIEngine& engine, CEnginePrimitiveHandle* handle)
{
    for (_STL::map<FRONTEND_SCREEN, CComponent*>::iterator screen = m_mapScreens.begin();
         screen != m_mapScreens.end(); ++screen)
    {
        if (screen->second)
            screen->second->Draw(engine, handle, 0, 0, 0);
    }
}
