#include <new>
#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "fable_vertex_shader.h"
#include "fable_render_state_defaults.h"
#include "fable_2d_vector.h"
#include "fable_font_bank.h"
namespace _STL {
template<> CQuickDrawTriInfo* vector<CQuickDrawTriInfo>::erase(CQuickDrawTriInfo*,CQuickDrawTriInfo*);
template<> CTVertexRHWColSpecTex1* vector<CTVertexRHWColSpecTex1>::erase(CTVertexRHWColSpecTex1*,CTVertexRHWColSpecTex1*);
}
static __forceinline void DrawUP(IDirect3DDevice9* device,unsigned type,unsigned count,const void* data)
{device->DrawPrimitiveUP(type,count,data,32);}
// Retail009DA9F0: text interrupts the geometry batch, then forces the
// built-in shader to be reapplied before the next normal record.
void CRenderManager2D::Render2DDrawList(bool depth)
{
 CShaderRenderManager& shader=FableShaderRenderManager_013bc470;
 bool wasVertex=shader.VertexShadersEnabled;
 CTexture currentTexture,batchTexture;
 unsigned long nextBlend=1;
 bool differentWindow=false,line=false,changedLine=false,reapply=false;
 if(QuickDraw2DTriInfo.size())
 {
 if(shader.VertexShadersEnabled!=true)shader.EnableVertexShaders();
 bool wasPixel=shader.PixelShadersEnabled;
 if(shader.PixelShadersEnabled)shader.DisablePixelShaders();
 CRenderStateManager* state=FableCurrentRenderStateManager_013cb508;
 long& stackSize=state->StateStackSize;
 state->StateStack[stackSize].State=&state->BookmarkDummyState;
 state->CurrentBookmarkMask<<=1;++stackSize;
 SetRenderState(FableRenderDefaultValue_01396fb0,DeviceState.PolyCullMode);
 SetRenderState(FableRenderDefaultValue_01396fa4,DeviceState.PolyFillMode);
 SetRenderState(FableRenderDefaultValue_01396fa0,DeviceState.PolyShadeMode);
 SetRenderState(FableRenderDefaultValue_01396e58,DeviceState.TextureFilterModeMin[0]);
 SetRenderState(FableRenderDefaultValue_01396e58,DeviceState.TextureFilterModeMag[0]);
 SetTextureMipMapMode(0,(TEXTURE_MIPMAP_MODE)0);
 SetRenderState(FableRenderDefaultValue_01396dd0,DeviceState.TextureUVModeU[0]);
 SetRenderState(FableRenderDefaultValue_01396dd0,DeviceState.TextureUVModeV[0]);
 SetRenderState(0,DeviceState.FogEnable);
 SetRenderState(depth,DeviceState.ZBufferEnable);
 SetRenderState(FableRenderDefaultValue_01396e04,DeviceState.ZBufferFunc);
 SetRenderState(depth,DeviceState.ZBufferWriteEnable);
 SetRenderState(1,DeviceState.ClippingEnable);

 batchTexture=CTexture((IDirect3DTexture9*)0);
 unsigned long blend=1;LibraryStateBlockFunctions[1]->Apply();
 shader.RegisterVertexShaderConstantLayout(0,&Fable2DShaderConstantLayout_013ca8a8);
 long previousLayout=shader.CurrentVSConstantLayoutIndex;
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

 C2DBoxF window=QuickDraw2DTriInfo[0].Window;
 SetAWindow(window);
 unsigned index=0,batchStart=0;long firstVertex=0;
 for(;index<QuickDraw2DTriInfo.size();++index)
 {
  // Retail retains this record pointer across shader/font callbacks.
  CQuickDrawTriInfo* info=&QuickDraw2DTriInfo[index];
  if(!info->IsText)
  {
   if(reapply)
   {
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

    reapply=false;
   }
   nextBlend=info->BlendMode;
   if(info->UploadedTexture.PD3DTexture){currentTexture=info->UploadedTexture;info->UploadedTexture.Uninitialise();}
   else currentTexture.Uninitialise();
   differentWindow=memcmp(&window,&info->Window,16)!=0;
   if(line!=bool(info->Line))changedLine=true;
  }
  long count=index-batchStart;
  if(count>2000 || info->IsText || currentTexture.PD3DTexture!=batchTexture.PD3DTexture || blend!=nextBlend || differentWindow || changedLine)
  {
   if(count>0)
   {
    AttachTextureToStage(0,&batchTexture);
    if(line){RealiseRenderState();DrawUP(DisplayManager->D3DDevice,2,count,QuickDraw2DVertices.begin()+firstVertex);firstVertex+=count*2;}
    else {RealiseRenderState();DrawUP(DisplayManager->D3DDevice,4,count,QuickDraw2DVertices.begin()+firstVertex);firstVertex+=count*3;}
    batchStart=index;
   }
   if(info->IsText)
   {
    C3DVector position(info->StringX+info->Window.left,info->StringY+info->Window.top,info->StringZ);
    if(memcmp(&window,&info->Window,16)!=0){SetAWindow(info->Window);window=info->Window;}
    bool textVertex=shader.VertexShadersEnabled;
    if(shader.VertexShadersEnabled)shader.DisableVertexShaders();
    bool textPixel=shader.PixelShadersEnabled;
    if(shader.PixelShadersEnabled)shader.DisablePixelShaders();
    blend=9999;
    info->PFont->PrepareToRender(true);
    info->PFont->Render(info->String,position,info->StringScale,info->Colour,0,0);
    batchStart=index+1;line=false;
    batchTexture=CTexture((IDirect3DTexture9*)0);
    changedLine=false;reapply=true;
    if(textVertex!=shader.VertexShadersEnabled){if(textVertex)shader.EnableVertexShaders();else shader.DisableVertexShaders();}
    if(textPixel!=shader.PixelShadersEnabled){if(textPixel)shader.EnablePixelShaders();else shader.DisablePixelShaders();}
   }
   else
   {
    if(batchTexture.PD3DTexture!=currentTexture.PD3DTexture)batchTexture=currentTexture;
    if(nextBlend!=blend){LibraryStateBlockFunctions[nextBlend]->Apply();blend=nextBlend;}
    if(differentWindow){SetAWindow(info->Window);window=info->Window;}
    if(changedLine){line=!line;changedLine=false;}
   }
  }
 }
 if((long)(index-batchStart)>0)
 {
  AttachTextureToStage(0,&batchTexture);
  if(line){RealiseRenderState();DrawUP(DisplayManager->D3DDevice,2,index-batchStart,QuickDraw2DVertices.begin()+firstVertex);}
  else {RealiseRenderState();DrawUP(DisplayManager->D3DDevice,4,index-batchStart,QuickDraw2DVertices.begin()+firstVertex);}
 }
 QuickDraw2DTriInfo.erase(QuickDraw2DTriInfo.begin(),QuickDraw2DTriInfo.end());
 QuickDraw2DVertices.erase(QuickDraw2DVertices.begin(),QuickDraw2DVertices.end());
 AttachTextureToStage(0,0);
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
 }
}
