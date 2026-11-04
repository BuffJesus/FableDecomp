#include "fable_render_state_manager.h"
#include "fable_render_state_defaults.h"
void CRenderStateManager::SetDefaultRenderStates(){
    SetRenderState(1,DeviceState.DitherEnable);
    SetRenderState(1,DeviceState.ZBufferEnable);
    SetRenderState(1,DeviceState.ZBufferWriteEnable);
    SetRenderState(FableRenderDefaultValue_01396e04,DeviceState.ZBufferFunc);
    SetRenderState(FableRenderDefaultValue_01396fa4,DeviceState.PolyFillMode);
    SetRenderState(FableRenderDefaultValue_01396fa0,DeviceState.PolyShadeMode);
    SetRenderState(0,DeviceState.AlphaTestEnable);
    SetRenderState(128,DeviceState.AlphaRef);
    SetRenderState(FableRenderDefaultValue_01396ddc,DeviceState.AlphaTestFunc);
    SetRenderState(FableRenderDefaultValue_01396f6c,DeviceState.SourceAlphaBlendMode);
    SetRenderState(FableRenderDefaultValue_01396f68,DeviceState.DestAlphaBlendMode);
    SetRenderState(FableRenderDefaultValue_01396fb0,DeviceState.PolyCullMode);
    SetRenderState(0,DeviceState.AlphaBlendEnable);
    SetRenderState(0,DeviceState.SpecularEnable);
    SetRenderState(0xff000000UL,DeviceState.TextureFactor);
    SetRenderState(0xffffffffUL,DeviceState.TextureFactor);
    SetRenderState(1,DeviceState.ClippingEnable);
    SetRenderState(0,DeviceState.FogEnable);
    SetRenderState(0x00ffffffUL,DeviceState.FogColour);
    SetRenderState(FableRenderDefaultValue_01396f58,DeviceState.FogMode);
    SetRenderState(FableRenderDefaultValue_01396f58,DeviceState.FogVertexMode);
    SetRenderState(0,DeviceState.FogDensity);
    SetRenderState(0x3dcccccdUL,DeviceState.FogStart); // float bits: 0.1
    SetRenderState(0x3f666666UL,DeviceState.FogEnd); // float bits: 0.9
    SetRenderState(FableRenderDefaultValue_01396fbc,DeviceState.VertexBlendMode);
    SetRenderState(15,DeviceState.ColourWriteEnable);
    SetRenderState(0,DeviceState.DepthOffset);
    SetRenderState(0,DeviceState.DepthOffsetSlope);
    SetRenderState(0,DeviceState.PointSpriteEnable);
    SetRenderState(0,DeviceState.PointScaleEnable);
    SetRenderState(0x42800000UL,DeviceState.PointSize); // float bits: 64.0
    SetRenderState(0x3f800000UL,DeviceState.PointSizeMin); // float bits: 1.0
    SetRenderState(0x42800000UL,DeviceState.PointSizeMax); // float bits: 64.0
    SetRenderState(0x3f800000UL,DeviceState.PointScaleConstantFactor); // float bits: 1.0
    SetRenderState(0,DeviceState.PointScaleDistanceFactor);
    SetRenderState(0,DeviceState.PointScaleDistanceSqrFactor);
    SetRenderState(FableRenderDefaultValue_01396ed0,DeviceState.BlendOpMode);
    SetRenderState(0,DeviceState.LightingEnable);
    SetRenderState(0,DeviceState.NormalizeNormals);
    for(long stage=0;stage<8;++stage){
    SetRenderState(FableRenderDefaultValue_01396e70,DeviceState.TextureOperation[stage]);
    SetRenderState(FableRenderDefaultValue_01396e70,DeviceState.TextureAlphaOperation[stage]);
    SetRenderState(FableRenderDefaultValue_01396ef0,DeviceState.TextureArgument1[stage]);
    SetRenderState(FableRenderDefaultValue_01396eec,DeviceState.TextureArgument2[stage]);
    SetRenderState(FableRenderDefaultValue_01396ef0,DeviceState.TextureAlphaArgument1[stage]);
    SetRenderState(FableRenderDefaultValue_01396eec,DeviceState.TextureAlphaArgument2[stage]);
    SetRenderState(FableRenderDefaultValue_01396e5c,DeviceState.TextureFilterModeMin[stage]);
    SetRenderState(FableRenderDefaultValue_01396e5c,DeviceState.TextureFilterModeMag[stage]);
    SetRenderState(FableRenderDefaultValue_01396dd0,DeviceState.TextureUVModeU[stage]);
    SetRenderState(FableRenderDefaultValue_01396dd0,DeviceState.TextureUVModeV[stage]);
    SetRenderState(FableRenderDefaultValue_01396dd0,DeviceState.TextureUVModeW[stage]);
    SetRenderState(0,DeviceState.TextureMipmapLODBias[stage]);
    SetRenderState(D3DTextureMipMapModeTable[1],DeviceState.TextureMipMapMode[stage]);
    SetRenderState(stage,DeviceState.TextureCoordIndex[stage]);
    SetRenderState(FableRenderDefaultValue_01396fcc,DeviceState.TextureTransformMode[stage]);
    SetRenderState(0,DeviceState.TextureBumpEnvMat00[stage]);
    SetRenderState(0,DeviceState.TextureBumpEnvMat10[stage]);
    SetRenderState(0,DeviceState.TextureBumpEnvMat01[stage]);
    SetRenderState(0,DeviceState.TextureBumpEnvMat11[stage]);
    SetRenderState(0,DeviceState.TextureCoordWrapMode[stage]);
    SetRenderState(1,DeviceState.TextureMaxAnisotropy[stage]);
    SetRenderState(0,DeviceState.TextureBorderColour[stage]);
    }
    SetRenderState(FableRenderDefaultValue_01396e7c,DeviceState.TextureOperation[0]);
    SetRenderState(FableRenderDefaultValue_01396e7c,DeviceState.TextureAlphaOperation[0]);
    SetRenderState(0,DeviceState.StencilEnable);
    SetRenderState(FableRenderDefaultValue_01396f38,DeviceState.StencilOpFail);
    SetRenderState(FableRenderDefaultValue_01396f38,DeviceState.StencilOpZFail);
    SetRenderState(FableRenderDefaultValue_01396f38,DeviceState.StencilOpPass);
    SetRenderState(FableRenderDefaultValue_01396e34,DeviceState.StencilFunc);
    SetRenderState(0,DeviceState.StencilRef);
    SetRenderState(0xffffffffUL,DeviceState.StencilMask);
    SetRenderState(0xffffffffUL,DeviceState.StencilWriteMask);
    SetRenderState(FableRenderDefaultValue_01396ff4,DeviceState.AmbientMaterialColourSource);
    SetRenderState(FableRenderDefaultValue_01396ff4,DeviceState.DiffuseMaterialColourSource);
    SetRenderState(FableRenderDefaultValue_01396ff4,DeviceState.SpecularMaterialColourSource);
}
