#include "fable_engine.h"
#include "fable_engine_component.h"

bool CEngine::OnPostDeviceReset()
{
    if(EnablePostDeviceReset)
        return SmartRestoreComponentVideoMemory()!=SMART_RESTORE_COMPONENT_VIDEO_MEMORY_FAILED;
    return true;
}

void CEngine::OnPreDeviceReset()
{
    for(unsigned long i=0;i<ScratchZBuffersD24S8.size();++i)
        delete ScratchZBuffersD24S8[i];
    ScratchZBuffersD24S8.clear();
    for(CEngineComponent** component=EngineComponents.begin();component!=EngineComponents.end();++component)
        (*component)->OnReleaseDefaultPoolResources();
}
