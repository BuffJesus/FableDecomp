#include <stdio.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "native_clear_event_ids.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mutation;
static unsigned& word(unsigned address){return *(unsigned*)address;}
static void event(unsigned kind,const void* receiver=0,unsigned count=0,const unsigned* args=0,const void* payload=0,unsigned length=0)
{
 unsigned header[10]={0};unsigned char data[140]={0};header[0]=kind;header[1]=(unsigned)receiver;header[2]=count;header[8]=length;
 for(unsigned i=0;i<count;++i)header[3+i]=args[i];if(length)memcpy(data,payload,length);
 fwrite(header,4,10,output);fwrite(data,1,140,output);fwrite(arena,1,0x6000,output);fwrite((void*)0x13bc470,1,0x3dc,output);
}
void CDisplayManager::ClearRenderTarget(const CRGBColour& colour,unsigned long flags)
{unsigned args[]={(unsigned)&colour,flags};event(E_ClearRenderTarget,this,2,args);}
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions()
{event(E_GetRenderTargetDimensions,this);return C2DExtentsI(RenderTargetDimensionsStorage_[0],RenderTargetDimensionsStorage_[1]);}
void CDisplayManager::SetIntegerViewportEndpoint(const C2DBoxI& viewport)
{event(E_SetIntegerViewportEndpoint,this,0,0,&viewport,16);ViewportBox=viewport;}
void CShaderRenderManager::EnableVertexShaders(){event(E_EnableVertexShaders,this);VertexShadersEnabled=true;}
void CShaderRenderManager::DisableVertexShaders(){event(E_DisableVertexShaders,this);VertexShadersEnabled=false;}
void CShaderRenderManager::EnablePixelShaders(){event(E_EnablePixelShaders,this);PixelShadersEnabled=true;}
void CShaderRenderManager::DisablePixelShaders(){event(E_DisablePixelShaders,this);PixelShadersEnabled=false;}
void CShaderRenderManager::RegisterVertexShaderConstantLayout(long index,const CVertexShaderConstantLayout* layout)
{unsigned args[]={(unsigned)index,(unsigned)layout};event(E_RegisterVertexShaderConstantLayout,this,2,args);if(mutation)CurrentVSConstantLayoutIndex=19;}
void CShaderRenderManager::SetVertexShaderConstantLayout(long index)
{unsigned arg=(unsigned)index;event(E_SetVertexShaderConstantLayout,this,1,&arg);CurrentVSConstantLayoutIndex=index;CurrentVSConstantLayout=(const CVertexShaderConstantLayout*)0x13ca8a8;}
void CShaderRenderManager::ApplyVertexShader(const CVertexShader& shader,bool force)
{unsigned args[]={(unsigned)&shader,(unsigned)force};event(E_ApplyVertexShader,this,2,args);}
void CShaderRenderManager::SetVSConstants(long offset,const void* data,unsigned long count)
{unsigned args[]={(unsigned)offset,(unsigned)data,count};event(E_SetVSConstants,this,3,args);}
void CShaderRenderManager::UpdatePixelShader()
{event(E_UpdatePixelShader,this);if(mutation)updateFlags3D8^=0x20486;}
void CShaderRenderManager::UpdatePixelShaderConstants(){event(E_UpdatePixelShaderConstants,this);}
void CShaderRenderManager::UpdateAmbient(){event(E_UpdateAmbient,this);}
void CShaderRenderManager::UpdateFogColour(){event(E_UpdateFogColour,this);}
void CShaderRenderManager::UpdateLightGlobals(){event(E_UpdateLightGlobals,this);}
void CShaderRenderManager::ResetWorldTransform(){event(E_ResetWorldTransform,this);WorldMatrixIsIdentity=true;}
void CShaderRenderManager::UpdateCombinedProjectionTransform(){event(E_UpdateCombinedProjectionTransform,this);}
void CRenderStateManager::RestoreCaptureBlock(){event(E_RestoreCaptureBlock,this);}
void CRenderManagerCore::DrawAsIndexedTris(const void* vertices,long stride,long count,void* indices,long primitives)
{
 unsigned args[]={(unsigned)stride,(unsigned)count,(unsigned)primitives};unsigned char data[140];
 memcpy(data,vertices,128);memcpy(data+128,indices,12);for(unsigned i=0;i<4;++i)memset(data+i*32+20,0,4);
 event(E_DrawAsIndexedTris,this,3,args,data,140);
}
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x20000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 if(VirtualAlloc((void*)0x1390000,0x40000,0x3000,4)!=(void*)0x1390000)return 5;
 if(fread((void*)0x1390000,1,0x40000,input)!=0x40000)return 6;
 unsigned flags,cases=0;
 while(fread(&flags,4,1,input)==1)
 {
  if(fread(&mutation,4,1,input)!=1 || fread(arena,1,0x6000,input)!=0x6000 || fread((void*)0x13bc470,1,0x3dc,input)!=0x3dc || fread((void*)0x13ca8a8,1,0x14c,input)!=0x14c)return 7;
  word(0x13cb508)=(unsigned)arena;
  ((CRenderManager2D*)arena)->ClearRenderTarget(*(CRGBColour*)(arena+0x5000),flags);event(0);++cases;
 }
 if(ferror(input))return 8;fclose(input);if(fclose(output))return 9;
 printf("NATIVE_CLEAR_RENDER_TARGET_PASS cases=%u\n",cases);return 0;
}
