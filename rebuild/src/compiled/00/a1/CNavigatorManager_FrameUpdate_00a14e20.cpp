// CNavigatorManager frame/area members and the queue's navigator removal
// (navigation manager TU, retail 0x00A14E20..0x00A155C0; Ego_r.pdb / FableWin.pdb names).
//   0x00A14E20 CNavigatorManager::FrameUpdate: refill the per-frame node-examination
//              budget (+0x20 = 800; caller 0x004A5CF3 via [esi+0x48]).
//   0x00A14E30 / 0x00A14E40 CNavigatorManager::UpdateNavigationMapArea (CNavigationAreaInit /
//              C2DBoxF overloads; retail bodies are empty `ret 8`; 0x004FEC4A passes a
//              C2DBoxF built from a C2DBoxI).
//   0x00A155C0 CNavigatorQueueManager::RemoveNavigatorFromQueue: five callers pass a
//              navigator and its NavigationManager+0x2C (QueueManager); tail-calls
//              list::remove_if<CIsNavigator> 0x00A16EB0.
#include "fable_navigator.h"
#include "fable_stlport_list_remove_if_impl.h"

void CNavigatorManager::FrameUpdate()
{
    NumberOfNodeExaminationsLeftThisFrame = 800;
}

void CNavigatorManager::UpdateNavigationMapArea(long, const CNavigationAreaInit&)
{
}

void CNavigatorManager::UpdateNavigationMapArea(long, const C2DBoxF&)
{
}

void CNavigatorQueueManager::RemoveNavigatorFromQueue(const CNavigatorBase& navigator)
{
    Navigators.remove_if(CIsNavigator(navigator));
}
