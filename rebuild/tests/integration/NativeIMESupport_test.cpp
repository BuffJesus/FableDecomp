#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_ime.h"

extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*, unsigned long, unsigned long, unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*, unsigned long, unsigned long);
namespace NUISystem {
struct CIMERecoveryTestAccess {
    static void Shutdown(CIME& ime) { ime.Shutdown(); }
    static void* Allocate(unsigned size) { return CIME::Callback_malloc(size); }
    static void Release(void* pointer) { CIME::Callback_free(pointer); }
};
}

static unsigned char* arena;
static unsigned char* current;
static unsigned mutation;
static FILE* output;
static void event(unsigned kind, unsigned value=0) {
    unsigned data[]={kind,value};
    fwrite(data,4,2,output); fwrite(arena,1,4096,output);
    fwrite(&g_FableIME_013B7CD8,1,sizeof(g_FableIME_013B7CD8),output);
}
void FABLE_FASTCALL ImeUi_FinalizeString(bool send) {
    event(1,send);
    if(mutation&1) *(unsigned*)(current+4)=0x24000740;
}
void FABLE_FASTCALL ImeUi_EnableIme(bool enable) {
    event(2,enable);
    if(mutation&2) *(unsigned*)(current+0x1c)=0x24000780;
}
bool FABLE_FASTCALL ImeUi_Initialize(HWND__* window,bool disable) {
    event(3,(unsigned)window); event(4,disable);
    if(mutation&4) { current[0]=1; current[8]=1; current[0x50]^=1; }
    return (mutation&1)!=0;
}
void __cdecl operator delete(void* pointer) {
    event(5,(unsigned)pointer);
    if(mutation&4) *(unsigned*)(current+0x4c)=0x240007c0;
}

int main(int argc,char** argv) {
    if(argc!=3)return 2;
    FILE* input=fopen(argv[1],"rb"); output=fopen(argv[2],"wb"); if(!input||!output)return 3;
    unsigned char zero[sizeof(g_FableIME_013B7CD8)]={0};
    if(memcmp(&g_FableIME_013B7CD8,zero,sizeof(zero)))return 4;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);
    if(arena!=(void*)0x24000000)return 5;
    unsigned params[6],cases=0; const unsigned modes[]={_RC_NEAR,_RC_DOWN,_RC_UP,_RC_CHOP};
    while(fread(params,4,6,input)==6) {
        if(fread(arena,1,4096,input)!=4096 || fread(&g_FableIME_013B7CD8,1,sizeof(g_FableIME_013B7CD8),input)!=sizeof(g_FableIME_013B7CD8))return 6;
        current=params[0]==2 ? (unsigned char*)&g_FableIME_013B7CD8 : arena+0x100;
        mutation=params[1]; _controlfp(modes[params[2]],_MCW_RC);
        if(params[0]==0) NUISystem::CIME::ResetToDefaultSettings();
        else if(params[0]==3) {
            NUISystem::CIMEFont* font=(NUISystem::CIMEFont*)(arena+0x200);
            font->NUISystem::CIMEFont::SetHeight(params[3]);
            font->NUISystem::CIMEFont::SetPosition((int)params[4],(int)params[5]);
        } else NUISystem::CIMERecoveryTestAccess::Shutdown(*(NUISystem::CIME*)current);
        event(6); ++cases;
    }
    const unsigned sizes[]={1,2,8,16,32,256,4096,65536};
    unsigned allocations=0;
    for(unsigned cycle=0;cycle<32;++cycle)for(unsigned i=0;i<8;++i) {
        unsigned char* data=(unsigned char*)NUISystem::CIMERecoveryTestAccess::Allocate(sizes[i]);
        if(!data)return 7;
        for(unsigned j=0;j<sizes[i];++j)data[j]=(unsigned char)(j+cycle);
        for(unsigned j=0;j<sizes[i];++j)if(data[j]!=(unsigned char)(j+cycle))return 8;
        NUISystem::CIMERecoveryTestAccess::Release(data); ++allocations;
    }
    NUISystem::CIMERecoveryTestAccess::Release(0);
    fclose(input); if(fclose(output))return 9; VirtualFree(arena,0,0x8000);
    printf("NATIVE_IME_SUPPORT_PASS cases=%u allocations=%u\n",cases,allocations);
    return 0;
}
