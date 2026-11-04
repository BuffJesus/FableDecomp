#include <stdio.h>
#include <string.h>
#include "fable_font_vertex_buffer.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
static unsigned char systemStorage[0x64],displayStorage[0x5c];
static unsigned long deviceStorage[1];
static void* deviceMethods[27];
static IDirect3DVertexBuffer9* expectedBuffer=reinterpret_cast<IDirect3DVertexBuffer9*>(0x556600);
static int calls,failNext;
CSystemManager* FABLE_FASTCALL GFGetSystemManager() { return reinterpret_cast<CSystemManager*>(systemStorage); }
static HRESULT __stdcall CreateVertexBuffer(
    IDirect3DDevice9* device, UINT length, DWORD usage, DWORD fvf, D3DPOOL pool,
    IDirect3DVertexBuffer9** output, HANDLE* shared)
{
    ++calls;
    if(device!=reinterpret_cast<IDirect3DDevice9*>(deviceStorage)||length!=8400||usage!=8||
       fvf!=0x144||pool!=D3DPOOL_MANAGED||shared)return -2;
    *output=expectedBuffer;
    return failNext?-1:0;
}
static unsigned long word(const unsigned char* p,unsigned offset) {return *reinterpret_cast<const unsigned long*>(p+offset);}
int main() {
    memset(systemStorage,0,sizeof(systemStorage));memset(displayStorage,0,sizeof(displayStorage));
    memset(deviceMethods,0,sizeof(deviceMethods));deviceMethods[26]=(void*)&CreateVertexBuffer;
    deviceStorage[0]=reinterpret_cast<unsigned long>(deviceMethods);
    *reinterpret_cast<void**>(systemStorage+0x60)=displayStorage;
    *reinterpret_cast<void**>(displayStorage+0x58)=deviceStorage;
    unsigned char storage[sizeof(CVertexBufferWin32)];
    for(unsigned trial=0;trial<2;++trial) {
        memset(storage,0xA5,sizeof(storage));failNext=trial;
        CVertexBufferWin32& buffer=*reinterpret_cast<CVertexBufferWin32*>(storage);
        bool ok=buffer.Initialise(VertexType_0x144,300,28,8,VertexBufferPool_1);
        if(ok!=(trial==0)||calls!=(int)trial+1)return 1;
        if(word(storage,4)!=reinterpret_cast<unsigned long>(expectedBuffer)||word(storage,8)!=8400||
           word(storage,12)!=0x144||storage[0x10]!=28)return 2;
        for(unsigned offset=0x14;offset<0x24;offset+=4)if(word(storage,offset)!=0x3f800000)return 3;
        for(unsigned offset=0x24;offset<0x34;offset+=4)if(word(storage,offset))return 4;
    }
    puts("FONT_VB_INITIALISE_00a63150_PASS");return 0;
}