#include "engine/CEngineShadowRenderer.h"
#include "fable_display_viewport.h"
#include "fable_engine_internal.h"
bool CEngineShadowRenderer::InitialiseTextures()
{
    using namespace NEngineInternal;
    UninitialiseTextures();
    RenderingTechnique = SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS;
    if(UserVideoOptionShadowBufferSize > 0) {
        CPixelFormat depthFormat((D3DFORMAT)80);
        if(GPDisplayManager->IsValidTextureFormat(depthFormat)) {
            CPixelFormat colourFormat;
            GPDisplayManager->GetBestOpaqueTextureFormat(16,colourFormat,true);
            if(GPDisplayManager->IsValidRenderTargetFormat(colourFormat,depthFormat)) {
                long size=UserVideoOptionShadowBufferSize;
                if(ShadowBufferTexture.Initialise(C2DExtentsI(size,size),1,depthFormat,2,SURFACE_POOL_DEFAULT_LOCATION,false,true)) {
                    if(ShadowBufferColourTexture.Initialise(C2DExtentsI(size,size),1,colourFormat,1,SURFACE_POOL_DEFAULT_LOCATION,false,true)) {
                        RenderingTechnique=SHADOW_RENDERING_TECHNIQUE_DEPTH_BUFFER;
                        ShadowBufferDepthSurface=ShadowBufferTexture.GetSurfaceLevel(0);
                        ShadowBufferColourSurface=ShadowBufferColourTexture.GetSurfaceLevel(0);
                    } else ShadowBufferTexture.Uninitialise();
                }
            }
        }
        if(RenderingTechnique==SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS && GPDisplayManager->GetPixelShaderVersion()>=1400) {
            D3DFORMAT formats[8]={(D3DFORMAT)21,(D3DFORMAT)32};
            CPixelFormat colourFormat;
            for(long i=0;i<8;++i) {
                CPixelFormat candidate(formats[i]);
                if(GPDisplayManager->IsValidTextureFormat(candidate) && GPDisplayManager->IsValidRenderTargetFormat(candidate,depthFormat)) {
                    colourFormat=candidate;
                    break;
                }
            }
            if(colourFormat.FormatIndex!=-1) {
                long size=UserVideoOptionShadowBufferSize;
                if(ShadowBufferTexture.Initialise(C2DExtentsI(size,size),1,colourFormat,1,SURFACE_POOL_DEFAULT_LOCATION,false,true)) {
                    if(ShadowBufferDepthSurface.InitialiseAsDepthBuffer(C2DExtentsI(size,size),depthFormat,SURFACE_MULTISAMPLE_NONE,true)) {
                        RenderingTechnique=SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER;
                        ShadowBufferColourTexture=ShadowBufferTexture;
                        ShadowBufferColourSurface=ShadowBufferTexture.GetSurfaceLevel(0);
                        BuildDepthPackingLookupTexture();
                    } else ShadowBufferTexture.Uninitialise();
                }
            }
        }
    }
    if(RenderingTechnique==SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS) {
        RenderTarget.Uninitialise();
        ShadowBufferColourSurface.Uninitialise();
        ShadowBufferDepthSurface.Uninitialise();
        ShadowBufferColourTexture.Uninitialise();
        ShadowBufferTexture.Uninitialise();
        TexturesAllocated=false;
        return UserVideoOptionShadowBufferSize==0;
    }
    RenderTarget.AttachTarget(ShadowBufferColourSurface);
    RenderTarget.AttachZBuffer(ShadowBufferDepthSurface);
    TexturesAllocated=true;
    return true;
}
