#include "fable_display_engine.h"
#include "fable_engine_init.h"
#include "fable_iengine.h"
#include "fable_game.h"
#include "fable_world.h"
#include "fable_engine_manager.h"
#include "fable_definition_manager.h"
#include "fable_resource_directories.h"
#include "fable_progress_display.h"
#include "fable_gfuninitialise.h"
#include "fable_config_detection.h"
#include "engine/CConfigOptions.h"

inline CEngineInit::~CEngineInit() {}
template<> __forceinline CDefPointer<const CEngineDef>::~CDefPointer() {
    if(Object)static_cast<CDefPointeeBase*>(Object)->Unlock();
}

bool CDisplayEngine::InitEngine() {
    CEngineInit init;
    GFGetSystemManager();
    Engine3D=0;
    Engine3D=GFAllocEngine();
    if(Engine3D) {
        init.DefinitionManager=DefinitionManager;
        init.RenderManager=g_FableRenderManager2D_013B8384;
        init.Font=&Component->PeekDebugFont();
        init.MainGraphicBank=EngineGraphicBank;
        init.World=World;
        init.MeshBank=MeshBank;
        init.GraphicsResourcePath=NResourceDirectoryNames::GetGraphicBankDir();
        init.MeshResourcePath=NResourceDirectoryNames::GetMeshBankDir();
        init.ShaderResourcePath=NResourceDirectoryNames::GetShaderBankDir();
        init.EngineCacheResourcePath=NResourceDirectoryNames::GetEngineCacheDir();
        init.WriteableEngineCacheResourcePath=NResourceDirectoryNames::GetWriteableEngineCacheDir();
        init.MemoryInit=g_FableEngineMemoryInit_0137549C;
        init.StaticMapQuality=GStaticMapQuality;
        init.InitFlags=1|(GEnableMinimumStaticMapRebuild?2:0);
        if(CMainGameComponent::Get()->IsEditorActive())init.InitFlags|=4;
        else init.InitFlags|=GAllowDataGeneration?4:0;
        if(GConfigOptions.EnableStretchBlitEffects)init.InitFlags|=0x80;
        if(GConfigOptions.EnableDisplacementBuffer)init.InitFlags|=8;
        if(GConfigOptions.UseAlternateOutlinePixelShader)init.InitFlags|=0x40;
        if(GConfigOptions.UseAlternateSkyPixelShader)init.InitFlags|=0x100;
        init.InitFlags|=GBuildRetailStaticMaps?0x10:0;
        {CCharString text("Init Engine: Initialise");NProgressDisplay::DisplayProgress(text,-1,false,false);}
        if(Engine3D->Initialise(init)==true) {
            {CCharString text("Init Engine: AddEngine");NProgressDisplay::DisplayProgress(text,-1,false,false);}
            CEngineManager::Get().AddEngine(*Engine3D);
            {CCharString text("Init Engine: OnActivate");NProgressDisplay::DisplayProgress(text,-1,false,false);}
            Engine3D->OnActivate();
            CDefPointer<const CEngineDef> def;
            DefinitionManager->GetDef<CEngineDef>(CCharString("ENGINE"),1,&def);
            GamePrimitiveDefaultFadeRangeRatio=def->GamePrimitiveDefaultFadeRangeRatio;
            return true;
        }
    }
    return false;
}
