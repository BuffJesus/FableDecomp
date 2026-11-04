#include <new>
#include <stdlib.h>
#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "fable_vertex_shader.h"
#include "fable_render_state_defaults.h"
#include "fable_render2d_sort.h"
#include "fable_stlport_algobase.h"
namespace _STL {
template<> CQuickDrawTriInfo* vector<CQuickDrawTriInfo>::erase(CQuickDrawTriInfo*,CQuickDrawTriInfo*);
template<> inline CTVertexRHWColSpecTex1* vector<CTVertexRHWColSpecTex1>::erase(CTVertexRHWColSpecTex1* first,CTVertexRHWColSpecTex1* last)
{CTVertexRHWColSpecTex1* finish=__copy(last,this->_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);_Destroy(finish,this->_M_finish);this->_M_finish=finish;return first;}
}
static __forceinline void DrawUP(IDirect3DDevice9* device,unsigned type,unsigned count,const void* data)
{device->DrawPrimitiveUP(type,count,data,32);}
// Full retail body009D9C80. Preserve the two passes and callback-time
// reloads: opaque and alpha queues observe texture/window changes differently.
void CRenderManager2D::DrawBufferedPolys()
{
 CShaderRenderManager& shader=FableShaderRenderManager_013bc470;
 CTexture currentTexture,batchTexture;
 bool line=false,changedLine=false;
 bool wasVertex=shader.VertexShadersEnabled;
 long firstVertex=0;
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
 SetRenderState(1,DeviceState.ZBufferEnable);
 SetRenderState(FableRenderDefaultValue_01396e04,DeviceState.ZBufferFunc);
 SetRenderState(1,DeviceState.ZBufferWriteEnable);
 SetRenderState(1,DeviceState.ClippingEnable);
 unsigned long blend=1;LibraryStateBlockFunctions[1]->Apply();
 long previousLayout;
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
 {
 C2DBoxF window(0,0,0,0);
 unsigned index=0,batchStart=0;
 for(;index<QuickDrawTriInfo.size();++index)
 {
  unsigned long nextBlend=QuickDrawTriInfo[index].BlendMode;
  bool differentWindow=memcmp(&window,&QuickDrawTriInfo[index].Window,16)!=0;
  if(QuickDrawTriInfo[index].UploadedTexture.PD3DTexture){currentTexture=QuickDrawTriInfo[index].UploadedTexture;QuickDrawTriInfo[index].UploadedTexture.Uninitialise();}
  else currentTexture.Uninitialise();
  if(line!=bool(QuickDrawTriInfo[index].Line))changedLine=true;
  if(nextBlend!=blend || currentTexture.PD3DTexture!=batchTexture.PD3DTexture || differentWindow || changedLine)
  {
   long count=index-batchStart;
   if(count>0)
   {
    AttachTextureToStage(0,&batchTexture);
    if(line){RealiseRenderState();DrawUP(DisplayManager->D3DDevice,2,count,QuickDrawVertices.begin()+firstVertex);firstVertex+=count*2;}
    else {RealiseRenderState();DrawUP(DisplayManager->D3DDevice,4,count,QuickDrawVertices.begin()+firstVertex);firstVertex+=count*3;}
    batchStart=index;
   }
   batchTexture=currentTexture;
   if(nextBlend!=blend){LibraryStateBlockFunctions[nextBlend]->Apply();blend=nextBlend;}
   if(differentWindow){SetAWindow(QuickDrawTriInfo[index].Window);window=QuickDrawTriInfo[index].Window;}
   if(changedLine){line=!line;changedLine=false;}
  }
 }
 if((long)(index-batchStart)>0)
 {
  AttachTextureToStage(0,&currentTexture);
  if(line){RealiseRenderState();DrawUP(DisplayManager->D3DDevice,2,index-batchStart,QuickDrawVertices.begin()+firstVertex);}
  else {RealiseRenderState();DrawUP(DisplayManager->D3DDevice,4,index-batchStart,QuickDrawVertices.begin()+firstVertex);}
 }
 }
 qsort(QuickDrawAlphaTriInfo.begin(),QuickDrawAlphaTriInfo.size(),60,FableCompareQuickDrawInfoDepth_009d8c30);
 qsort(QuickDrawAlphaVertices.begin(),QuickDrawAlphaTriInfo.size(),96,FableCompareQuickDrawVerticesDepth_009d8cb0);
 firstVertex=0;
 {
 C2DBoxF window(0,0,0,0);
 unsigned index=0,batchStart=0;
 for(;index<QuickDrawAlphaTriInfo.size();++index)
 {
  unsigned long nextBlend=QuickDrawAlphaTriInfo[index].BlendMode;
  if(QuickDrawAlphaTriInfo[index].UploadedTexture.PD3DTexture){currentTexture=QuickDrawAlphaTriInfo[index].UploadedTexture;QuickDrawAlphaTriInfo[index].UploadedTexture.Uninitialise();}
  else currentTexture.Uninitialise();
  bool differentWindow=memcmp(&window,&QuickDrawAlphaTriInfo[index].Window,16)!=0;
  if(line!=bool(QuickDrawAlphaTriInfo[index].Line))changedLine=true;
  if(nextBlend!=blend || currentTexture.PD3DTexture!=batchTexture.PD3DTexture || differentWindow || changedLine)
  {
   long count=index-batchStart;
   if(count>0)
   {
    AttachTextureToStage(0,&batchTexture);
    if(line){RealiseRenderState();DrawUP(DisplayManager->D3DDevice,2,count,QuickDrawAlphaVertices.begin()+firstVertex);firstVertex+=count*2;}
    else {RealiseRenderState();DrawUP(DisplayManager->D3DDevice,4,count,QuickDrawAlphaVertices.begin()+firstVertex);firstVertex+=count*3;}
    batchStart=index;
   }
   batchTexture=currentTexture;
   if(nextBlend!=blend){LibraryStateBlockFunctions[nextBlend]->Apply();blend=nextBlend;}
   if(differentWindow){SetAWindow(QuickDrawAlphaTriInfo[index].Window);window=QuickDrawAlphaTriInfo[index].Window;}
   if(changedLine){line=!line;changedLine=false;}
  }
 }
 if((long)(index-batchStart)>0)
 {
  AttachTextureToStage(0,&currentTexture);
  if(line){RealiseRenderState();DrawUP(DisplayManager->D3DDevice,2,index-batchStart,QuickDrawAlphaVertices.begin()+firstVertex);}
  else {RealiseRenderState();DrawUP(DisplayManager->D3DDevice,4,index-batchStart,QuickDrawAlphaVertices.begin()+firstVertex);}
 }
 }
 QuickDrawTriInfo.erase(QuickDrawTriInfo.begin(),QuickDrawTriInfo.end());
 QuickDrawVertices.erase(QuickDrawVertices.begin(),QuickDrawVertices.end());
 QuickDrawAlphaTriInfo.erase(QuickDrawAlphaTriInfo.begin(),QuickDrawAlphaTriInfo.end());
 QuickDrawAlphaVertices.erase(QuickDrawAlphaVertices.begin(),QuickDrawAlphaVertices.end());
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
