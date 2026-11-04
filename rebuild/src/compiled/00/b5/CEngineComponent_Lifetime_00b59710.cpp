#include "fable_engine_component.h"
#include "fable_string.h"
CEngineComponent::CEngineComponent()
 :GlobalPointerAddress(0),RenderEnabled(true),KeepAlive(0) {}
CEngineComponent::~CEngineComponent()
{
    if(GlobalPointerAddress)*GlobalPointerAddress=0;
}
void CEngineComponent::SetGlobalComponentPointer(void** address)
{
    GlobalPointerAddress=address;
}
CCharString CEngineComponent::GetConsoleEnableFunctionName()
{
    return CCharString("");
}

void CEngineComponent::FrameStart(const CEngineComponentParams&) {  }

void CEngineComponent::FrameEnd(const CEngineComponentParams&) {  }

bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) { return false; }

void CEngineComponent::Render(const CEngineComponentRenderParams&) {  }

void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) {  }

void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) {  }

void CEngineComponent::WorldFrameUpdate(bool) {  }

void CEngineComponent::PausedWorldFrameUpdate() {  }

void CEngineComponent::OnReleaseDefaultPoolResources() {  }

bool CEngineComponent::OnRestoreDefaultPoolResources() { return true; }
