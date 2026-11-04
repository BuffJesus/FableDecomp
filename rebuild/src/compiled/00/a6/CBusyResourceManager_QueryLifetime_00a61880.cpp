// Native A61880 constructor / A61A80 ClearAllQueries, named by FableWin.
#include "fable_busy_resource_manager.h"
CBusyResourceManager::CBusyResourceManager() : PendingFrame(1),FinishedFrame(0) {}
void CBusyResourceManager::ClearAllQueries()
{
    while(PendingQueories.size()>0) {
        PendingQueories.back().Query->Release();
        PendingQueories.pop_back();
    }
    while(FreeQueoryObjects.size()>0) {
        FreeQueoryObjects.back().Query->Release();
        FreeQueoryObjects.pop_back();
    }
}
