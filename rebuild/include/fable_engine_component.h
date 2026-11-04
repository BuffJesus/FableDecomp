#pragma once
#include "rebuild_abi.h"
class CCharString;
class CEngineComponentParams;
class CEngineComponentLoadNewSceneParams;
class CEngineComponentRenderParams;
// Ego_r PDB: twelve-byte object, thirteen virtual entries in this order.
class CEngineComponent {
public:
    CEngineComponent();
    virtual ~CEngineComponent();
    virtual void FrameStart(const CEngineComponentParams&);
    virtual void FrameEnd(const CEngineComponentParams&);
    virtual bool LoadNewScene(const CEngineComponentLoadNewSceneParams&);
    virtual void Render(const CEngineComponentRenderParams&);
    virtual void PreLayerRender(const CEngineComponentRenderParams&);
    virtual void PostLayerRender(const CEngineComponentRenderParams&);
    virtual void WorldFrameUpdate(bool);
    virtual void PausedWorldFrameUpdate();
    virtual CCharString GetConsoleEnableFunctionName();
    virtual unsigned long PeekSceneFilterFlags() const = 0;
    virtual void OnReleaseDefaultPoolResources();
    virtual bool OnRestoreDefaultPoolResources();
    void SetGlobalComponentPointer(void** address);
    void** GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
};
FABLE_STATIC_ASSERT(sizeof(CEngineComponent)==12);
