#include "fable_system_manager.h"
CProfileManager::CProfileManager()
    : UpPressed(false), DownPressed(false), TogglePressed(false),
      CursorPosition(0), NeedsSaving(false)
{
    if (!g_FableProfileManager_013CB4F8)
        g_FableProfileManager_013CB4F8=this;
    if (GFGetSystemManager()->PDisplayManager.Data)
        RenderManager=GFGetSystemManager()->PDisplayManager.Data->PRenderManager.get();
    ProfileTree.insert(ProfileTree.end());
    ProfileTree.begin()->Parent=0;
    CurrentTreeEntry=&ProfileTree.back();
    MinApplicationTotalTime=0.0;
    NoTurns=0;
    DrawMode=0;
    MinUpdated=false;
    EvalMode=1;
}