#include "engine/CGraphicDataBank.h"

void CGraphicDataBank::ChangeScene()
{
    ResourceList.IncrementFrameCounter(UnloadFrameDelay);
    LoadingQueue.IncrementFrameCounter(UnloadFrameDelay);
}

void CGraphicDataBank::SetReduceSizeFrameDelay(unsigned long delay)
{
    ReduceSizeFrameDelay=delay;
    ResourceList.SetResourceUnloadDelay(delay);
}
