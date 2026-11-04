#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_font_instances.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"
#include "fable_font_vertex_buffer.h"
#include "fable_render_state.h"
#include "fable_2d_vector.h"
#include "fable_rgb_colour.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static unsigned row,mode,used,locks,unlocks,draws,selected,bytesUsed;
static unsigned char trace[200000],savedTrace[200000];
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"FONT_RENDER_FAILURE row=%u mode=%u line=%d\n",row,mode,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
static void Event(unsigned a,unsigned b=0,unsigned c=0,unsigned d=0){unsigned values[4]={a,b,c,d};CHECK(used+sizeof(values)<=sizeof(trace));memcpy(trace+used,values,sizeof(values));used+=sizeof(values);}
fable_i32 g_CCharStringInstanceCount_013BD800=0,g_CWideStringInstanceCount_013BCA20=0;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
extern "C" {float FontHalf=0.5f,FontOne=1.0f,FontUnsignedBias=4294967296.0f;}
#pragma comment(linker,"/include:_FontHalf")
#pragma comment(linker,"/include:_FontOne")
#pragma comment(linker,"/include:_FontUnsignedBias")
CStaticFontBank::CStaticFontBank(CFontDataBank*,unsigned long){}
CStaticFontBank::~CStaticFontBank(){}
void CStaticFontBank::PrepareToRender(bool)const{CHECK(false);}
__declspec(align(16)) static unsigned char systemStorage[sizeof(CSystemManager)],displayStorage[sizeof(CDisplayManager)],renderStorage[sizeof(CRenderManager)],managerStorage[sizeof(CFontManager)];
static CSystemManager* fixtureSystem=(CSystemManager*)systemStorage;
static CDisplayManager* display=(CDisplayManager*)displayStorage;
static CRenderManager* render=(CRenderManager*)renderStorage;
static CFontManager* manager=(CFontManager*)managerStorage;
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){Event(1);return fixtureSystem;}
__declspec(align(16)) static unsigned char rawBuffers[3][sizeof(CVertexBufferWin32)];
static CVertexBufferWin32* buffers[3]={(CVertexBufferWin32*)rawBuffers[0],(CVertexBufferWin32*)rawBuffers[1],(CVertexBufferWin32*)rawBuffers[2]};
static unsigned char gpuBytes[3][8432];static bool locked[3];static void* table[5];static void* resources[3][1];
static unsigned Index(void* self){for(unsigned i=0;i<3;++i)if(self==buffers[i])return i;CHECK(false);return 0;}
static unsigned char* __fastcall Lock(void* self,void*,unsigned flags,unsigned offset,unsigned bytes){unsigned i=Index(self);CHECK(selected==3 && !locked[i] && flags==0 && offset==0 && bytes==0);locked[i]=true;++locks;memset(gpuBytes[i],0xcd,sizeof(gpuBytes[i]));Event(2,i);return gpuBytes[i]+16;}
static void __fastcall Unlock(void* self,void*){unsigned i=Index(self);CHECK(locked[i]);locked[i]=false;++unlocks;for(unsigned n=0;n<16;++n)CHECK(gpuBytes[i][n]==0xcd && gpuBytes[i][8416+n]==0xcd);Event(3,i);CHECK(used+8400<=sizeof(trace));memcpy(trace+used,gpuBytes[i]+16,8400);used+=8400;}
static void __fastcall Cold(){CHECK(false);}
static fable_i32 __stdcall SetStream(FableRenderStateDevice* self,unsigned stream,IDirect3DVertexBuffer9* resource,unsigned offset,unsigned stride){CHECK(self==(FableRenderStateDevice*)render->PD3DDevice && !stream && !offset);if(!resource){CHECK(!stride);selected=3;Event(4,3);return 0;}for(unsigned i=0;i<3;++i)if(resource==(IDirect3DVertexBuffer9*)resources[i]){CHECK(!locked[i] && stride==28);selected=i;Event(4,i,stride);return 0;}CHECK(false);return 0;}
void CRenderManagerCore::DrawCurrentBuffersAsTris(const CTexture** textures,unsigned long count,unsigned long start,unsigned long triangles){CHECK(this==render && selected<3 && !locked[selected] && start==0 && triangles && triangles<=100 && (triangles%2)==0 && (count==1 || count==2));CHECK(textures[0]==(CTexture*)0x13572468);CHECK(count==1 || textures[1]==(CTexture*)0x24681357);++draws;bytesUsed+=triangles*3*28;Event(5,selected,count,triangles);}
struct CFontManagerTestAccess{static void Reset(){manager->VertexBuffers._M_start=buffers;manager->VertexBuffers._M_finish=buffers+3;manager->VertexBuffers._M_end_of_storage._M_data=buffers+3;manager->CurrentVB=0;}static long Current(){return manager->CurrentVB;}};
static FableRenderStateDeviceVTable deviceTable;static FableRenderStateDevice device={&deviceTable};
static unsigned char* Original(const char* name,const char* patchName){FILE* input=fopen(name,"rb");CHECK(input);unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code && fread(code,1,1910,input)==1910 && fgetc(input)==EOF && !fclose(input));input=fopen(patchName,"rb");unsigned count;CHECK(input && fread(&count,4,1,input)==1);for(unsigned i=0;i<count;++i){unsigned patch[3];CHECK(fread(patch,4,3,input)==3 && patch[0]<=1906);*(unsigned*)(code+patch[0])=patch[1]-(patch[2]?(unsigned)(code+patch[0]+4):0);}CHECK(fgetc(input)==EOF && !fclose(input));return code;}
int main(int argc,char** argv){CHECK(argc==1 || argc==3);unsigned char* code=argc==3?Original(argv[1],argv[2]):0;typedef void(CStaticFontBank::*Method)(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const;Method original;CHECK(sizeof(original)==4);memcpy(&original,&code,4);
 unsigned savedControl=_controlfp(0,0);_controlfp(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);
 memset(systemStorage,0,sizeof(systemStorage));memset(displayStorage,0,sizeof(displayStorage));memset(renderStorage,0,sizeof(renderStorage));memset(managerStorage,0,sizeof(managerStorage));fixtureSystem->PDisplayManager.Data=display;CRenderManager* rp=render;memcpy(&display->PRenderManager,&rp,4);render->PD3DDevice=(IDirect3DDevice9*)&device;deviceTable.SetStreamSource=SetStream;
 table[0]=(void*)&Cold;table[1]=(void*)&Lock;table[2]=(void*)&Unlock;table[3]=table[4]=(void*)&Cold;
 for(unsigned i=0;i<3;++i){memset(rawBuffers[i],0,sizeof(rawBuffers[i]));*(void***)buffers[i]=table;buffers[i]->Buffer=(IDirect3DVertexBuffer9*)resources[i];buffers[i]->ByteSize=8400;buffers[i]->VertexStride=28;}
 {CStaticFontBank font(0,0);CGlyph glyphs[64];memset(font.GlyphBanks,0,sizeof(font.GlyphBanks));font.PManager=manager;font.PTexture=(CTexture*)0x13572468;font.MinChar=0;font.MaxChar=65535;font.TextureSize_X=64;font.TextureSize_Y=32;
  for(unsigned i=0;i<64;++i){memset(&glyphs[i],0xa5,sizeof(CGlyph));glyphs[i].Left=(i%7)*0.1f;glyphs[i].Right=glyphs[i].Left+0.07f;glyphs[i].Top=(i%5)*0.1f;glyphs[i].Bottom=glyphs[i].Top+0.07f;glyphs[i].Offset=(short)(int(i%5)-2);glyphs[i].Width=(short)(i%11);glyphs[i].Advance=(short)(int(i%13)-3);}
  for(unsigned i=0;i<1024;++i){font.GlyphBanks[i].FirstGlyph=0;font.GlyphBanks[i].NoGlyphs=64;font.GlyphBanks[i].Glyphs=glyphs;}
  wchar_t longText[122];for(unsigned i=0;i<121;++i)longText[i]=L'A'+i%26;longText[121]=0;
  wchar_t boundaryTexts[6][102];const unsigned boundaryLengths[6]={49,50,51,99,100,101};for(unsigned n=0;n<6;++n){for(unsigned i=0;i<boundaryLengths[n];++i)boundaryTexts[n][i]=L'A';boundaryTexts[n][boundaryLengths[n]]=0;}
  const wchar_t* texts[]={L"",L"A",L"ABC",L"A\nB",L"\n",L"\n\n",L"A\n\nB",L"\xfffe\xffff",boundaryTexts[0],boundaryTexts[1],boundaryTexts[2],boundaryTexts[3],boundaryTexts[4],boundaryTexts[5],longText};const float scales[]={0,0.5f,1,2,-1,0.1f,0.33333334f,1.00000012f};const unsigned heights[]={0,1,12,255,0x7fffffff,0x80000000,0xffffffff};
  for(unsigned t=0;t<15;++t)for(unsigned f=0;f<8;++f)for(unsigned s=0;s<8;++s)for(unsigned h=0;h<7;++h)for(unsigned pos=0;pos<3;++pos)for(unsigned config=0;config<4;++config)for(unsigned dimensions=0;dimensions<3;++dimensions){font.FontHeight=heights[h];font.TextureSize_X=dimensions==0?64:dimensions==1?67:16777219;font.TextureSize_Y=dimensions==0?32:dimensions==1?31:16777221;
   font.MinChar=config==3?65:0;font.MaxChar=config==3?65:65535;
   for(unsigned bi=0;bi<1024;++bi){font.GlyphBanks[bi].Glyphs=config==1 && bi!=0 && bi!=1 && bi!=1023?0:glyphs;font.GlyphBanks[bi].FirstGlyph=config==2?1:0;font.GlyphBanks[bi].NoGlyphs=config==2?1:64;}
   unsigned length=0;while(texts[t][length])++length;unsigned flags=f*2 | ((config&1)?0x80000001:0);C3DVector position(pos==0?13.25f:pos==1?0.1f:100000.1015625f,pos==0?-7.75f:pos==1?-0.3f:0.10000000894f,0.375f);unsigned zBits=pos==0?0x3ec00000:pos==1?0x7f812345:0x80000000;memcpy(&position.Z,&zBits,4);CRGBColour colour(0x12,0x34,0x56,0x78);__declspec(align(16)) unsigned char dataBytes[sizeof(CWideStringData)];CWideStringData* data=(CWideStringData*)dataBytes;memset(dataBytes,0,sizeof(dataBytes));data->SetText_(const_cast<wchar_t*>(texts[t]));data->SetFinish_((size_t)(texts[t]+length));data->SetEndOfStorage_((size_t)(texts[t]+length+1));data->owners=2;
   {CWideString text;if(length)*(CWideStringData**)&text=data;unsigned beforeFont[sizeof(font)/4+1];memcpy(beforeFont,&font,sizeof(font));unsigned referenceUsed=0,referenceLocks=0,referenceDraws=0;long referenceCurrent=0;unsigned referenceStatus=0,referenceControl=0;const unsigned caseControl=_controlfp(0,0);unsigned char beforeGlyphs[sizeof(glyphs)],beforeBuffers[sizeof(rawBuffers)];memcpy(beforeGlyphs,glyphs,sizeof(glyphs));memcpy(beforeBuffers,rawBuffers,sizeof(rawBuffers));unsigned visibleGlyphs=0;for(unsigned n=0;n<length;++n){unsigned ch=texts[t][n];if((long)ch<font.MinChar || (long)ch>font.MaxChar)continue;const CStaticFontGlyphBank& gb=font.GlyphBanks[ch>>6];if(gb.Glyphs && (ch%64-gb.FirstGlyph)<gb.NoGlyphs)++visibleGlyphs;}
    for(mode=0;mode<(code?2:1);++mode){used=locks=unlocks=draws=bytesUsed=0;selected=3;memset(locked,0,sizeof(locked));CFontManagerTestAccess::Reset();_clearfp();const CTexture* overrideTexture=(row&1)?(CTexture*)0x24681357:0;if(mode)(font.*original)(text,position,scales[s],colour,flags,overrideTexture);else font.Render(text,position,scales[s],colour,flags,overrideTexture);CHECK(locks==visibleGlyphs/50+1 && draws==(visibleGlyphs+49)/50 && bytesUsed==visibleGlyphs*168 && CFontManagerTestAccess::Current()==(long)(locks%3) && _controlfp(0,0)==caseControl && !memcmp(beforeGlyphs,glyphs,sizeof(glyphs)) && !memcmp(beforeBuffers,rawBuffers,sizeof(rawBuffers)));CHECK(locks==unlocks && selected==3 && !locked[0] && !locked[1] && !locked[2] && !memcmp(beforeFont,&font,sizeof(font)));
     if(!mode){referenceStatus=_statusfp();referenceControl=_controlfp(0,0);referenceUsed=used;referenceLocks=locks;referenceDraws=draws;referenceCurrent=CFontManagerTestAccess::Current();memcpy(savedTrace,trace,used);}else{CHECK(referenceStatus==_statusfp() && referenceControl==_controlfp(0,0));CHECK(referenceUsed==used && referenceLocks==locks && referenceDraws==draws && referenceCurrent==CFontManagerTestAccess::Current());if(memcmp(savedTrace,trace,used)){for(unsigned i=0;i<used;++i)if(trace[i]!=savedTrace[i]){fprintf(stderr,"TRACE_DIFF byte=%u candidate=%02x original=%02x text=%u flags=%u scale=%.9g height=%u\n",i,savedTrace[i],trace[i],t,flags,scales[s],heights[h]);for(unsigned v=0;v<6;++v){float* a=(float*)(savedTrace+48+v*28);float* b=(float*)(trace+48+v*28);fprintf(stderr,"VERTEX%u C=(%.9g,%.9g) O=(%.9g,%.9g)\n",v,a[0],a[1],b[0],b[1]);}break;}CHECK(false);}}
    }*(CWideStringData**)&text=0;
   }++row;
  }
 }
 CHECK(!g_CCharStringInstanceCount_013BD800 && !g_CWideStringInstanceCount_013BCA20);_controlfp(savedControl,_MCW_PC|_MCW_RC);if(code)CHECK(VirtualFree(code,0,0x8000));printf("STATIC_FONT_RENDER_SCRATCH_PASS cases=%u original_paths=%u\n",row,row*(code?2:1));return 0;
}
