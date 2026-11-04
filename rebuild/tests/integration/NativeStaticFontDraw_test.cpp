// Font draw composition: actual CRenderStateManager::Init, CStaticFontBank::PrepareToRender,
// CStaticFontBank::Render, Win32 vertex buffers, CRenderManagerCore::DrawCurrentBuffersAsTris and
// RealiseRenderState against a recording fake device, in the retail Render2DDrawList order
// (PrepareToRender then Render). With two arguments the retail Render/Prepare/Draw/Realise bytes
// run as one transplanted graph and every device call, vertex payload and manager byte must match.
#include <new>
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
static unsigned row,mode,pass,locks,unlocks,selected,phase,creating,created,released,systemCalls,driverStatus,nevents,payloadUsed;
struct DeviceEvent{unsigned kind,a,b,c,d;};
static DeviceEvent events[4096],savedEvents[4096];static unsigned savedCount,savedPayloadUsed;
static unsigned char payload[6*8400],savedPayload[6*8400];
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"FONT_DRAW_FAILURE row=%u mode=%u pass=%u line=%d\n",row,mode,pass,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
enum{E_SYSTEM=1,E_LOCK,E_UNLOCK,E_STREAM,E_FVF,E_TEXTURE,E_RS,E_TSS,E_SAMPLER,E_DRAW};
static void Event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){CHECK(nevents<4096);DeviceEvent e={kind,a,b,c,d};events[nevents++]=e;}
fable_i32 g_CCharStringInstanceCount_013BD800=0,g_CWideStringInstanceCount_013BCA20=0;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
extern "C" {float FontHalf=0.5f,FontOne=1.0f,FontUnsignedBias=4294967296.0f;}
#pragma comment(linker,"/include:_FontHalf")
#pragma comment(linker,"/include:_FontOne")
#pragma comment(linker,"/include:_FontUnsignedBias")
CStaticFontBank::CStaticFontBank(CFontDataBank*,unsigned long){}
CStaticFontBank::~CStaticFontBank(){}
__declspec(align(16)) static unsigned char systemStorage[sizeof(CSystemManager)],displayStorage[sizeof(CDisplayManager)],renderStorage[sizeof(CRenderManager)],managerStorage[sizeof(CFontManager)];
static CSystemManager* fixtureSystem=(CSystemManager*)systemStorage;
static CDisplayManager* display=(CDisplayManager*)displayStorage;
static CRenderManager* render=(CRenderManager*)renderStorage;
static CFontManager* manager=(CFontManager*)managerStorage;
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){++systemCalls;Event(E_SYSTEM);return fixtureSystem;}
__declspec(align(16)) static unsigned char rawBuffers[3][sizeof(CVertexBufferWin32)];
static CVertexBufferWin32* buffers[3]={(CVertexBufferWin32*)rawBuffers[0],(CVertexBufferWin32*)rawBuffers[1],(CVertexBufferWin32*)rawBuffers[2]};
static unsigned char gpuBytes[3][8432];static bool locked[3];static void* resourceTable[14];static void* resources[3][1];static unsigned references[3];
static void* deviceTable[119];static void* device[1]={deviceTable};
// Two texture wrappers: the font's own page and a caller override; distinct fake D3D handles.
__declspec(align(16)) static unsigned char textureStorage[2][sizeof(CTexture)];static unsigned char d3dTextures[2][16];
static CTexture* fontTexture=(CTexture*)textureStorage[0];static CTexture* overrideTexture=(CTexture*)textureStorage[1];
static unsigned Index(void* self){for(unsigned i=0;i<3;++i)if(self==resources[i])return i;CHECK(false);return 0;}
static HRESULT __stdcall Lock(void* self,unsigned offset,unsigned bytes,void** output,unsigned flags){unsigned i=Index(self);CHECK(phase==2 && selected==3 && !locked[i] && flags==0 && offset==0 && bytes==8400 && output && !*output && references[i]==1);locked[i]=true;++locks;memset(gpuBytes[i],0xcd,sizeof(gpuBytes[i]));Event(E_LOCK,i);*output=gpuBytes[i]+16;return (HRESULT)driverStatus;}
static HRESULT __stdcall Unlock(void* self){unsigned i=Index(self);CHECK(phase==2 && locked[i] && references[i]==1);locked[i]=false;++unlocks;for(unsigned n=0;n<16;++n)CHECK(gpuBytes[i][n]==0xcd && gpuBytes[i][8416+n]==0xcd);Event(E_UNLOCK,i);CHECK(payloadUsed+8400<=sizeof(payload));memcpy(payload+payloadUsed,gpuBytes[i]+16,8400);payloadUsed+=8400;return (HRESULT)driverStatus;}
static unsigned long __stdcall Release(void* self){unsigned i=Index(self);CHECK(phase==3 && references[i]==1 && !locked[i]);references[i]=0;++released;return 0;}
static HRESULT __stdcall Create(void* self,unsigned bytes,unsigned flags,unsigned format,D3DPOOL pool,IDirect3DVertexBuffer9** output,HANDLE* shared){CHECK(phase==1 && creating<3 && self==device && bytes==8400 && flags==8 && format==0x144 && (unsigned)pool==1 && !shared && output==&buffers[creating]->Buffer && !*output && !references[creating]);*output=(IDirect3DVertexBuffer9*)resources[creating];references[creating]=1;++created;return 0;}
extern "C" void* __fastcall BufferScalar(CVertexBufferWin32*,void*,unsigned);
#pragma comment(linker,"/alternatename:@BufferScalar@12=??_GCVertexBufferWin32@@UAEPAXI@Z")
static HRESULT __stdcall SetStream(void* self,unsigned stream,IDirect3DVertexBuffer9* resource,unsigned offset,unsigned stride){CHECK(self==device && !stream && !offset);if(!resource){CHECK(!stride);selected=3;Event(E_STREAM,3);return 0;}for(unsigned i=0;i<3;++i)if(resource==(IDirect3DVertexBuffer9*)resources[i]){CHECK(!locked[i] && stride==28);selected=i;Event(E_STREAM,i,stride);return 0;}CHECK(false);return 0;}
static HRESULT __stdcall SetFVF(void* self,unsigned format){CHECK(self==device && phase==2);Event(E_FVF,format);return (HRESULT)driverStatus;}
static HRESULT __stdcall SetTexture(void* self,unsigned stage,void* texture){CHECK(self==device && phase==2 && stage<2 && (texture==d3dTextures[0] || texture==d3dTextures[1] || !texture));Event(E_TEXTURE,stage,(unsigned)texture);return (HRESULT)driverStatus;}
static HRESULT __stdcall SetRS(void* self,unsigned state,unsigned value){CHECK(self==device);Event(E_RS,state,value);return (HRESULT)driverStatus;}
static HRESULT __stdcall SetTSS(void* self,unsigned stage,unsigned state,unsigned value){CHECK(self==device && stage<8);Event(E_TSS,stage,state,value);return (HRESULT)driverStatus;}
static HRESULT __stdcall SetSampler(void* self,unsigned stage,unsigned state,unsigned value){CHECK(self==device && stage<8);Event(E_SAMPLER,stage,state,value);return (HRESULT)driverStatus;}
static unsigned capsProfile,capsCalls;
// SetupRenderStates queries RasterCaps for depth bias (0x04000000), slope bias (0x02000000) and LOD bias (0x2000).
static HRESULT __stdcall GetCaps(void* self,D3DCAPS9* caps){CHECK(self==device && caps);memset(caps,0xa5,sizeof(*caps));caps->RasterCaps=capsProfile?0x06002000:0;++capsCalls;return 0;}
static HRESULT __stdcall DrawPrimitive(void* self,unsigned type,unsigned start,unsigned count){CHECK(self==device && phase==2 && selected<3 && !locked[selected]);Event(E_DRAW,type,start,count,selected);return (HRESULT)driverStatus;}
struct CFontManagerTestAccess{static void Reset(){manager->VertexBuffers._M_start=buffers;manager->VertexBuffers._M_finish=buffers+3;manager->VertexBuffers._M_end_of_storage._M_data=buffers+3;manager->CurrentVB=0;}};
struct CRenderStateManagerTestAccess{
 static long Registered(){return render->AllStatesListSize;}
 static long Dirty(){return render->StateUpdateListSize;}
 static CRenderStateManager::CStateInfo* Registered(long i){return render->AllStatesList[i];}
 static CRenderStateManager::CStateInfo* Dirty(long i){return render->StateUpdateList[i];}
 static void* StateDevice(){return render->CRenderStateManager::PD3DDevice;}
 // An earlier undrawn caller changes states Prepare later restores, leaving queued entries whose desired
 // value equals the realised one (Realise must skip them).
 static void History(){CRenderStateManager::CStateInfo& z=render->DeviceState.ZBufferEnable;CRenderStateManager::CStateInfo& fog=render->DeviceState.FogEnable;render->SetRenderState(z.DesiredState^1,z);render->SetRenderState(fog.DesiredState^1,fog);}
};
// Retail Render(0xAB7C20,1910B), PrepareToRender(0xAB83A0,2672B), DrawCurrentBuffersAsTris(0xA0ABE0,127B) and
// RealiseRenderState(0xA058C0,167B) transplanted into one block; patch kinds: 0 absolute, 1 rel32 to an
// absolute fixture address, 2 rel32 to an offset inside the block.
enum{RENDER_AT=0,PREPARE_AT=2048,DRAW_AT=4864,REALISE_AT=5120,BLOCK=5287};
static unsigned char* Original(const char* name,const char* patchName){FILE* input=fopen(name,"rb");CHECK(input);unsigned char* code=(unsigned char*)VirtualAlloc(0,8192,0x3000,0x40);CHECK(code && fread(code,1,BLOCK,input)==BLOCK && fgetc(input)==EOF && !fclose(input));input=fopen(patchName,"rb");unsigned count;CHECK(input && fread(&count,4,1,input)==1);for(unsigned i=0;i<count;++i){unsigned patch[3];CHECK(fread(patch,4,3,input)==3 && patch[0]<=BLOCK-4 && patch[2]<=2);unsigned target=patch[2]==2?(unsigned)(code+patch[1]):patch[1];*(unsigned*)(code+patch[0])=target-(patch[2]?(unsigned)(code+patch[0]+4):0);}CHECK(fgetc(input)==EOF && !fclose(input));return code;}
// Independent expectations for one Prepare+Render pass from the manager state Prepare left behind.
struct Expected{unsigned dirtyCalls,unchanged;DeviceEvent calls[256];};
static void ExpectRealise(Expected& e){e.dirtyCalls=e.unchanged=0;for(long i=0;i<CRenderStateManagerTestAccess::Dirty();++i){CRenderStateManager::CStateInfo* s=CRenderStateManagerTestAccess::Dirty(i);CHECK(s->DirtyListFlag);if(s->DesiredState==s->CurrentState){++e.unchanged;continue;}CHECK(s->StateType>=1 && s->StateType<=3);DeviceEvent v={s->StateType==1?E_RS:s->StateType==2?E_TSS:E_SAMPLER,0,0,0,0};if(s->StateType==1){v.a=s->StateParam;v.b=s->DesiredState;}else{v.a=s->TextureStage;v.b=s->StateParam;v.c=s->DesiredState;}e.calls[e.dirtyCalls++]=v;}}
static bool Same(const DeviceEvent& a,const DeviceEvent& b){return a.kind==b.kind && a.a==b.a && a.b==b.b && a.c==b.c && a.d==b.d;}
int main(int argc,char** argv){CHECK(argc==1 || argc==3);unsigned char* code=argc==3?Original(argv[1],argv[2]):0;
 typedef void(CStaticFontBank::*RenderMethod)(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const;typedef void(CStaticFontBank::*PrepareMethod)(bool)const;
 RenderMethod originalRender;PrepareMethod originalPrepare;CHECK(sizeof(originalRender)==4 && sizeof(originalPrepare)==4);unsigned char* renderCode=code?code+RENDER_AT:0;unsigned char* prepareCode=code?code+PREPARE_AT:0;memcpy(&originalRender,&renderCode,4);memcpy(&originalPrepare,&prepareCode,4);
 unsigned savedControl=_controlfp(0,0);_controlfp(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);
 memset(systemStorage,0,sizeof(systemStorage));memset(displayStorage,0,sizeof(displayStorage));memset(renderStorage,0,sizeof(renderStorage));memset(managerStorage,0,sizeof(managerStorage));fixtureSystem->PDisplayManager.Data=display;CRenderManager* rp=render;memcpy(&display->PRenderManager,&rp,4);display->D3DDevice=(IDirect3DDevice9*)device;
 deviceTable[7]=(void*)&GetCaps;deviceTable[26]=(void*)&Create;deviceTable[57]=(void*)&SetRS;deviceTable[65]=(void*)&SetTexture;deviceTable[67]=(void*)&SetTSS;deviceTable[69]=(void*)&SetSampler;deviceTable[81]=(void*)&DrawPrimitive;deviceTable[89]=(void*)&SetFVF;deviceTable[100]=(void*)&SetStream;
 resourceTable[2]=(void*)&Release;resourceTable[11]=(void*)&Lock;resourceTable[12]=(void*)&Unlock;
 memset(textureStorage,0,sizeof(textureStorage));fontTexture->PD3DTexture=(IDirect3DTexture9*)d3dTextures[0];overrideTexture->PD3DTexture=(IDirect3DTexture9*)d3dTextures[1];
 // Core boundary: CRenderManagerCore::Init also initialises the system font, device caps and back-buffer
 // format; only its device pointer and stage count, plus the zeroed texture cache, are used here.
 render->PD3DDevice=(IDirect3DDevice9*)device;
 phase=1;for(creating=0;creating<3;++creating){resources[creating][0]=resourceTable;memset(rawBuffers[creating],0xa5,sizeof(rawBuffers[creating]));new(buffers[creating]) CVertexBufferWin32;CHECK(buffers[creating]->Buffer==(IDirect3DVertexBuffer9*)resources[creating] && buffers[creating]->ByteSize==8400 && buffers[creating]->VertexStride==28 && buffers[creating]->VertexType==0x144);}CHECK(created==3 && systemCalls==3);phase=2;
 unsigned initCalls[2]={0,0};
 {CStaticFontBank font(0,0);CGlyph glyphs[64];memset(font.GlyphBanks,0,sizeof(font.GlyphBanks));font.PManager=manager;font.PTexture=fontTexture;font.MinChar=0;font.MaxChar=65535;font.TextureSize_X=64;font.TextureSize_Y=32;
  for(unsigned i=0;i<64;++i){memset(&glyphs[i],0xa5,sizeof(CGlyph));glyphs[i].Left=(i%7)*0.1f;glyphs[i].Right=glyphs[i].Left+0.07f;glyphs[i].Top=(i%5)*0.1f;glyphs[i].Bottom=glyphs[i].Top+0.07f;glyphs[i].Offset=(short)(int(i%5)-2);glyphs[i].Width=(short)(i%11);glyphs[i].Advance=(short)(int(i%13)-3);}
  for(unsigned i=0;i<1024;++i){font.GlyphBanks[i].FirstGlyph=0;font.GlyphBanks[i].NoGlyphs=64;font.GlyphBanks[i].Glyphs=glyphs;}
  wchar_t longText[122];for(unsigned i=0;i<121;++i)longText[i]=L'A'+i%26;longText[121]=0;
  wchar_t boundaryTexts[6][102];const unsigned boundaryLengths[6]={49,50,51,99,100,101};for(unsigned n=0;n<6;++n){for(unsigned i=0;i<boundaryLengths[n];++i)boundaryTexts[n][i]=L'A';boundaryTexts[n][boundaryLengths[n]]=0;}
  const wchar_t* texts[]={L"",L"A",L"ABC",L"A\nB",L"\n",L"\n\n",L"A\n\nB",L"\xfffe\xffff",boundaryTexts[0],boundaryTexts[1],boundaryTexts[2],boundaryTexts[3],boundaryTexts[4],boundaryTexts[5],longText};
  const unsigned flagSet[]={0,2,4,8,6};const float scales[]={1,0.5f,2};const unsigned heights[]={12,255};
  for(unsigned t=0;t<15;++t)for(unsigned f=0;f<5;++f)for(unsigned s=0;s<3;++s)for(unsigned h=0;h<2;++h)for(unsigned enabled=0;enabled<2;++enabled)for(unsigned useOverride=0;useOverride<2;++useOverride)for(unsigned cache=0;cache<3;++cache)for(unsigned stages=0;stages<2;++stages)for(unsigned history=0;history<2;++history){capsProfile=(t+f+s+h+enabled+useOverride+cache+stages+history)&1;
   font.FontHeight=heights[h];unsigned length=0;while(texts[t][length])++length;unsigned flags=flagSet[f];
   C3DVector position(13.25f,-7.75f,0.375f);CRGBColour colour(0x12,0x34,0x56,0x78);const CTexture* extra=useOverride?overrideTexture:0;
   // Every character in this glyph table is drawn, newline included (Render advances the line and still emits its quad).
   unsigned visibleGlyphs=length;
   __declspec(align(16)) unsigned char dataBytes[sizeof(CWideStringData)];CWideStringData* data=(CWideStringData*)dataBytes;memset(dataBytes,0,sizeof(dataBytes));data->SetText_(const_cast<wchar_t*>(texts[t]));data->SetFinish_((size_t)(texts[t]+length));data->SetEndOfStorage_((size_t)(texts[t]+length+1));data->owners=2;
   {CWideString text;if(length)*(CWideStringData**)&text=data;unsigned beforeFont[sizeof(font)/4+1];memcpy(beforeFont,&font,sizeof(font));static unsigned char referenceManager[sizeof(CRenderManager)],rowStart[sizeof(CRenderManager)];
    // Rows chain re-initialisations (Init keeps earlier bookmark masks, so only the first Init pushes the
    // state stack); both modes of a row start from the same manager bytes.
    memcpy(rowStart,renderStorage,sizeof(rowStart));
    for(mode=0;mode<(code?2:1);++mode){memcpy(renderStorage,rowStart,sizeof(rowStart));
     nevents=payloadUsed=0;driverStatus=(row&1)?0x80004005:0;selected=3;memset(locked,0,sizeof(locked));CFontManagerTestAccess::Reset();_clearfp();
     // Fresh state-manager initialisation: every registered state is resent once.
     capsCalls=0;render->CRenderStateManager::Init((IDirect3DDevice9*)device);CHECK(capsCalls==1);CHECK(CRenderStateManagerTestAccess::StateDevice()==device && CRenderStateManagerTestAccess::Dirty()==0 && CRenderStateManagerTestAccess::Registered()>0 && (unsigned)CRenderStateManagerTestAccess::Registered()==nevents);
     if(!initCalls[capsProfile])initCalls[capsProfile]=nevents;CHECK(initCalls[capsProfile]==nevents);for(unsigned i=0;i<nevents;++i)CHECK(events[i].kind>=E_RS && events[i].kind<=E_SAMPLER);
     render->TextureStagesInUse=stages?5:0;for(unsigned i=0;i<8;++i)render->CurrentlySelectedTextures[i]=0;if(cache==1)render->CurrentlySelectedTextures[0]=(IDirect3DBaseTexture9*)d3dTextures[0];if(cache==2){render->CurrentlySelectedTextures[0]=(IDirect3DBaseTexture9*)d3dTextures[1];render->CurrentlySelectedTextures[1]=(IDirect3DBaseTexture9*)d3dTextures[0];}
     for(pass=0;pass<2;++pass){
      unsigned start=nevents;systemCalls=locks=unlocks=0;if(history && !pass){CRenderStateManagerTestAccess::History();CHECK(CRenderStateManagerTestAccess::Dirty()==2 && nevents==start);}
      if(mode)(font.*originalPrepare)(enabled!=0);else font.CStaticFontBank::PrepareToRender(enabled!=0);
      unsigned afterPrepare=nevents;CHECK(systemCalls==1);for(unsigned i=start;i<afterPrepare;++i)CHECK(events[i].kind==E_SYSTEM || (events[i].kind==E_FVF && events[i].a==0x144));CHECK(afterPrepare-start==1+enabled);
      Expected expected;ExpectRealise(expected);const long dirtyAfterPrepare=CRenderStateManagerTestAccess::Dirty();if(pass && visibleGlyphs)CHECK(!expected.dirtyCalls);if(!pass)CHECK(expected.unchanged==(history?2u:0u));
      IDirect3DBaseTexture9* cacheBefore[2]={render->CurrentlySelectedTextures[0],render->CurrentlySelectedTextures[1]};
      if(mode)(font.*originalRender)(text,position,scales[s],colour,flags,extra);else font.Render(text,position,scales[s],colour,flags,extra);
      // Walk the device stream Render produced and check it against the model.
      unsigned draws=0,triangles=0,i=afterPrepare;long textureCount=useOverride?2:1;
      while(i<nevents){const DeviceEvent& e=events[i];
       if(e.kind==E_SYSTEM||e.kind==E_LOCK||e.kind==E_UNLOCK||e.kind==E_STREAM){++i;continue;}
       // A draw: optional texture binds, then (first draw only) the queued state flush, then one triangle list.
       for(long stage=0;stage<textureCount;++stage){IDirect3DBaseTexture9* want=(IDirect3DBaseTexture9*)d3dTextures[stage];if((draws?want:cacheBefore[stage])!=want){CHECK(events[i].kind==E_TEXTURE && events[i].a==(unsigned)stage && events[i].b==(unsigned)want);++i;}}
       if(!draws)for(unsigned k=0;k<expected.dirtyCalls;++k){CHECK(i<nevents && Same(events[i],expected.calls[k]));++i;}
       CHECK(i<nevents && events[i].kind==E_DRAW && events[i].a==4 && events[i].b==0 && events[i].c && events[i].c<=100 && !(events[i].c%2));triangles+=events[i].c;++draws;++i;}
      CHECK(draws==(visibleGlyphs+49)/50 && triangles==visibleGlyphs*2 && locks==unlocks && locks==visibleGlyphs/50+1 && systemCalls==2+locks && selected==3 && !locked[0] && !locked[1] && !locked[2]);
      if(draws){CHECK(CRenderStateManagerTestAccess::Dirty()==0 && render->CurrentlySelectedTextures[0]==(IDirect3DBaseTexture9*)d3dTextures[0] && render->TextureStagesInUse>=textureCount);for(long r=0;r<CRenderStateManagerTestAccess::Registered();++r){CRenderStateManager::CStateInfo* st=CRenderStateManagerTestAccess::Registered(r);CHECK(st->CurrentState==st->DesiredState && !st->DirtyListFlag);}if(useOverride)CHECK(render->CurrentlySelectedTextures[1]==(IDirect3DBaseTexture9*)d3dTextures[1]);}
      else CHECK(CRenderStateManagerTestAccess::Dirty()==dirtyAfterPrepare);
     }
     CHECK(!memcmp(beforeFont,&font,sizeof(font)) && references[0]==1 && references[1]==1 && references[2]==1);
     if(!mode){savedCount=nevents;memcpy(savedEvents,events,nevents*sizeof(DeviceEvent));savedPayloadUsed=payloadUsed;memcpy(savedPayload,payload,payloadUsed);memcpy(referenceManager,renderStorage,sizeof(referenceManager));}
     else{CHECK(savedCount==nevents);CHECK(savedPayloadUsed==payloadUsed);for(unsigned k=0;k<payloadUsed;++k)if(savedPayload[k]!=payload[k]){fprintf(stderr,"PAYLOAD_DIFF byte=%u candidate=%02x original=%02x\n",k,savedPayload[k],payload[k]);CHECK(false);}for(unsigned e=0;e<nevents;++e)if(!Same(events[e],savedEvents[e])){fprintf(stderr,"EVENT_DIFF index=%u candidate=%u,%x,%x,%x original=%u,%x,%x,%x\n",e,savedEvents[e].kind,savedEvents[e].a,savedEvents[e].b,savedEvents[e].c,events[e].kind,events[e].a,events[e].b,events[e].c);CHECK(false);}for(unsigned k=0;k<sizeof(referenceManager);++k)if(referenceManager[k]!=renderStorage[k]){fprintf(stderr,"MANAGER_DIFF offset=%x candidate=%02x original=%02x\n",k,referenceManager[k],renderStorage[k]);CHECK(false);}}
    }*(CWideStringData**)&text=0;
   }++row;
  }
 }
 phase=3;systemCalls=0;for(unsigned i=0;i<3;++i){CHECK(BufferScalar(buffers[i],0,0)==buffers[i] && !buffers[i]->Buffer && !references[i]);}CHECK(created==released && released==3 && systemCalls==3);
 CHECK(!g_CCharStringInstanceCount_013BD800 && !g_CWideStringInstanceCount_013BCA20);_controlfp(savedControl,_MCW_PC|_MCW_RC);if(code)CHECK(VirtualFree(code,0,0x8000));CHECK(initCalls[1]==initCalls[0]+2+8);printf("STATIC_FONT_DRAW_PASS cases=%u paths=%u init_states=%u,%u\n",row,row*(code?2:1),initCalls[0],initCalls[1]);return 0;
}
