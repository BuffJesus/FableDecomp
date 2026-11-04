#include <stdio.h>
#include <string.h>
#include "fable_render_state_manager.h"
#include "fable_display_viewport.h"
#include "fable_surface_types.h"
static unsigned timeCalls;
double FABLE_FASTCALL GFGetTime(){return ++timeCalls*0.25;}
// Harness owns the OS/device boundary. COM slots and structures follow d3d9.h.
extern "C" __declspec(dllimport) void* __stdcall LoadLibraryA(const char*);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void*,const char*);
extern "C" __declspec(dllimport) void* __stdcall CreateWindowExA(unsigned,const char*,const char*,unsigned,int,int,int,int,void*,void*,void*,void*);
extern "C" __declspec(dllimport) int __stdcall DestroyWindow(void*);
struct Present { unsigned width,height,format,count,multi,quality,swap;void* window;int windowed,depth;unsigned depthFormat,flags,refresh,interval; };
struct Locked { int pitch;void* bits; };
struct CRenderStateManagerTestAccess {
    static void setup(CRenderStateManager* manager,void* device,CRenderStateManager::CStateInfo* states) {
        manager->PD3DDevice=device;
        for(unsigned i=0;i<5;++i)manager->StateUpdateList[i]=states+i;
        manager->StateUpdateListSize=5;
        manager->BookmarkDummyState.StateType=4;
        manager->CurrentBookmarkMask=1;
        manager->StartCaptureBlock();
        manager->StateStack[manager->StateStackSize].State=states+4;
        manager->StateStack[manager->StateStackSize++].Value=2;
        states[4].BookmarkMask=manager->CurrentBookmarkMask;
    }
    static bool empty(CRenderStateManager* manager){return manager->StateUpdateListSize==0;}
    static void queue(CRenderStateManager* manager,CRenderStateManager::CStateInfo* states,unsigned count){
        for(unsigned i=0;i<count;++i){manager->StateUpdateList[i]=states+i;states[i].DirtyListFlag=true;}
        manager->StateUpdateListSize=count;
    }
};
static void* slot(void* p,unsigned n){return (*(void***)p)[n];}
static void release(void* p){if(p)((unsigned(__stdcall*)(void*))slot(p,2))(p);}
static int failure(const char* op,long hr){printf("RASTER_FAIL %s %08lx\n",op,hr);return 1;}
#define CHECK(op,expr) do {long result=(expr);if(result<0)return failure(op,result);}while(0)
int main(int argc,char** argv)
{
    if(argc!=8)return 2;
    void* lib=LoadLibraryA("d3d9.dll");if(!lib)return failure("d3d9.dll",-1);
    typedef void* (__stdcall* Create)(unsigned);
    Create create=(Create)GetProcAddress(lib,"Direct3DCreate9");if(!create)return 3;
    void* api=create(32);if(!api)return 4;
    void* window=CreateWindowExA(0,"STATIC","Native reconstruction raster test",0,0,0,128,128,0,0,0,0);if(!window)return 5;
    Present pp={128,128,22,1,0,0,1,window,1,0,0,0,0,0x80000000};void* device=0;
    CHECK("CreateDevice",((long(__stdcall*)(void*,unsigned,unsigned,void*,unsigned,Present*,void**))slot(api,16))(api,0,1,window,0x20,&pp,&device));
    unsigned long storage[sizeof(CRenderManagerCore)/4];memset(storage,0,sizeof(storage));
    CRenderManagerCore* manager=(CRenderManagerCore*)storage;
    unsigned long displayStorage[sizeof(CDisplayManager)/4];memset(displayStorage,0,sizeof(displayStorage));
    CDisplayManager* display=(CDisplayManager*)displayStorage;
    *(CRenderManagerCore**)((unsigned char*)displayStorage+8)=manager;
    display->D3DDevice=(IDirect3DDevice9*)device;
    manager->PD3DDevice=(IDirect3DDevice9*)device;
    CRenderStateManager::CStateInfo states[5];
    const unsigned params[]={7,22,137,27,8};const unsigned values[]={0,1,0,0,3};
    for(unsigned i=0;i<5;++i){states[i].CurrentState=~values[i];states[i].DesiredState=values[i];states[i].StateParam=params[i];states[i].StateType=1;states[i].DirtyListFlag=true;}
    CRenderStateManagerTestAccess::setup(manager,device,states);
    manager->SetVertexFormat(0x44);
    void* texture=0;
    CHECK("CreateTexture",((long(__stdcall*)(void*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,void**,void*))slot(device,23))(device,2,2,1,0,21,1,&texture,0));
    Locked texLock;
    CHECK("TextureLock",((long(__stdcall*)(void*,unsigned,Locked*,void*,unsigned))slot(texture,19))(texture,0,&texLock,0,0));
    unsigned palette[]={0xff20c060,0xff2040e0,0xffe04020,0xffe0c020};
    for(unsigned ty=0;ty<2;++ty)memcpy((char*)texLock.bits+ty*texLock.pitch,palette+ty*2,8);
    CHECK("TextureUnlock",((long(__stdcall*)(void*,unsigned))slot(texture,20))(texture,0));
    unsigned textureStorage[2]={(unsigned)texture,0};CTexture* engineTexture=(CTexture*)textureStorage;
    C2DExtentsI actualTextureDimensions=engineTexture->GetDimensions(0);
    if(actualTextureDimensions.X!=2||actualTextureDimensions.Y!=2)return failure("RecoveredTextureDimensions",-1);
    long actualTextureLevelBytes=engineTexture->GetLevelByteLength(0);
    if(actualTextureLevelBytes!=16)return failure("RecoveredTextureLevelBytes",-1);
    CRenderStateManager::CStateInfo textureStates[10];
    const unsigned textureParams[]={8,1,2,4,5,6,5,1,2,1};
    const unsigned textureValues[]={3,2,2,2,2,1,1,3,3,1};
    for(unsigned ts=0;ts<10;++ts){textureStates[ts].CurrentState=~textureValues[ts];textureStates[ts].DesiredState=textureValues[ts];textureStates[ts].StateParam=textureParams[ts];textureStates[ts].StateType=ts==0?1:ts>=5&&ts<=8?3:2;textureStates[ts].TextureStage=ts==9?1:0;}
    unsigned redCounts[7];
    for(unsigned frame=0;frame<7;++frame){
    if(frame==1)manager->RestoreCaptureBlock();
    if(frame==2){
        CRenderStateManagerTestAccess::queue(manager,textureStates,10);
        manager->SetVertexFormat(0x144);
    }
    CHECK("Clear",((long(__stdcall*)(void*,unsigned,void*,unsigned,unsigned,float,unsigned))slot(device,43))(device,0,0,1,0xff102030,1.0f,0));
    if(!display->BeginRender()||!manager->Rendering||!display->CanBeginRender)return 9;
    struct Vertex {float x,y,z,rhw;unsigned colour;float u,v;};
    unsigned colour=frame==2?0xffffffff:0xffe04020;
    Vertex vertices[]={{16,16,0,1,colour,0,0},{112,16,0,1,colour,1,0},{16,112,0,1,colour,0,1}};
    unsigned short indices[]={0,1,2};
    if(frame>=3)for(unsigned vc=0;vc<3;++vc)vertices[vc].colour=0xffffffff;
    void* vertexBuffer=0;void* indexBuffer=0;
    if(frame>=3){
        CHECK("CreateVertexBuffer",((long(__stdcall*)(void*,unsigned,unsigned,unsigned,unsigned,void**,void*))slot(device,26))(device,sizeof(vertices),0,0x144,1,&vertexBuffer,0));
        CHECK("CreateIndexBuffer",((long(__stdcall*)(void*,unsigned,unsigned,unsigned,unsigned,void**,void*))slot(device,27))(device,sizeof(indices),0,101,1,&indexBuffer,0));
        void* uploaded=0;
        CHECK("VertexLock",((long(__stdcall*)(void*,unsigned,unsigned,void**,unsigned))slot(vertexBuffer,11))(vertexBuffer,0,sizeof(vertices),&uploaded,0));memcpy(uploaded,vertices,sizeof(vertices));
        CHECK("VertexUnlock",((long(__stdcall*)(void*))slot(vertexBuffer,12))(vertexBuffer));
        CHECK("IndexLock",((long(__stdcall*)(void*,unsigned,unsigned,void**,unsigned))slot(indexBuffer,11))(indexBuffer,0,sizeof(indices),&uploaded,0));memcpy(uploaded,indices,sizeof(indices));
        CHECK("IndexUnlock",((long(__stdcall*)(void*))slot(indexBuffer,12))(indexBuffer));
        CHECK("SetStreamSource",((long(__stdcall*)(void*,unsigned,void*,unsigned,unsigned))slot(device,100))(device,0,vertexBuffer,0,sizeof(Vertex)));
        CHECK("SetIndices",((long(__stdcall*)(void*,void*))slot(device,104))(device,indexBuffer));
    }
    if(frame==2){const CTexture* textures[]={engineTexture};manager->DrawAsIndexedTris(textures,1,vertices,sizeof(Vertex),3,indices,1);}
    else if(frame==3)manager->DrawCurrentBuffersAsIndexedTris(0,3,0,1);
    else if(frame==4)manager->DrawCurrentBuffersAsTris(0,1);
    else if(frame>=5){
        manager->AttachTextureToStage(0,0);
        const CTexture* textures[]={engineTexture};
        if(frame==5)manager->DrawCurrentBuffersAsIndexedTris(textures,1,0,3,0,1);
        else manager->DrawCurrentBuffersAsTris(textures,1,0,1);
    }
    else manager->DrawAsIndexedTris(vertices,sizeof(Vertex),3,indices,1);
    if(frame>=2&&(manager->CurrentlySelectedTextures[0]!=(IDirect3DBaseTexture9*)texture||manager->TextureStagesInUse!=1))return 12;
    display->EndRender();
    release(indexBuffer);release(vertexBuffer);
    if(manager->Rendering||display->CanBeginRender||timeCalls!=(frame+1)*2)return 10;
    if(!CRenderStateManagerTestAccess::empty(manager))return 6;
    for(unsigned j=0;j<4;++j)if(states[j].CurrentState!=values[j]||states[j].DirtyListFlag)return 7;
    if(states[4].CurrentState!=(frame?2:3)||states[4].DirtyListFlag)return 11;
    if(frame>=2)for(unsigned t=0;t<10;++t)if(textureStates[t].CurrentState!=textureValues[t]||textureStates[t].DirtyListFlag)return 14;
    void* target=0;void* readback=0;
    CHECK("GetRenderTarget",((long(__stdcall*)(void*,unsigned,void**))slot(device,38))(device,0,&target));
    CHECK("CreateOffscreenPlainSurface",((long(__stdcall*)(void*,unsigned,unsigned,unsigned,unsigned,void**,void*))slot(device,36))(device,128,128,22,2,&readback,0));
    CHECK("GetRenderTargetData",((long(__stdcall*)(void*,void*,void*))slot(device,32))(device,target,readback));
    Locked lock;CHECK("LockRect",((long(__stdcall*)(void*,Locked*,void*,unsigned))slot(readback,13))(readback,&lock,0,0x10));
    unsigned red=0,background=0,green=0,blue=0;FILE* out=fopen(argv[frame+1],"wb");if(!out)return 8;
    fprintf(out,"P6\n128 128\n255\n");
    for(unsigned y=0;y<128;++y)for(unsigned x=0;x<128;++x){unsigned pixel=(*(unsigned*)((char*)lock.bits+y*lock.pitch+x*4))&0xffffff;unsigned char rgb[]={(unsigned char)(pixel>>16),(unsigned char)(pixel>>8),(unsigned char)pixel};fwrite(rgb,1,3,out);red+=pixel==0xe04020;background+=pixel==0x102030;green+=pixel==0x20c060;blue+=pixel==0x2040e0;}
    unsigned inside=(*(unsigned*)((char*)lock.bits+32*lock.pitch+32*4))&0xffffff;
    unsigned outside=(*(unsigned*)((char*)lock.bits+120*lock.pitch+120*4))&0xffffff;
    unsigned upperRight=(*(unsigned*)((char*)lock.bits+32*lock.pitch+88*4))&0xffffff;
    unsigned lowerLeft=(*(unsigned*)((char*)lock.bits+88*lock.pitch+32*4))&0xffffff;
    fclose(out);CHECK("UnlockRect",((long(__stdcall*)(void*))slot(readback,14))(readback));
    release(readback);release(target);
    if(frame<2){
        if(inside!=(frame?0x102030:0xe04020)||outside!=0x102030||red<(frame?256:4400)||red>(frame?320:4800)||red+background!=16384)return failure("pixel validation",-1);
        redCounts[frame]=red;
    }else{
        if(inside!=0x20c060||upperRight!=0x2040e0||lowerLeft!=0xe04020||outside!=0x102030||green<2000||blue<500||red<500||red+green+blue!=redCounts[0]||red+green+blue+background!=16384)return failure("texture pixels",-1);
        redCounts[frame]=red+green+blue;
    }
    }
    manager->AttachTextureToStage(0,0);if(manager->CurrentlySelectedTextures[0])return 13;
    release(texture);
    release(device);release(api);DestroyWindow(window);
    printf("NATIVE_INDEXED_RASTER_PASS solid=%u restored_wire=%u textured=%u buffer_indexed=%u buffer_tris=%u textured_buffer_indexed=%u textured_buffer_tris=%u pixels=114688 texture_level_bytes=16\n",redCounts[0],redCounts[1],redCounts[2],redCounts[3],redCounts[4],redCounts[5],redCounts[6]);return 0;
}
