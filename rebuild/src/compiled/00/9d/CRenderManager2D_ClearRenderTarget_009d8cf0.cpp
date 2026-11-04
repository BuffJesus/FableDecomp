#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "fable_vertex_shader.h"
#include "fable_render2d_vertex.h"
#include "fable_render_state_defaults.h"
// Retail 009D8CF0: complete alternate clear path, including captured state
// restoration. Engine dependencies retain their own native providers.
void CRenderManager2D::ClearRenderTarget(const CRGBColour& colour,unsigned long flags)
{
 CShaderRenderManager& shader=FableShaderRenderManager_013bc470;
 if(!flags)flags=7;
 if(!AlternateRenderTargetClearEnabled){DisplayManager->ClearRenderTarget(colour,flags);return;}
 {
  bool wasVertex=shader.VertexShadersEnabled;
  C2DBoxI previous,full;
  C2DExtentsI dimensions;
  CTVertexRHWColSpecTex1 vertices[4];
  unsigned short indices[6];
  unsigned long packedColour;
  long previousLayout;
  CTVertexRHWColSpecTex1* vertex;long remaining;
  if(shader.VertexShadersEnabled!=true)shader.EnableVertexShaders();
  bool wasPixel=shader.PixelShadersEnabled;
  if(shader.PixelShadersEnabled)shader.DisablePixelShaders();
  CRenderStateManager* state=FableCurrentRenderStateManager_013cb508;
  long& stackSize=state->StateStackSize;
  state->StateStack[stackSize].State=&state->BookmarkDummyState;
  state->CurrentBookmarkMask<<=1;
  ++stackSize;
  SetRenderState(0,DeviceState.FogEnable);
  SetRenderState(0,DeviceState.SpecularEnable);
  SetRenderState(0,DeviceState.AlphaBlendEnable);
  SetRenderState(0,DeviceState.AlphaTestEnable);
  if(flags&4)SetRenderState(15,DeviceState.ColourWriteEnable);
  else SetRenderState(0,DeviceState.ColourWriteEnable);
  if(flags&1)
  {
   SetRenderState(1,DeviceState.ZBufferEnable);
   SetRenderState(1,DeviceState.ZBufferWriteEnable);
   SetRenderState(FableRenderDefaultValue_01396e14,DeviceState.ZBufferFunc);
  }
  else SetRenderState(0,DeviceState.ZBufferEnable);
  if(flags&2)
  {
   SetRenderState(1,DeviceState.StencilEnable);
   SetRenderState(FableRenderDefaultValue_01396e34,DeviceState.StencilFunc);
   SetRenderState(FableRenderDefaultValue_01396f40,DeviceState.StencilOpPass);
   SetRenderState(FableRenderDefaultValue_01396f40,DeviceState.StencilOpFail);
   SetRenderState(FableRenderDefaultValue_01396f40,DeviceState.StencilOpZFail);
   SetRenderState(255,DeviceState.StencilMask);
   SetRenderState(0,DeviceState.StencilRef);
  }
  else SetRenderState(0,DeviceState.StencilEnable);
  previous=DisplayManager->ViewportBox;
  dimensions=DisplayManager->GetRenderTargetDimensions();
  full.left=0;full.top=0;full.right=dimensions.X;full.bottom=dimensions.Y;
  DisplayManager->SetIntegerViewportEndpoint(full);
  packedColour=colour.GetD3DCOLOR();
  vertices[0].X=-1.0f;vertices[0].Y=1.0f;vertices[0].Z=0.999999940395355224609375f;
  vertices[1].X=1.0f;vertices[1].Y=1.0f;vertices[1].Z=0.999999940395355224609375f;
  vertices[2].X=1.0f;vertices[2].Y=-1.0f;vertices[2].Z=0.999999940395355224609375f;
  vertices[3].X=-1.0f;vertices[3].Y=-1.0f;vertices[3].Z=0.999999940395355224609375f;
  vertex=vertices;remaining=4;
  do
  {
   vertex->U1=0;vertex->V1=0;
   vertex->DiffuseColour=packedColour;vertex->RHW=1;
   ++vertex;
  }while(--remaining);
  indices[0]=0;indices[1]=1;indices[2]=2;indices[3]=0;indices[4]=2;indices[5]=3;
  shader.RegisterVertexShaderConstantLayout(0,&Fable2DShaderConstantLayout_013ca8a8);
  previousLayout=shader.CurrentVSConstantLayoutIndex;
  shader.SetVertexShaderConstantLayout(0);
  shader.ApplyVertexShader(VertexShader2D,false);
  if(shader.updateFlags3D8&4)
  {
   const CVertexShaderConstantLayout& layout=*shader.CurrentVSConstantLayout;
   const CArrayVector<CVertexShaderConstant>& presets=layout.ReservedPresets();
   const CVertexShaderConstant* data=presets.begin();
   if(presets.size()==0)data=reinterpret_cast<const CVertexShaderConstant*>(&presets);
   shader.SetVSConstants(layout.Reserved.Offset,data,presets.size());
  }
  if(shader.updateFlags3D8&1)shader.UpdatePixelShader();
  if(shader.updateFlags3D8&2)shader.UpdatePixelShaderConstants();
  if(shader.updateFlags3D8&4)shader.UpdateAmbient();
  if(shader.updateFlags3D8&0x20000)shader.UpdateFogColour();
  if(shader.updateFlags3D8&0x400)shader.UpdateLightGlobals();
  if(!shader.WorldMatrixIsIdentity)shader.ResetWorldTransform();
  if(shader.updateFlags3D8&0x80)shader.UpdateCombinedProjectionTransform();
  SetRenderState(FableRenderDefaultValue_01396e74,DeviceState.TextureOperation[0]);
  SetRenderState(FableRenderDefaultValue_01396e70,DeviceState.TextureOperation[1]);
  SetRenderState(FableRenderDefaultValue_01396e74,DeviceState.TextureAlphaOperation[0]);
  SetRenderState(FableRenderDefaultValue_01396e70,DeviceState.TextureAlphaOperation[1]);
  SetRenderState(FableRenderDefaultValue_01396eec,DeviceState.TextureArgument1[0]);
  SetRenderState(FableRenderDefaultValue_01396eec,DeviceState.TextureAlphaArgument1[0]);
  DrawAsIndexedTris(vertices,32,4,indices,2);
  DisplayManager->SetIntegerViewportEndpoint(previous);
  shader.SetVertexShaderConstantLayout(previousLayout);
  FableCurrentRenderStateManager_013cb508->RestoreCaptureBlock();
  if(wasVertex!=shader.VertexShadersEnabled)
  {
   if(wasVertex)shader.EnableVertexShaders();else shader.DisableVertexShaders();
  }
  if(wasPixel!=shader.PixelShadersEnabled)
  {
   if(wasPixel)shader.EnablePixelShaders();else shader.DisablePixelShaders();
  }
  return;
 }
}
