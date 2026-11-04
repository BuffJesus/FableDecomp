#include "fable_display_engine.h"
#include "fable_progress_display.h"
#include "fable_iengine.h"
void CDisplayEngine::WorldUpdate()
{
    if(InitialFadeReadynessCount>0 && --InitialFadeReadynessCount==0){
        NProgressDisplay::StopProgress(true,false);
        GResetProgressTipText=L"";
        GResetProgressTipTitleText=L"";
    }
    if(InitialFadeReadynessCount>0)SetAboutToRun();
    if(Engine3D)GamePrimitiveDefaultFadeRangeRatio=Engine3D->GetPrimitiveDrawDistanceFactor()-1.0f;
}
