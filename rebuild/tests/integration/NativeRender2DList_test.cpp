#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "fable_gfuninitialise.h"
#include "fable_font_bank.h"
#include "fable_2d_vector.h"
#include "native_render2d_event_ids.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mutation,width;
static unsigned& word(unsigned address){return *(unsigned*)address;}
static void event(unsigned kind,const void* receiver=0,unsigned count=0,const unsigned* args=0,const void* payload=0,unsigned length=0)
{
 unsigned header[10]={0};unsigned char data[140]={0};header[0]=kind;header[1]=(unsigned)receiver;header[2]=count;header[8]=length;
 for(unsigned i=0;i<count;++i)header[3+i]=args[i];if(length)memcpy(data,payload,length);
 fwrite(header,4,10,output);fwrite(data,1,140,output);if(width>0x9000 && kind==E_StringDestroy)return;fwrite(arena,1,width,output);fwrite((void*)0x13bc470,1,0x3dc,output);
}
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
void CRenderManagerCore::SetAWindow(const C2DBoxF& value)
{event(E_Window,this,0,0,&value,16);if(mutation)word((unsigned)&value)^=0x80000000;}
void CRenderStateManager::RealiseRenderState()
{
 event(E_Realise,this);
 if(mutation){
  word((unsigned)arena+0x4058)=(unsigned)arena+(word((unsigned)arena+0x4058)==(unsigned)arena+0x8500?0x8520:0x8500);
  unsigned a=(unsigned)arena+0x3e88,start=word(a),length=word(a+4)-start;
  unsigned next=(unsigned)arena+(start==(unsigned)arena+0x5000?0x6000:0x5000);word(a)=next;word(a+4)=next+length;word(a+8)=next+0x1000;
 }
}
CWideString::~CWideString(){event(E_StringDestroy,this);storage_=0;}
static unsigned long __stdcall release(void* receiver){event(E_Release,receiver);return 7;}
static unsigned long __stdcall addref(void* receiver){event(E_AddRef,receiver);return 7;}
static long __stdcall setTexture(void* receiver,unsigned stage,void* texture)
{unsigned args[]={stage,(unsigned)texture};event(E_SetTexture,receiver,2,args);if(mutation)word((unsigned)arena+0x3cb8)=8;return 0x80004005;}
static long __stdcall draw(void* receiver,unsigned type,unsigned count,const void* vertices,unsigned stride)
{unsigned args[]={type,count,(unsigned)vertices,stride};event(E_DrawUP,receiver,4,args);return 0x80004005;}
class BlockProbe:public CStateBlockFunctionBase {
public: virtual void Apply(){event(E_ApplyBlock,this);if(mutation)word(0x13bc848)^=0x20487;}
};
#ifdef FABLE_COMPOSE_STATE_BLOCKS
CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{event(E_SystemRoot);return (CSystemManager*)(arena+0x8800);}
#endif

bool CFontBank::Preload(const CWideString&,bool)const{return false;}
class FontProbe:public CFontBank {
public:
 virtual C2DExtentsF GetStringExtents(const wchar_t*,bool)const{return C2DExtentsF(0,0);}
 virtual float GetFontMaxHeight()const{return 0;}
 virtual void PrepareToRender(bool enabled)const
 {
  unsigned arg=enabled;event(E_FontPrepare,this,1,&arg);
  if(mutation){
   *(unsigned char*)0x13bc800=1;*(unsigned char*)0x13bc801=1;
   for(unsigned at=word((unsigned)arena+0x3e94);at<word((unsigned)arena+0x3e98);at+=60)
    if(*(unsigned char*)at&2){word(at+0x2c)^=0x80000000;word(at+0x38)=(unsigned)arena+(word(at+0x38)==(unsigned)arena+0x8700?0x8720:0x8700);}
  }
 }
 virtual void Render(const CWideString& text,const C3DVector& position,float scale,const CRGBColour& colour,unsigned long flags,const CTexture* texture)const
 {
  unsigned bits;memcpy(&bits,&scale,4);unsigned args[]={(unsigned)&text,bits,(unsigned)&colour,flags,(unsigned)texture};event(E_FontRender,this,5,args,&position,12);
  if(mutation){*(unsigned char*)0x13bc800=0;*(unsigned char*)0x13bc801=0;}
 }
};
void CPixelFormat::Initialise(D3DFORMAT){abort();}
unsigned long CPixelFormat::GetColourDepth()const{abort();return 0;}
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x100000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 if(VirtualAlloc((void*)0x1390000,0x40000,0x3000,4)!=(void*)0x1390000)return 5;
 if(fread((void*)0x1390000,1,0x40000,input)!=0x40000)return 6;
 word((unsigned)arena+0xd0004)=(unsigned)&addref;word((unsigned)arena+0xd0008)=(unsigned)&release;
 word((unsigned)arena+0xd0100+65*4)=(unsigned)&setTexture;word((unsigned)arena+0xd0100+83*4)=(unsigned)&draw;
 BlockProbe probe;word((unsigned)arena+0xd0400)=**(unsigned**)&probe;
 FontProbe font;memcpy(arena+0xd0600,*(void***)&font,6*4);
 unsigned flags,cases=0;
 while(fread(&flags,4,1,input)==1)
 {
  if(fread(&mutation,4,1,input)!=1 || fread(&width,4,1,input)!=1 || width>0xc0000 || fread(arena,1,width,input)!=width || fread((void*)0x13bc470,1,0x3dc,input)!=0x3dc || fread((void*)0x13ca8a8,1,0x14c,input)!=0x14c)return 7;
  word(0x13cb508)=(unsigned)arena;
#ifndef FABLE_COMPOSE_STATE_BLOCKS
  for(unsigned i=0;i<7;++i)word(0x1396510+i*4)=(unsigned)arena+0x8600+i*16;
#endif
  ((CRenderManager2D*)arena)->Render2DDrawList(flags!=0);event(0);++cases;
 }
 if(ferror(input))return 8;fclose(input);if(fclose(output))return 9;
 printf("NATIVE_RENDER2D_LIST_PASS cases=%u\n",cases);return 0;
}

CWideString& CWideString::operator=(const CWideString& other){abort();return *this;}

fable_i32 g_CCharStringInstanceCount_013BD800=0;
#include "../../src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp"
#include "../../src/compiled/00/99/CCharString_DefaultConstructor_0099e4b0.cpp"
#include "../../src/compiled/00/99/CCharString_Destructor_0099eae0.cpp"
#include "../../src/compiled/00/99/CCharString_UnassignString_0099e9b0.cpp"
