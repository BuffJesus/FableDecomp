#include "fable_render_state_manager.h"
void CRenderStateManager::SetupRenderState(CStateInfo& state,D3DRENDERSTATETYPE type){
 state.CurrentState=0;state.DesiredState=0;state.DirtyListFlag=false;
 state.StateType=1;state.StateParam=type;
 AllStatesList[AllStatesListSize++]=&state;
}
void CRenderStateManager::SetupTextureStageState(CStateInfo& state,D3DTEXTURESTAGESTATETYPE type,long stage){
 state.CurrentState=0;state.DesiredState=0;state.DirtyListFlag=false;
 state.StateType=2;state.StateParam=type;
 state.TextureStage=static_cast<unsigned char>(stage);
 AllStatesList[AllStatesListSize++]=&state;
}
void CRenderStateManager::SetupSamplerStageState(CStateInfo& state,D3DSAMPLERSTATETYPE type,long stage){
 state.CurrentState=0;state.DesiredState=0;state.DirtyListFlag=false;
 state.StateType=3;state.StateParam=type;
 state.TextureStage=static_cast<unsigned char>(stage);
 AllStatesList[AllStatesListSize++]=&state;
}

void CRenderStateManager::SetupRenderStates(){
 D3DCAPS9 caps;
 static_cast<IDirect3DDevice9*>(PD3DDevice)->GetDeviceCaps(&caps);
 SetupRenderState(DeviceState.PolyShadeMode,D3DRS_SHADEMODE);
 SetupRenderState(DeviceState.PolyFillMode,D3DRS_FILLMODE);
 SetupRenderState(DeviceState.ColourWriteEnable,D3DRS_COLORWRITEENABLE);
 SetupRenderState(DeviceState.ZBufferWriteEnable,D3DRS_ZWRITEENABLE);
 SetupRenderState(DeviceState.ZBufferFunc,D3DRS_ZFUNC);
 SetupRenderState(DeviceState.ZBufferEnable,D3DRS_ZENABLE);
 SetupRenderState(DeviceState.PolyCullMode,D3DRS_CULLMODE);
 SetupRenderState(DeviceState.SourceAlphaBlendMode,D3DRS_SRCBLEND);
 SetupRenderState(DeviceState.DestAlphaBlendMode,D3DRS_DESTBLEND);
 SetupRenderState(DeviceState.AlphaBlendEnable,D3DRS_ALPHABLENDENABLE);
 SetupRenderState(DeviceState.AlphaTestEnable,D3DRS_ALPHATESTENABLE);
 SetupRenderState(DeviceState.AlphaTestFunc,D3DRS_ALPHAFUNC);
 SetupRenderState(DeviceState.AlphaRef,D3DRS_ALPHAREF);
 SetupRenderState(DeviceState.VertexBlendMode,D3DRS_VERTEXBLEND);
 SetupRenderState(DeviceState.FogEnable,D3DRS_FOGENABLE);
 SetupRenderState(DeviceState.DitherEnable,D3DRS_DITHERENABLE);
 SetupRenderState(DeviceState.FogStart,D3DRS_FOGSTART);
 SetupRenderState(DeviceState.FogEnd,D3DRS_FOGEND);
 SetupRenderState(DeviceState.FogDensity,D3DRS_FOGDENSITY);
 SetupRenderState(DeviceState.FogColour,D3DRS_FOGCOLOR);
 SetupRenderState(DeviceState.FogMode,D3DRS_FOGTABLEMODE);
 SetupRenderState(DeviceState.FogVertexMode,D3DRS_FOGVERTEXMODE);
 SetupRenderState(DeviceState.TextureFactor,D3DRS_TEXTUREFACTOR);
 SetupRenderState(DeviceState.SpecularEnable,D3DRS_SPECULARENABLE);
 SetupRenderState(DeviceState.LightingEnable,D3DRS_LIGHTING);
 SetupRenderState(DeviceState.NormalizeNormals,D3DRS_NORMALIZENORMALS);
 SetupRenderState(DeviceState.AmbientMaterialColourSource,D3DRS_AMBIENTMATERIALSOURCE);
 SetupRenderState(DeviceState.DiffuseMaterialColourSource,D3DRS_DIFFUSEMATERIALSOURCE);
 SetupRenderState(DeviceState.SpecularMaterialColourSource,D3DRS_SPECULARMATERIALSOURCE);
 SetupRenderState(DeviceState.PointSpriteEnable,D3DRS_POINTSPRITEENABLE);
 SetupRenderState(DeviceState.PointScaleEnable,D3DRS_POINTSCALEENABLE);
 SetupRenderState(DeviceState.PointSize,D3DRS_POINTSIZE);
 SetupRenderState(DeviceState.PointSizeMin,D3DRS_POINTSIZE_MIN);
 SetupRenderState(DeviceState.PointSizeMax,D3DRS_POINTSIZE_MAX);
 SetupRenderState(DeviceState.PointScaleConstantFactor,D3DRS_POINTSCALE_A);
 SetupRenderState(DeviceState.PointScaleDistanceFactor,D3DRS_POINTSCALE_B);
 SetupRenderState(DeviceState.PointScaleDistanceSqrFactor,D3DRS_POINTSCALE_C);
 SetupRenderState(DeviceState.BlendOpMode,D3DRS_BLENDOP);
 SetupRenderState(DeviceState.StencilEnable,D3DRS_STENCILENABLE);
 SetupRenderState(DeviceState.StencilOpFail,D3DRS_STENCILFAIL);
 SetupRenderState(DeviceState.StencilOpZFail,D3DRS_STENCILZFAIL);
 SetupRenderState(DeviceState.StencilOpPass,D3DRS_STENCILPASS);
 SetupRenderState(DeviceState.StencilFunc,D3DRS_STENCILFUNC);
 SetupRenderState(DeviceState.StencilRef,D3DRS_STENCILREF);
 SetupRenderState(DeviceState.StencilMask,D3DRS_STENCILMASK);
 SetupRenderState(DeviceState.StencilWriteMask,D3DRS_STENCILWRITEMASK);
 SetupRenderState(DeviceState.TextureCoordWrapMode[0],FableRenderWrapStateTypes_01396da8[0]);
 SetupRenderState(DeviceState.TextureCoordWrapMode[1],FableRenderWrapStateTypes_01396da8[1]);
 SetupRenderState(DeviceState.TextureCoordWrapMode[2],FableRenderWrapStateTypes_01396da8[2]);
 SetupRenderState(DeviceState.TextureCoordWrapMode[3],FableRenderWrapStateTypes_01396da8[3]);
 SetupRenderState(DeviceState.TextureCoordWrapMode[4],FableRenderWrapStateTypes_01396da8[4]);
 SetupRenderState(DeviceState.TextureCoordWrapMode[5],FableRenderWrapStateTypes_01396da8[5]);
 SetupRenderState(DeviceState.TextureCoordWrapMode[6],FableRenderWrapStateTypes_01396da8[6]);
 SetupRenderState(DeviceState.TextureCoordWrapMode[7],FableRenderWrapStateTypes_01396da8[7]);
 SetupRenderState(DeviceState.ClippingEnable,D3DRS_CLIPPING);
 if(caps.RasterCaps & 0x04000000)
 SetupRenderState(DeviceState.DepthOffset,D3DRS_DEPTHBIAS);
 if(caps.RasterCaps & 0x02000000)
 SetupRenderState(DeviceState.DepthOffsetSlope,D3DRS_SLOPESCALEDEPTHBIAS);
 for(long stage=0;stage<8;++stage){
 SetupSamplerStageState(DeviceState.TextureMipMapMode[stage],D3DSAMP_MIPFILTER,stage);
 SetupSamplerStageState(DeviceState.TextureUVModeU[stage],D3DSAMP_ADDRESSU,stage);
 SetupSamplerStageState(DeviceState.TextureUVModeV[stage],D3DSAMP_ADDRESSV,stage);
 SetupSamplerStageState(DeviceState.TextureUVModeW[stage],D3DSAMP_ADDRESSW,stage);
 SetupSamplerStageState(DeviceState.TextureFilterModeMag[stage],D3DSAMP_MAGFILTER,stage);
 SetupSamplerStageState(DeviceState.TextureFilterModeMin[stage],D3DSAMP_MINFILTER,stage);
  if(caps.RasterCaps & 0x2000)
 SetupSamplerStageState(DeviceState.TextureMipmapLODBias[stage],D3DSAMP_MIPMAPLODBIAS,stage);
 SetupSamplerStageState(DeviceState.TextureBorderColour[stage],D3DSAMP_BORDERCOLOR,stage);
 SetupSamplerStageState(DeviceState.TextureMaxAnisotropy[stage],D3DSAMP_MAXANISOTROPY,stage);
 SetupTextureStageState(DeviceState.TextureOperation[stage],D3DTSS_COLOROP,stage);
 SetupTextureStageState(DeviceState.TextureAlphaOperation[stage],D3DTSS_ALPHAOP,stage);
 SetupTextureStageState(DeviceState.TextureArgument1[stage],D3DTSS_COLORARG1,stage);
 SetupTextureStageState(DeviceState.TextureArgument2[stage],D3DTSS_COLORARG2,stage);
 SetupTextureStageState(DeviceState.TextureAlphaArgument1[stage],D3DTSS_ALPHAARG1,stage);
 SetupTextureStageState(DeviceState.TextureAlphaArgument2[stage],D3DTSS_ALPHAARG2,stage);
 SetupTextureStageState(DeviceState.TextureCoordIndex[stage],D3DTSS_TEXCOORDINDEX,stage);
  if((InitFlags & 2)==0)
 SetupTextureStageState(DeviceState.TextureTransformMode[stage],D3DTSS_TEXTURETRANSFORMFLAGS,stage);
 SetupTextureStageState(DeviceState.TextureBumpEnvMat00[stage],D3DTSS_BUMPENVMAT00,stage);
 SetupTextureStageState(DeviceState.TextureBumpEnvMat10[stage],D3DTSS_BUMPENVMAT10,stage);
 SetupTextureStageState(DeviceState.TextureBumpEnvMat01[stage],D3DTSS_BUMPENVMAT01,stage);
 SetupTextureStageState(DeviceState.TextureBumpEnvMat11[stage],D3DTSS_BUMPENVMAT11,stage);
 }
}
