#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "fable_engine_component.h"
#include "fable_engine_layer_renderer.h"
inline void CRenderManagerCore::AttachD3DTextureToStage(long stage,IDirect3DBaseTexture9* texture)
{
    if(CurrentlySelectedTextures[stage]!=texture) {
        CurrentlySelectedTextures[stage]=texture;
        typedef HRESULT (__stdcall* Call)(IDirect3DDevice9*,unsigned long,IDirect3DBaseTexture9*);
        void** table=*reinterpret_cast<void***>(PD3DDevice);
        reinterpret_cast<Call>(table[65])(PD3DDevice,stage,texture);
    }
    if(TextureStagesInUse<stage+1)TextureStagesInUse=stage+1;
}
void CEngine::Uninitialise()
{
    using namespace NEngineInternal;
    if(IsInitialised) {
        GPDisplayManager->RemoveDeviceResetCallback(this);
        typedef HRESULT (__stdcall* SetStream)(IDirect3DDevice9*,unsigned int,IDirect3DVertexBuffer9*,unsigned int,unsigned int);
        IDirect3DDevice9* device=GPRenderManager->PD3DDevice;
        reinterpret_cast<SetStream>((*reinterpret_cast<void***>(device))[100])(device,0,0,0,0);
        GPRenderManager->PSelectedIndexBuffer=0;
        typedef HRESULT (__stdcall* SetIndices)(IDirect3DDevice9*,IDirect3DIndexBuffer9*);
        device=GPRenderManager->PD3DDevice;
        reinterpret_cast<SetIndices>((*reinterpret_cast<void***>(device))[104])(device,0);
        GPRenderManager->AttachD3DTextureToStage(0,0);
        GPRenderManager->AttachD3DTextureToStage(1,0);
        GPRenderManager->AttachD3DTextureToStage(2,0);
        GPRenderManager->AttachD3DTextureToStage(3,0);
        while(EngineComponents.size()) {
            CEngineComponent* component=EngineComponents[EngineComponents.size()-1];
            EngineComponents.pop_back();
            delete component;
        }
        GPPrimitiveRendererSharedData=0;
        for(unsigned long i=0;i<ScratchZBuffersD24S8.size();++i)delete ScratchZBuffersD24S8[i];
        ScratchZBuffersD24S8.clear();
        for(CEngineLayerRenderer** layer=RenderLayers.begin();layer!=RenderLayers.end();++layer) {
            delete *layer;
            *layer=0;
        }
        RenderLayers.clear();
        GPVideoOptions=0;
        CInitBaseClass::Uninitialise();
        IsInitialised=false;
    }
}
