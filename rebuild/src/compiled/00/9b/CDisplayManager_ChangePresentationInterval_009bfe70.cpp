#include "fable_display_viewport.h"

void CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc& desc)
{
    CCreateDeviceParameters parameters=LastDeviceCreationParameters;
    if (parameters.PresentationInterval.PresentImmediate!=desc.PresentImmediate ||
        parameters.PresentationInterval.VSyncCount!=desc.VSyncCount)
    {
        parameters.PresentationInterval=desc;
        ResetDevice(parameters);
    }
}

void CDisplayManager::ChangePresentationInterval(bool immediate,long interval,bool immediateIfSyncMissed)
{
    CPresentationIntervalDesc desc;
    desc.PresentImmediate=immediate;
    desc.PresentImmediateIfSyncMissed=immediateIfSyncMissed;
    desc.VSyncCount=interval;
    ChangePresentationInterval(desc);
}
