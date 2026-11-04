// UI text frame: actual Draw2DText/Draw3DScaledText/AddText enqueue, then actual
// CRenderManager2D::Render2DDrawList flush driving the actual static font chain (PrepareToRender, Render,
// Win32 vertex buffers, DrawCurrentBuffersAsTris, RealiseRenderState), state-block library,
// AttachTextureToStage and RestoreCaptureBlock against a recording fake device. With two arguments the
// retail Render2DDrawList/Render/Prepare/Draw/Realise bytes run as one transplanted graph (the font is
// reached through a vtable copy whose Prepare/Render slots point into it) and every device, shader and
// viewport event, vertex payload byte and manager/shader byte must match the recreated graph.
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_font_instances.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "fable_shader_resource.h"
#include "fable_memory_allocator.h"
#include "fable_font_vertex_buffer.h"
#include "fable_render_state.h"
#include "fable_2d_vector.h"
#include "fable_rgb_colour.h"
#include "fable_font_data_banks.h"
#include "fable_data_input_stream.h"
#include "fable_surface.h"
#define FABLE_STUBS_NO_MEMORY_DATA_INPUT_STREAM
#include "test_support/data_input_stream_stubs.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static unsigned row,mode,frame,locks,unlocks,selected,phase,creating,created,released,systemCalls,nevents,payloadUsed,capsCalls;
struct DeviceEvent{unsigned kind,a,b,c,d;};
static DeviceEvent events[8192],savedEvents[8192];static unsigned savedCount,savedPayloadUsed;
static unsigned char payload[16*8400],savedPayload[16*8400];
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"UI_TEXT_FAILURE row=%u mode=%u frame=%u line=%d\n",row,mode,frame,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
enum{E_SYSTEM=1,E_LOCK,E_UNLOCK,E_STREAM,E_FVF,E_TEXTURE,E_RS,E_TSS,E_SAMPLER,E_DRAW,E_DRAWUP,E_VIEWPORT,E_DECL,E_VS,E_PS,E_VSCONST,E_PSCONST};
static void Event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){CHECK(nevents<8192);DeviceEvent e={kind,a,b,c,d};events[nevents++]=e;}
fable_i32 g_CCharStringInstanceCount_013BD800=0,g_CWideStringInstanceCount_013BCA20=0;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};char g_FableEmptyNarrowString_0129AAF4[1]={0};
// Retail 0x0139652C = 100.0f (gate checks the image bytes): RHW clamp used by SetRHWFromZ near z=0.
float g_FableVertexReciprocalLimit_0139652C=100.0f;
extern "C" {float FontHalf=0.5f,FontOne=1.0f,FontUnsignedBias=4294967296.0f;}
#pragma comment(linker,"/include:_FontHalf")
#pragma comment(linker,"/include:_FontOne")
#pragma comment(linker,"/include:_FontUnsignedBias")
// Actual shader-manager global (constructed by the CRT) and 2D constant layout global; the frame drives
// their real methods. Only shader resources (the 2D vertex shader's device handles) are fixture data.
static CShaderRenderManager& shader=FableShaderRenderManager_013bc470;
static unsigned char* const ShaderStorage=(unsigned char*)&FableShaderRenderManager_013bc470;
enum{SHADER_BYTES=sizeof(CShaderRenderManager)};
struct CVertexShaderTestAccess{static void Bind(CVertexShader& s,CVertexShaderData* d,void* declaration,void* code){d->VertexDeclaration=(IDirect3DVertexDeclaration9*)declaration;d->VertexShader=(IDirect3DVertexShader9*)code;d->FormatDecl=0;d->ReferenceCount=1000000;s.Data=d;}};
__declspec(align(16)) static unsigned char vertexShaderData[sizeof(CVertexShaderData)];static unsigned char shaderHandles[2][16];
// Viewport boundary: retail SetViewport(C2DBoxF) is an inline-asm fistp conversion feeding the integer
// endpoint; the fixture only uses integral windows, records them and mirrors the float box.
__declspec(align(16)) static unsigned char systemStorage[sizeof(CSystemManager)],displayStorage[sizeof(CDisplayManager)],renderStorage[sizeof(CRenderManager)],managerStorage[sizeof(CFontManager)];
static CSystemManager* fixtureSystem=(CSystemManager*)systemStorage;
static CDisplayManager* display=(CDisplayManager*)displayStorage;
static CRenderManager* render=(CRenderManager*)renderStorage;
static CFontManager* manager=(CFontManager*)managerStorage;
void CDisplayManager::SetViewport(const C2DBoxF& box){CHECK(this==display);const float* v=&box.left;for(unsigned i=0;i<4;++i)CHECK(v[i]==(float)(long)v[i]);Event(E_VIEWPORT,(unsigned)(long)box.left,(unsigned)(long)box.top,(unsigned)(long)box.right,(unsigned)(long)box.bottom);ViewportBoxFloat=box;}
// Cold symbols pulled in by linked translation units; this frame never reaches them.
void CDisplayManager::SetIntegerViewportEndpoint(const C2DBoxI&){CHECK(false);}
double FABLE_FASTCALL GFGetTime(){CHECK(false);return 0;}
namespace NVectorMath{void __cdecl GFClear4DVectors(C4DVector*,unsigned long,const C4DVector&){CHECK(false);}}
// Shader resources are a boundary (the manager owns none here) and light pools stay unallocated.
void CShaderResource::Initialise(CShaderRenderManager&){CHECK(false);}
void CShaderResource::Uninitialise(){CHECK(false);}
void* FABLE_FASTCALL CMemoryAllocator::AllocateAligned(unsigned long,long){CHECK(false);return 0;}
void FABLE_FASTCALL CMemoryAllocator::DeallocateAligned(void*){CHECK(false);}
// Font loading: the actual CStaticFontBank constructor reads a generated bank entry (name, metrics, two
// 64-glyph banks, A8R8G8B8 pixels), negotiates DXT3, creates and uploads its texture; the destructor
// releases it. Bank file, D3D and D3DX are adapters; the font manager assignment (PManager) is a boundary.
__declspec(align(16)) static unsigned char bankStorage[sizeof(CFontDataBank)];
static unsigned char fontPayload[2][40000];static unsigned fontPayloadSize[2],loadingFont,fontWidth[2]={64,128},fontHeight[2]={32,64};
unsigned long CBankFile::GetEntryDataSize(unsigned long index)const{CHECK(this==(const CBankFile*)bankStorage && (index==0x40 || index==0x41));loadingFont=index-0x40;return fontPayloadSize[loadingFont];}
void CBankFile::ReadEntryData(unsigned long index,unsigned long offset,void* data,unsigned long size){CHECK(this==(CBankFile*)bankStorage && index==0x40+loadingFont && !offset && size==fontPayloadSize[loadingFont]);memcpy(data,fontPayload[loadingFont],size);}
static void* textureDevice;static void* d3dTable[17];static void* d3dObject[1]={d3dTable};static void* textureTable[22];static void* surfaceTable[13];
static void* textureObjects[2][1]={{textureTable},{textureTable}};static void* surfaceObjects[2][1]={{surfaceTable},{surfaceTable}};static unsigned textureRefs[2],surfaceRefs[2],uploads[2];
static unsigned TextureIndex(void* self){for(unsigned i=0;i<2;++i)if(self==textureObjects[i])return i;CHECK(false);return 0;}
static unsigned SurfaceIndex(void* self){for(unsigned i=0;i<2;++i)if(self==surfaceObjects[i])return i;CHECK(false);return 0;}
static HRESULT __stdcall CheckDeviceFormat(void* self,UINT adapter,D3DDEVTYPE type,D3DFORMAT current,DWORD usage,D3DRESOURCETYPE resource,D3DFORMAT format){CHECK(self==d3dObject && phase==1 && adapter==7 && type==D3DDEVTYPE_HAL && current==D3DFMT_X8R8G8B8 && !usage && resource==D3DRTYPE_TEXTURE && format==D3DFMT_DXT3);return 0;}
static HRESULT __stdcall CreateTexture(void* self,UINT width,UINT height,UINT levels,DWORD usage,D3DFORMAT format,D3DPOOL pool,IDirect3DTexture9** output,HANDLE* shared){unsigned n=loadingFont;CHECK(self==textureDevice && phase==1 && width==fontWidth[n] && height==fontHeight[n] && levels==1 && !usage && format==D3DFMT_DXT3 && pool==D3DPOOL_MANAGED && !shared && output && !textureRefs[n]);*output=(IDirect3DTexture9*)textureObjects[n];textureRefs[n]=1;return 0;}
static unsigned long __stdcall TextureRelease(void* self){unsigned n=TextureIndex(self);CHECK(phase==3 && textureRefs[n]==1);return --textureRefs[n];}
static DWORD __stdcall GetLevelCount(void* self){TextureIndex(self);CHECK(phase==1);return 1;}
static HRESULT __stdcall GetLevelDesc(void* self,UINT level,D3DSURFACE_DESC* description){unsigned n=TextureIndex(self);CHECK(phase==1 && !level && description);memset(description,0,sizeof(*description));description->Format=D3DFMT_DXT3;description->Width=fontWidth[n];description->Height=fontHeight[n];return 0;}
static HRESULT __stdcall GetSurfaceLevel(void* self,UINT level,IDirect3DSurface9** output){unsigned n=TextureIndex(self);CHECK(phase==1 && !level && !surfaceRefs[n] && output);*output=(IDirect3DSurface9*)surfaceObjects[n];surfaceRefs[n]=1;return 0;}
static unsigned long __stdcall SurfaceAddRef(void* self){unsigned n=SurfaceIndex(self);CHECK(phase==1 && surfaceRefs[n]);return ++surfaceRefs[n];}
static unsigned long __stdcall SurfaceRelease(void* self){unsigned n=SurfaceIndex(self);CHECK(phase==1 && surfaceRefs[n]);return --surfaceRefs[n];}
static HRESULT __stdcall GetSurfaceDesc(void* self,D3DSURFACE_DESC* description){unsigned n=SurfaceIndex(self);CHECK(phase==1 && description);memset(description,0,sizeof(*description));description->Format=D3DFMT_DXT3;description->Width=fontWidth[n];description->Height=fontHeight[n];return 0;}
extern "C" HRESULT __stdcall D3DXLoadSurfaceFromMemory(IDirect3DSurface9* surface,const PALETTEENTRY* dp,const RECT* dest,const void* source,D3DFORMAT format,UINT pitch,const PALETTEENTRY* sp,const RECT* box,DWORD filter,D3DCOLOR key){unsigned n=SurfaceIndex(surface);CHECK(phase==1 && !dp && !sp && !key && dest && box && dest->right==(long)fontWidth[n] && dest->bottom==(long)fontHeight[n] && format==D3DFMT_A8R8G8B8 && pitch==fontWidth[n]*4 && filter==0xffffffff);for(unsigned i=0;i<fontWidth[n]*fontHeight[n]*4;++i)CHECK(((const unsigned char*)source)[i]==(unsigned char)(n*7+i*13));++uploads[n];return 0;}
void CSurface::CopyFromMemory(const C2DBoxI*,const void*,const C2DBoxI*,long){CHECK(false);}
void CSurface::CopyFromUnsignedDataToSignedSurface(const void*,const CPixelFormat&,long){CHECK(false);}
void CDataInputStream::SetPosition(unsigned long){CHECK(false);}CFileDataInputStream::~CFileDataInputStream(){CHECK(false);}template<>void _STL::vector<char>::resize(unsigned,const char&){CHECK(false);}
static void Append(unsigned n,const void* data,unsigned size){CHECK(fontPayloadSize[n]+size<=sizeof(fontPayload[n]));memcpy(fontPayload[n]+fontPayloadSize[n],data,size);fontPayloadSize[n]+=size;}
static void Word(unsigned n,unsigned value){Append(n,&value,4);}
static void MakeFontPayloads(){for(unsigned n=0;n<2;++n){fontPayloadSize[n]=0;const char* name=n?"UIFontLarge":"UIFontSmall";Append(n,name,(unsigned)strlen(name)+1);Word(n,n?18:12);Word(n,400);unsigned char italics=0;Append(n,&italics,1);Word(n,n?20:14);Word(n,fontWidth[n]);Word(n,fontHeight[n]);Word(n,0);Word(n,65535);Word(n,2);
 for(unsigned b=0;b<2;++b){Word(n,b);Word(n,0x10000);Word(n,0x10000+64);for(unsigned i=0;i<64;++i){float floats[4];floats[0]=(i%7)*0.1f;floats[1]=(i%5)*0.1f;floats[2]=floats[0]+0.07f;floats[3]=floats[1]+0.07f;Append(n,floats,16);short shorts[3]={(short)(int(i%5)-2),(short)(i%11+n),(short)(int(i%13)-3)};Append(n,shorts,6);}}
 const unsigned pixels=fontWidth[n]*fontHeight[n]*4;Word(n,pixels+18);unsigned char header[18];memset(header,0xf1,18);Append(n,header,18);for(unsigned k=0;k<pixels;++k){unsigned char v=(unsigned char)(n*7+k*13);Append(n,&v,1);}}}
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){++systemCalls;Event(E_SYSTEM);return fixtureSystem;}
__declspec(align(16)) static unsigned char rawBuffers[3][sizeof(CVertexBufferWin32)];
static CVertexBufferWin32* buffers[3]={(CVertexBufferWin32*)rawBuffers[0],(CVertexBufferWin32*)rawBuffers[1],(CVertexBufferWin32*)rawBuffers[2]};
static unsigned char gpuBytes[3][8432];static bool locked[3];static void* resourceTable[14];static void* resources[3][1];static unsigned references[3];
static void* deviceTable[119];static void* device[1]={deviceTable};
__declspec(align(16)) static unsigned char textureStorage[2][sizeof(CTexture)];static unsigned char d3dTextures[2][16];
static unsigned Index(void* self){for(unsigned i=0;i<3;++i)if(self==resources[i])return i;CHECK(false);return 0;}
static HRESULT __stdcall Lock(void* self,unsigned offset,unsigned bytes,void** output,unsigned flags){unsigned i=Index(self);CHECK(phase==2 && selected==3 && !locked[i] && flags==0 && offset==0 && bytes==8400 && output && !*output && references[i]==1);locked[i]=true;++locks;memset(gpuBytes[i],0xcd,sizeof(gpuBytes[i]));Event(E_LOCK,i);*output=gpuBytes[i]+16;return 0;}
static HRESULT __stdcall Unlock(void* self){unsigned i=Index(self);CHECK(phase==2 && locked[i] && references[i]==1);locked[i]=false;++unlocks;for(unsigned n=0;n<16;++n)CHECK(gpuBytes[i][n]==0xcd && gpuBytes[i][8416+n]==0xcd);Event(E_UNLOCK,i);CHECK(payloadUsed+8400<=sizeof(payload));memcpy(payload+payloadUsed,gpuBytes[i]+16,8400);payloadUsed+=8400;return 0;}
static unsigned long __stdcall Release(void* self){unsigned i=Index(self);CHECK(phase==3 && references[i]==1 && !locked[i]);references[i]=0;++released;return 0;}
static HRESULT __stdcall Create(void* self,unsigned bytes,unsigned flags,unsigned format,D3DPOOL pool,IDirect3DVertexBuffer9** output,HANDLE* shared){CHECK(phase==1 && creating<3 && self==device && bytes==8400 && flags==8 && format==0x144 && (unsigned)pool==1 && !shared && output==&buffers[creating]->Buffer && !*output && !references[creating]);*output=(IDirect3DVertexBuffer9*)resources[creating];references[creating]=1;++created;return 0;}
extern "C" void* __fastcall BufferScalar(CVertexBufferWin32*,void*,unsigned);
#pragma comment(linker,"/alternatename:@BufferScalar@12=??_GCVertexBufferWin32@@UAEPAXI@Z")
static HRESULT __stdcall SetStream(void* self,unsigned stream,IDirect3DVertexBuffer9* resource,unsigned offset,unsigned stride){CHECK(self==device && !stream && !offset);if(!resource){CHECK(!stride);selected=3;Event(E_STREAM,3);return 0;}for(unsigned i=0;i<3;++i)if(resource==(IDirect3DVertexBuffer9*)resources[i]){CHECK(!locked[i] && stride==28);selected=i;Event(E_STREAM,i,stride);return 0;}CHECK(false);return 0;}
static HRESULT __stdcall SetFVF(void* self,unsigned format){CHECK(self==device && phase==2);Event(E_FVF,format);return 0;}
static HRESULT __stdcall SetTexture(void* self,unsigned stage,void* texture){CHECK(self==device && phase==2 && stage<2 && (texture==textureObjects[0] || texture==textureObjects[1] || !texture));Event(E_TEXTURE,stage,(unsigned)texture);return 0;}
static HRESULT __stdcall SetRS(void* self,unsigned state,unsigned value){CHECK(self==device);Event(E_RS,state,value);return 0;}
static HRESULT __stdcall SetTSS(void* self,unsigned stage,unsigned state,unsigned value){CHECK(self==device && stage<8);Event(E_TSS,stage,state,value);return 0;}
static HRESULT __stdcall SetSampler(void* self,unsigned stage,unsigned state,unsigned value){CHECK(self==device && stage<8);Event(E_SAMPLER,stage,state,value);return 0;}
static HRESULT __stdcall SetDeclaration(void* self,void* declaration){CHECK(self==device && declaration==shaderHandles[0]);Event(E_DECL,(unsigned)declaration);return 0;}
static HRESULT __stdcall SetVertexShader(void* self,void* code){CHECK(self==device && (code==shaderHandles[1] || !code));Event(E_VS,(unsigned)code);return 0;}
static HRESULT __stdcall SetPixelShader(void* self,void* code){CHECK(self==device && !code);Event(E_PS,(unsigned)code);return 0;}
// Shader constants: register range in the event, the float4 values in the compared payload.
static void Constants(unsigned kind,unsigned startRegister,const void* data,unsigned count){CHECK(count<=256 && (data || !count));Event(kind,startRegister,count);unsigned bytes=count*16;CHECK(payloadUsed+bytes<=sizeof(payload));memcpy(payload+payloadUsed,data,bytes);payloadUsed+=bytes;}
static HRESULT __stdcall SetVSConstants(void* self,unsigned startRegister,const void* data,unsigned count){CHECK(self==device);Constants(E_VSCONST,startRegister,data,count);return 0;}
static HRESULT __stdcall SetPSConstants(void* self,unsigned startRegister,const void* data,unsigned count){CHECK(self==device);Constants(E_PSCONST,startRegister,data,count);return 0;}
// Every other device slot fails with its index.
template<int N> struct UnexpectedSlot{static HRESULT __stdcall Call(void*){fprintf(stderr,"UNEXPECTED_DEVICE_SLOT %d row=%u mode=%u\n",N,row,mode);fflush(stderr);ExitProcess(80);return 0;}};
template<int N> struct FillSlots{static void Run(void** table){if(!table[N])table[N]=(void*)&UnexpectedSlot<N>::Call;FillSlots<N-1>::Run(table);}};
template<> struct FillSlots<-1>{static void Run(void**){}};
static HRESULT __stdcall GetCaps(void* self,D3DCAPS9* caps){CHECK(self==device && caps);memset(caps,0xa5,sizeof(*caps));caps->RasterCaps=0x06002000;++capsCalls;return 0;}
static HRESULT __stdcall DrawPrimitive(void* self,unsigned type,unsigned start,unsigned count){CHECK(self==device && phase==2 && selected<3 && !locked[selected]);Event(E_DRAW,type,start,count,selected);return 0;}
// Geometry batches: the vertex pointer is a heap address, so the event keeps type/count/stride and the
// vertices themselves join the compared payload. Draw2DTriangle/Draw2DBoxWithZ never write the specular
// colour (+20), which carries stack residue in retail and here, so those four bytes are masked.
static HRESULT __stdcall DrawPrimitiveUP(void* self,unsigned type,unsigned count,const void* data,unsigned stride){CHECK(self==device && phase==2 && (type==2 || type==4) && count && stride==32 && data);Event(E_DRAWUP,type,count,stride);unsigned bytes=count*(type==2?2:3)*stride;CHECK(payloadUsed+bytes<=sizeof(payload));memcpy(payload+payloadUsed,data,bytes);for(unsigned v=0;v<bytes;v+=32)memset(payload+payloadUsed+v+20,0,4);payloadUsed+=bytes;return 0;}
struct CFontManagerTestAccess{static void Reset(){manager->VertexBuffers._M_start=buffers;manager->VertexBuffers._M_finish=buffers+3;manager->VertexBuffers._M_end_of_storage._M_data=buffers+3;manager->CurrentVB=0;}};
struct CRenderStateManagerTestAccess{
 static long Registered(){return render->AllStatesListSize;}
 static long Dirty(){return render->StateUpdateListSize;}
 static long Stack(){return render->StateStackSize;}
 static unsigned long Mask(){return render->CurrentBookmarkMask;}
 static CRenderStateManager::CStateInfo* Registered(long i){return render->AllStatesList[i];}
};
// Retail Render(0xAB7C20), PrepareToRender(0xAB83A0), DrawCurrentBuffersAsTris(0xA0ABE0), RealiseRenderState
// (0xA058C0) and Render2DDrawList(0x9DA9F0) in one block; patch kinds: 0 absolute, 1 rel32 to an absolute
// fixture address, 2 rel32 to an offset inside the block.
enum{RENDER_AT=0,PREPARE_AT=2048,DRAW_AT=4864,REALISE_AT=5120,LIST_AT=5376,BLOCK=12288}; // retail shader functions follow the list
static unsigned char* Original(const char* name,const char* patchName){FILE* input=fopen(name,"rb");CHECK(input);unsigned char* code=(unsigned char*)VirtualAlloc(0,16384,0x3000,0x40);CHECK(code && fread(code,1,BLOCK,input)==BLOCK && fgetc(input)==EOF && !fclose(input));input=fopen(patchName,"rb");unsigned count;CHECK(input && fread(&count,4,1,input)==1);for(unsigned i=0;i<count;++i){unsigned patch[3];CHECK(fread(patch,4,3,input)==3 && patch[0]<=BLOCK-4 && patch[2]<=2);unsigned target=patch[2]==2?(unsigned)(code+patch[1]):patch[1];*(unsigned*)(code+patch[0])=target-(patch[2]?(unsigned)(code+patch[0]+4):0);}CHECK(fgetc(input)==EOF && !fclose(input));return code;}
static bool Same(const DeviceEvent& a,const DeviceEvent& b){return a.kind==b.kind && a.a==b.a && a.b==b.b && a.c==b.c && a.d==b.d;}
enum{K_TEXT,K_TRI,K_BOX,K_LINE};
// Text: font selects the bank, scaled uses Draw3DScaledText. Geometry: font carries the blend flags
// (0 solid, 2 additive, 1 alpha-if-translucent) and scaled makes the colour translucent.
struct Item{unsigned kind,text,font,window,scaled,offscreen;};
int main(int argc,char** argv){CHECK(argc==1 || argc==3);unsigned char* code=argc==3?Original(argv[1],argv[2]):0;
 typedef void(CRenderManager2D::*ListMethod)(bool);ListMethod originalList;CHECK(sizeof(originalList)==4);unsigned char* listCode=code?code+LIST_AT:0;memcpy(&originalList,&listCode,4);
 unsigned savedControl=_controlfp(0,0);_controlfp(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);
 memset(systemStorage,0,sizeof(systemStorage));memset(displayStorage,0,sizeof(displayStorage));memset(renderStorage,0,sizeof(renderStorage));memset(managerStorage,0,sizeof(managerStorage));fixtureSystem->PDisplayManager.Data=display;CRenderManager* rp=render;memcpy(&display->PRenderManager,&rp,4);display->D3DDevice=(IDirect3DDevice9*)device;display->D3D=(IDirect3D9*)d3dObject;display->D3DDeviceCaps.AdapterOrdinal=7;display->D3DDeviceCaps.DeviceType=D3DDEVTYPE_HAL;display->CurrentMode.Format=D3DFMT_X8R8G8B8;systemStorage[4]=1;
 display->D3DDeviceCaps.PixelShaderVersion=0xffff0200;display->RenderTargetDimensionsStorage_[0]=640;display->RenderTargetDimensionsStorage_[1]=480;render->DisplayManager=display;
 deviceTable[7]=(void*)&GetCaps;deviceTable[23]=(void*)&CreateTexture;textureDevice=device;d3dTable[10]=(void*)&CheckDeviceFormat;textureTable[2]=(void*)&TextureRelease;textureTable[13]=(void*)&GetLevelCount;textureTable[17]=(void*)&GetLevelDesc;textureTable[18]=(void*)&GetSurfaceLevel;surfaceTable[1]=(void*)&SurfaceAddRef;surfaceTable[2]=(void*)&SurfaceRelease;surfaceTable[12]=(void*)&GetSurfaceDesc;deviceTable[26]=(void*)&Create;deviceTable[57]=(void*)&SetRS;deviceTable[65]=(void*)&SetTexture;deviceTable[67]=(void*)&SetTSS;deviceTable[69]=(void*)&SetSampler;deviceTable[81]=(void*)&DrawPrimitive;deviceTable[83]=(void*)&DrawPrimitiveUP;deviceTable[87]=(void*)&SetDeclaration;deviceTable[89]=(void*)&SetFVF;deviceTable[92]=(void*)&SetVertexShader;deviceTable[94]=(void*)&SetVSConstants;deviceTable[100]=(void*)&SetStream;deviceTable[107]=(void*)&SetPixelShader;deviceTable[109]=(void*)&SetPSConstants;FillSlots<118>::Run(deviceTable);
 resourceTable[2]=(void*)&Release;resourceTable[11]=(void*)&Lock;resourceTable[12]=(void*)&Unlock;
 memset(textureStorage,0,sizeof(textureStorage));((CTexture*)textureStorage[0])->PD3DTexture=(IDirect3DTexture9*)d3dTextures[0];((CTexture*)textureStorage[1])->PD3DTexture=(IDirect3DTexture9*)d3dTextures[1];
 render->PD3DDevice=(IDirect3DDevice9*)device;
 phase=1;for(creating=0;creating<3;++creating){resources[creating][0]=resourceTable;memset(rawBuffers[creating],0xa5,sizeof(rawBuffers[creating]));new(buffers[creating]) CVertexBufferWin32;CHECK(buffers[creating]->Buffer==(IDirect3DVertexBuffer9*)resources[creating]);}CHECK(created==3);phase=2;
 // State-manager initialisation once; frames chain from it as in the running game.
 capsCalls=0;render->CRenderStateManager::Init((IDirect3DDevice9*)device);CHECK(capsCalls==1 && CRenderStateManagerTestAccess::Dirty()==0 && CRenderStateManagerTestAccess::Mask()==1);
 // Actual shader-manager initialisation against this renderer and device, and the 2D vertex shader's handles.
 shader.Initialise(render,(IDirect3DDevice9*)device);CHECK(shader.CurrentVSConstantLayoutIndex==-1 && !shader.VertexShadersEnabled && !shader.PixelShadersEnabled && !shader.updateFlags3D8);
 CVertexShaderTestAccess::Bind(render->VertexShader2D,(CVertexShaderData*)vertexShaderData,shaderHandles[0],shaderHandles[1]);
 {// Both fonts load through the actual constructor (phase 1: texture creation and upload happen here).
  MakeFontPayloads();CStaticFontBank* fonts[2];const unsigned savedPhase=phase;phase=1;
  for(unsigned n=0;n<2;++n){fonts[n]=new CStaticFontBank((CFontDataBank*)bankStorage,0x40+n);CHECK(fonts[n]->PTexture && fonts[n]->PTexture->PD3DTexture==(IDirect3DTexture9*)textureObjects[n] && textureRefs[n]==1 && !surfaceRefs[n] && uploads[n]==1);
   CHECK(fonts[n]->FontHeight==(n?18:12) && fonts[n]->TextureSize_X==(long)fontWidth[n] && fonts[n]->TextureSize_Y==(long)fontHeight[n] && fonts[n]->MinChar==0 && fonts[n]->MaxChar==65535 && fonts[n]->GlyphBanks[0].NoGlyphs==64 && fonts[n]->GlyphBanks[1].NoGlyphs==64 && !fonts[n]->GlyphBanks[2].Glyphs);
   fonts[n]->PManager=manager;}
  phase=savedPhase;
  // Char-string baseline: each font's name plus the shader manager's nine pixel-shader constant names,
  // held once in the CIndexedArray vector and once as its index-map keys.
  const long charBaseline=g_CCharStringInstanceCount_013BD800;if(charBaseline!=20)fprintf(stderr,"CHAR_BASELINE %ld\n",charBaseline);CHECK(charBaseline==20);
  // Retail-mode font vtable: Prepare (slot 3) and Render (slot 5) enter the transplanted block.
  void* originalVtable[6];void* recreatedVtable=*(void**)fonts[0];memcpy(originalVtable,recreatedVtable,sizeof(originalVtable));if(code){originalVtable[3]=code+PREPARE_AT;originalVtable[5]=code+RENDER_AT;}
  wchar_t longText[61];for(unsigned i=0;i<60;++i)longText[i]=L'a'+i%26;longText[60]=0;
  const wchar_t* texts[]={L"",L"Quest",L"A\nB",longText,L"Gold 100"};unsigned lengths[5];for(unsigned t=0;t<5;++t){lengths[t]=0;while(texts[t][lengths[t]])++lengths[t];}
  const C2DBoxF windows[3]={C2DBoxF(0,0,640,480),C2DBoxF(32,16,320,240),C2DBoxF(100,50,600,400)};
  // Frame shapes: (kind,text,font|flags,window,scaled|translucent,offscreen) records.
  const Item shapes[][6]={
   {{K_TEXT,1,0,0,0,0}},
   {{K_TEXT,1,0,0,0,0},{K_TEXT,4,0,0,1,0}},
   {{K_TEXT,1,0,0,0,0},{K_TEXT,4,1,1,0,0}},
   {{K_TEXT,3,1,2,1,0},{K_TEXT,2,0,2,0,0},{K_TEXT,0,0,0,0,0}},
   {{K_TEXT,4,0,0,0,0},{K_TEXT,1,0,0,0,1},{K_TEXT,3,1,1,1,0},{K_TEXT,2,1,0,0,0}},
   {{K_BOX,0,0,0,0,0},{K_TEXT,1,0,0,0,0},{K_BOX,0,0,0,0,0}},
   {{K_TRI,0,2,0,0,0},{K_TRI,0,2,0,0,0},{K_BOX,0,1,0,1,0},{K_TEXT,4,1,0,0,0},{K_LINE,0,0,0,0,0}},
   {{K_LINE,0,0,2,0,0},{K_LINE,0,1,2,1,0},{K_TEXT,3,1,2,0,0},{K_TRI,0,0,1,0,0},{K_TEXT,0,0,1,1,0}},
   {{K_BOX,0,0,1,0,0},{K_BOX,0,0,2,0,0},{K_TEXT,1,0,2,0,0},{K_LINE,0,2,2,0,1},{K_TRI,0,0,0,0,1},{K_TEXT,4,0,0,1,0}},
   {{K_BOX,0,0,0,0,0},{K_TRI,0,1,0,1,0},{K_LINE,0,2,1,0,0}},
  };const unsigned shapeCounts[]={1,2,2,3,4,3,5,5,6,3};
  for(unsigned shapeIndex=0;shapeIndex<10;++shapeIndex)for(unsigned depth=0;depth<2;++depth)for(unsigned shaders=0;shaders<4;++shaders)for(unsigned frames=1;frames<=3;++frames){
   static unsigned char rowRender[sizeof(CRenderManager)],rowShader[SHADER_BYTES],rowDisplay[sizeof(CDisplayManager)],referenceRender[sizeof(CRenderManager)],referenceShader[SHADER_BYTES];
   memcpy(rowRender,renderStorage,sizeof(rowRender));memcpy(rowDisplay,displayStorage,sizeof(rowDisplay));
   // Initial shader enables through the actual methods (pixel shaders exist: ps_2_0 caps).
   if(shaders&1)shader.EnableVertexShaders();else shader.DisableVertexShaders();if(shaders&2)shader.EnablePixelShaders();else shader.DisablePixelShaders();CHECK(shader.VertexShadersEnabled==((shaders&1)!=0) && shader.PixelShadersEnabled==((shaders&2)!=0));memcpy(rowShader,ShaderStorage,sizeof(rowShader));
   for(mode=0;mode<(code?2:1);++mode){
    memcpy(renderStorage,rowRender,sizeof(rowRender));memcpy(ShaderStorage,rowShader,sizeof(rowShader));memcpy(displayStorage,rowDisplay,sizeof(rowDisplay));
    // The 2D queues are empty between frames; start each mode without storage so a restored snapshot never
    // refers to a block the other mode's growth already released.
    memset(renderStorage+0x3e58,0,0x3ea0-0x3e58);
    for(unsigned n=0;n<2;++n)*(void**)fonts[n]=mode?(void*)originalVtable:recreatedVtable;
    nevents=payloadUsed=0;selected=3;memset(locked,0,sizeof(locked));CFontManagerTestAccess::Reset();_clearfp();
    for(frame=0;frame<frames;++frame){
     const Item* items=shapes[shapeIndex];unsigned queued=0,records=0,queuedTris=0,queuedLines=0,expectedDraws=0,expectedLocks=0;
     // Enqueue through the public 2D API, each record under its own current viewport.
     for(unsigned k=0;k<shapeCounts[shapeIndex];++k){const Item& item=items[k];display->ViewportOffscreen=item.offscreen!=0;display->ViewportBoxFloat=windows[item.window];bool accepted;
      if(item.kind==K_TEXT){CWideString text(texts[item.text]);CRGBColour colour(0x10*k,0x20,0x30,0xff);
       accepted=item.scaled?render->Draw3DScaledText(C3DVector(40.0f*k+8,30.0f*k+4,0.25f*k),1.5f,text,*fonts[item.font],colour,0x80000000|k):render->Draw2DText(C2DVector(40.0f*k+8,30.0f*k+4),text,*fonts[item.font],colour,k);
       if(!item.offscreen){++queued;++records;expectedDraws+=(lengths[item.text]+49)/50;expectedLocks+=lengths[item.text]/50+1;}}
      else{CRGBColour colour(0x40,0x80+k,0xc0,item.scaled?0x80:0xff);const float a=3000.0f+1000*k,c=30000.0f+500*k;
       if(item.kind==K_TRI)accepted=render->Draw2DTriangle(C2DVector(a,2000),C2DVector(c,4000),C2DVector(a,40000),colour,item.font);
       else if(item.kind==K_BOX)accepted=render->Draw2DBox(C2DBoxF(a,2000,c,40000),colour,item.font);
       else accepted=render->Draw2DLine(C2DVector(a,2000),C2DVector(c,40000),colour,item.font);
       if(!item.offscreen){unsigned n=item.kind==K_BOX?2:1;records+=n;if(item.kind==K_LINE)++queuedLines;else queuedTris+=n;}}
      CHECK(accepted);}
     display->ViewportOffscreen=false;CHECK(render->QuickDraw2DTriInfo.size()==records && render->QuickDraw2DVertices.size()==queuedTris*3+queuedLines*2);
     const long stackBefore=CRenderStateManagerTestAccess::Stack();const unsigned long maskBefore=CRenderStateManagerTestAccess::Mask();unsigned long desiredBefore[256],bookmarkBefore[256];const long registered=CRenderStateManagerTestAccess::Registered();
     for(long r=0;r<registered;++r){desiredBefore[r]=CRenderStateManagerTestAccess::Registered(r)->DesiredState;bookmarkBefore[r]=CRenderStateManagerTestAccess::Registered(r)->BookmarkMask;}
     const unsigned start=nevents;systemCalls=locks=unlocks=0;const bool vsBefore=shader.VertexShadersEnabled,psBefore=shader.PixelShadersEnabled;
     if(mode)(render->*originalList)(depth!=0);else render->Render2DDrawList(depth!=0);
     // Model: one Prepare(true) per queued text (one SetFVF each), the font's own draws, and when anything
     // was queued the stage-0 cache cleared at the end. Capture block restores every desired value.
     unsigned fvf=0,draws=0,triangles=0,viewports=0,upTris=0,upLines=0,textures=0;for(unsigned e=start;e<nevents;++e){if(events[e].kind==E_FVF){CHECK(events[e].a==0x144);++fvf;}if(events[e].kind==E_DRAW){++draws;triangles+=events[e].c;CHECK(events[e].a==4 && !events[e].b);}if(events[e].kind==E_VIEWPORT)++viewports;if(events[e].kind==E_TEXTURE)++textures;if(events[e].kind==E_DRAWUP){if(events[e].a==4)upTris+=events[e].b;else upLines+=events[e].b;}}
     // Geometry: every queued primitive is drawn exactly once from the user-pointer batches.
     CHECK(upTris==queuedTris && upLines==queuedLines);
     CHECK(fvf==queued && draws==expectedDraws && locks==expectedLocks && locks==unlocks && selected==3);
     if(!expectedDraws)CHECK(!textures);
     CHECK(render->QuickDraw2DTriInfo.size()==0 && render->QuickDraw2DVertices.size()==0);
     CHECK(CRenderStateManagerTestAccess::Stack()==stackBefore && CRenderStateManagerTestAccess::Mask()==maskBefore && CRenderStateManagerTestAccess::Registered()==registered);
     for(long r=0;r<registered;++r){CRenderStateManager::CStateInfo* st=CRenderStateManagerTestAccess::Registered(r);CHECK(st->DesiredState==desiredBefore[r] && st->BookmarkMask==bookmarkBefore[r]);}
     if(records){CHECK(viewports>=1 && render->CurrentlySelectedTextures[0]==0 && shader.VertexShadersEnabled==vsBefore && shader.PixelShadersEnabled==psBefore);
      // A font draw binds its page; the next untextured geometry batch or the final attach unbinds stage 0.
      if(expectedDraws){unsigned last=nevents;while(last>start && events[last-1].kind!=E_TEXTURE)--last;CHECK(last>start && events[last-1].a==0 && events[last-1].b==0);}}
     else CHECK(nevents==start);
     // Each font owns one CCharString name; every queued/copied wide string is released by the flush.
     CHECK(g_CCharStringInstanceCount_013BD800==charBaseline && !g_CWideStringInstanceCount_013BCA20);
    }
    CHECK(references[0]==1 && references[1]==1 && references[2]==1);
    if(!mode){savedCount=nevents;memcpy(savedEvents,events,nevents*sizeof(DeviceEvent));savedPayloadUsed=payloadUsed;memcpy(savedPayload,payload,payloadUsed);memcpy(referenceRender,renderStorage,sizeof(referenceRender));memcpy(referenceShader,ShaderStorage,sizeof(referenceShader));}
    else{CHECK(savedCount==nevents);CHECK(savedPayloadUsed==payloadUsed);for(unsigned k=0;k<payloadUsed;++k)if(savedPayload[k]!=payload[k]){fprintf(stderr,"PAYLOAD_DIFF byte=%u candidate=%02x original=%02x\n",k,savedPayload[k],payload[k]);CHECK(false);}
     for(unsigned e=0;e<nevents;++e)if(!Same(events[e],savedEvents[e])){fprintf(stderr,"EVENT_DIFF index=%u candidate=%u,%x,%x,%x,%x original=%u,%x,%x,%x,%x\n",e,savedEvents[e].kind,savedEvents[e].a,savedEvents[e].b,savedEvents[e].c,savedEvents[e].d,events[e].kind,events[e].a,events[e].b,events[e].c,events[e].d);CHECK(false);}
     // The six 2D vectors (0x3E58..0x3EA0) hold heap addresses from each mode's own allocations: compare
     // their element counts and capacities, and every other manager byte exactly.
     for(unsigned v=0;v<6;++v){const unsigned* a=(const unsigned*)(referenceRender+0x3e58+v*12);const unsigned* o=(const unsigned*)(renderStorage+0x3e58+v*12);CHECK(a[1]-a[0]==o[1]-o[0] && a[2]-a[0]==o[2]-o[0]);}
     for(unsigned k=0;k<sizeof(referenceRender);++k)if((k<0x3e58 || k>=0x3ea0) && referenceRender[k]!=renderStorage[k]){fprintf(stderr,"MANAGER_DIFF offset=%x candidate=%02x original=%02x\n",k,referenceRender[k],renderStorage[k]);CHECK(false);}
     for(unsigned k=0;k<sizeof(referenceShader);++k)if(referenceShader[k]!=ShaderStorage[k]){fprintf(stderr,"SHADER_DIFF offset=%x candidate=%02x original=%02x\n",k,referenceShader[k],ShaderStorage[k]);CHECK(false);}}
   }
   for(unsigned n=0;n<2;++n)*(void**)fonts[n]=recreatedVtable;++row;
  }
  // Teardown: the actual destructor frees glyph banks and name and releases each font's texture.
  phase=3;for(unsigned n=0;n<2;++n){delete fonts[n];CHECK(!textureRefs[n] && !surfaceRefs[n]);}CHECK(g_CCharStringInstanceCount_013BD800==charBaseline-2);
 }
 phase=3;systemCalls=0;for(unsigned i=0;i<3;++i){CHECK(BufferScalar(buffers[i],0,0)==buffers[i] && !buffers[i]->Buffer && !references[i]);}CHECK(created==released && released==3);
 _controlfp(savedControl,_MCW_PC|_MCW_RC);if(code)CHECK(VirtualFree(code,0,0x8000));printf("UI_TEXT_FRAME_PASS cases=%u paths=%u\n",row,row*(code?2:1));return 0;
}
