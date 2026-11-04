// Retail 00AB83A0; state capture and call ordering qualified by the native prepare gate.
#include "fable_font_instances.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"
#include "fable_render_state_defaults.h"

// Descriptive identities for one more native lookup entry, not original symbol names.
unsigned long FableRenderDefaultValue_01396dc8=1;

void CStaticFontBank::PrepareToRender(bool setupVertexFormat) const
{
    CRenderManagerCore* render=&GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    render->SetRenderState(1,render->DeviceState.AlphaBlendEnable);
    render->SetRenderState(FableRenderDefaultValue_01396f78,render->DeviceState.SourceAlphaBlendMode);
    render->SetRenderState(FableRenderDefaultValue_01396f7c,render->DeviceState.DestAlphaBlendMode);
    render->SetRenderState(1,render->DeviceState.AlphaTestEnable);
    render->SetRenderState(8,render->DeviceState.AlphaRef);
    render->SetRenderState(CRenderStateManager::D3DAlphaFuncModeTable[ALPHA_TEST_GREATER_EQUAL],render->DeviceState.AlphaTestFunc);
    render->SetRenderState(FableRenderDefaultValue_01396fa4,render->DeviceState.PolyFillMode);
    render->SetRenderState(FableRenderDefaultValue_01396fb0,render->DeviceState.PolyCullMode);
    render->SetRenderState(1,render->DeviceState.ZBufferEnable);
    render->SetRenderState(0,render->DeviceState.FogEnable);
    render->SetRenderState(1,render->DeviceState.ClippingEnable);
    render->SetRenderState(FableRenderDefaultValue_01396e5c,render->DeviceState.TextureFilterModeMin[0]);
    render->SetRenderState(FableRenderDefaultValue_01396e5c,render->DeviceState.TextureFilterModeMag[0]);
    render->SetRenderState(CRenderStateManager::D3DTextureMipMapModeTable[0],render->DeviceState.TextureMipMapMode[0]);
    render->SetRenderState(FableRenderDefaultValue_01396dc8,render->DeviceState.TextureUVModeU[0]);
    render->SetRenderState(FableRenderDefaultValue_01396dc8,render->DeviceState.TextureUVModeV[0]);
    render->SetRenderState(FableRenderDefaultValue_01396fcc,render->DeviceState.TextureTransformMode[0]);
    if(setupVertexFormat){
        render->SetVertexFormat(0x144);
        render->SetRenderState(0,render->DeviceState.TextureCoordIndex[0]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_MODULATE],render->DeviceState.TextureOperation[0]);
        render->SetRenderState(FableRenderDefaultValue_01396ef0,render->DeviceState.TextureArgument1[0]);
        render->SetRenderState(FableRenderDefaultValue_01396eec,render->DeviceState.TextureArgument2[0]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_MODULATE],render->DeviceState.TextureAlphaOperation[0]);
        render->SetRenderState(FableRenderDefaultValue_01396ef0,render->DeviceState.TextureAlphaArgument1[0]);
        render->SetRenderState(FableRenderDefaultValue_01396eec,render->DeviceState.TextureAlphaArgument2[0]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_DISABLE],render->DeviceState.TextureOperation[1]);
        render->SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_DISABLE],render->DeviceState.TextureAlphaOperation[1]);
    }
}
