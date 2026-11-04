#include <new>
#include "fable_display_engine.h"
#include "fable_display_view_manager.h"
#include "fable_progress_display.h"

void CDisplayEngine::Init()
{
    {
        CCharString text("Init Display Engine: Init Graphics",-1);
        NProgressDisplay::DisplayProgress(text,-1.0f,false,false);
    }
    DrawGame=true;Frame=0;
    {
        CCharString text("Init Display Engine: Init Engine",-1);
        NProgressDisplay::DisplayProgress(text,-1.0f,false,false);
    }
    InitEngine();
    LastRefreshTime=0;CameraInputsOn=true;
    PViewManager.Reset(new CDisplayViewManager);
}
