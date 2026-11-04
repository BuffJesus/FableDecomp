#include "fable_state_block_function.h"
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_gfuninitialise.h"
#include "fable_render_state_defaults.h"
void CStateBlockFunctionSold::Apply()
{
 CRenderManager& manager=GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
 manager.SetRenderState(FableRenderDefaultValue_01396f6c,manager.DeviceState.SourceAlphaBlendMode);
 manager.SetRenderState(FableRenderDefaultValue_01396f68,manager.DeviceState.DestAlphaBlendMode);
 manager.SetRenderState(1,manager.DeviceState.ZBufferEnable);
 manager.SetRenderState(0,manager.DeviceState.AlphaBlendEnable);
 manager.SetRenderState(0,manager.DeviceState.AlphaTestEnable);
 manager.SetRenderState(1,manager.DeviceState.ZBufferWriteEnable);
 manager.SetRenderState(FableRenderDefaultValue_01396e04,manager.DeviceState.ZBufferFunc);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureAlphaArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureAlphaArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureAlphaOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureAlphaOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e5c,manager.DeviceState.TextureFilterModeMin[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e5c,manager.DeviceState.TextureFilterModeMag[0]);
}
void CStateBlockFunctionAlphaAdditive::Apply()
{
 CRenderManager& manager=GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
 manager.SetRenderState(FableRenderDefaultValue_01396f6c,manager.DeviceState.SourceAlphaBlendMode);
 manager.SetRenderState(FableRenderDefaultValue_01396f6c,manager.DeviceState.DestAlphaBlendMode);
 manager.SetRenderState(1,manager.DeviceState.ZBufferEnable);
 manager.SetRenderState(0,manager.DeviceState.ZBufferWriteEnable);
 manager.SetRenderState(1,manager.DeviceState.AlphaBlendEnable);
 manager.SetRenderState(0,manager.DeviceState.AlphaTestEnable);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureAlphaArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureAlphaArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureAlphaOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureAlphaOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e5c,manager.DeviceState.TextureFilterModeMin[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e5c,manager.DeviceState.TextureFilterModeMag[0]);
}
void CStateBlockFunctionAlphaMultiply::Apply()
{
 CRenderManager& manager=GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
 manager.SetRenderState(FableRenderDefaultValue_01396f78,manager.DeviceState.SourceAlphaBlendMode);
 manager.SetRenderState(FableRenderDefaultValue_01396f7c,manager.DeviceState.DestAlphaBlendMode);
 manager.SetRenderState(1,manager.DeviceState.ZBufferEnable);
 manager.SetRenderState(0,manager.DeviceState.ZBufferWriteEnable);
 manager.SetRenderState(1,manager.DeviceState.AlphaBlendEnable);
 manager.SetRenderState(0,manager.DeviceState.AlphaTestEnable);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureAlphaArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureAlphaArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureAlphaOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureAlphaOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e5c,manager.DeviceState.TextureFilterModeMin[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e5c,manager.DeviceState.TextureFilterModeMag[0]);
}
void CStateBlockFunctionAlphaBoolean::Apply()
{
 CRenderManager& manager=GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
 manager.SetRenderState(FableRenderDefaultValue_01396f78,manager.DeviceState.SourceAlphaBlendMode);
 manager.SetRenderState(FableRenderDefaultValue_01396f7c,manager.DeviceState.DestAlphaBlendMode);
 manager.SetRenderState(1,manager.DeviceState.ZBufferEnable);
 manager.SetRenderState(1,manager.DeviceState.ZBufferWriteEnable);
 manager.SetRenderState(1,manager.DeviceState.AlphaBlendEnable);
 manager.SetRenderState(0,manager.DeviceState.AlphaTestEnable);
 manager.SetRenderState(FableRenderDefaultValue_01396de8,manager.DeviceState.AlphaTestFunc);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureAlphaArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureAlphaArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureAlphaOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureAlphaOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e58,manager.DeviceState.TextureFilterModeMin[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e58,manager.DeviceState.TextureFilterModeMag[0]);
}
void CStateBlockFunctionAlphaText::Apply()
{
 CRenderManager& manager=GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
 manager.SetRenderState(FableRenderDefaultValue_01396f78,manager.DeviceState.SourceAlphaBlendMode);
 manager.SetRenderState(FableRenderDefaultValue_01396f7c,manager.DeviceState.DestAlphaBlendMode);
 manager.SetRenderState(1,manager.DeviceState.ZBufferEnable);
 manager.SetRenderState(1,manager.DeviceState.ZBufferWriteEnable);
 manager.SetRenderState(1,manager.DeviceState.AlphaBlendEnable);
 manager.SetRenderState(0,manager.DeviceState.AlphaTestEnable);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396ef0,manager.DeviceState.TextureAlphaArgument1[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396eec,manager.DeviceState.TextureAlphaArgument2[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e7c,manager.DeviceState.TextureAlphaOperation[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e70,manager.DeviceState.TextureAlphaOperation[1]);
 manager.SetRenderState(FableRenderDefaultValue_01396e58,manager.DeviceState.TextureFilterModeMin[0]);
 manager.SetRenderState(FableRenderDefaultValue_01396e58,manager.DeviceState.TextureFilterModeMag[0]);
}
