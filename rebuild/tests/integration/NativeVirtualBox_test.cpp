#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_virtual_pixels.h"
#include "fable_2d_vector.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
void FABLE_FASTCALL GFVBoxToABox(const C2DBoxF*,C2DBoxF*);
double g_FableVirtualWidth_013961E8=128,g_FableVirtualHeight_013961F0=256;
static unsigned char* arena;static FILE* output;static unsigned calls,mutation,sourceOffset;
static unsigned long& word(unsigned offset){return *(unsigned long*)(arena+offset);}
static void event(unsigned kind,unsigned arg=0){unsigned header[]={kind,arg,calls,0};fwrite(header,4,4,output);fwrite(arena,1,0x5000,output);}
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){
    event(1);++calls;
    if(mutation){
        word(0x160)=(unsigned)(arena+((calls&1)?0x600:0x300));
        word(sourceOffset+(calls%4)*4)=0x42800000+calls*0x10000;
    }
    return (CSystemManager*)(arena+0x100);
}
void CDisplayManager::SetViewport(const C2DBoxF& box){
    memcpy(arena+0x900,&box,16);event(2,(unsigned)this);
}
int main(int argc,char** argv){
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned params[5],cases=0;const unsigned modes[]={_RC_NEAR,_RC_DOWN,_RC_UP,_RC_CHOP};
    while(fread(params,4,5,input)==5){
        if(fread(arena,1,0x5000,input)!=0x5000)return 5;
        calls=0;mutation=params[1];sourceOffset=params[3];_controlfp(modes[params[2]],_MCW_RC);
        g_FableVirtualWidth_013961E8=*(double*)(arena+0x4800);g_FableVirtualHeight_013961F0=*(double*)(arena+0x4808);
        if(params[0]==3)GFABoxToVBox(*(C2DBoxF*)(arena+sourceOffset),*(C2DBoxF*)(arena+params[4]));
        else if(params[0]==2)GFACoordToVCoord(*(C2DVector*)(arena+sourceOffset),*(C2DVector*)(arena+params[4]));
        else if(params[0])((CRenderManagerCore*)(arena+0x1000))->SetVWindow(*(C2DBoxF*)(arena+sourceOffset));
        else GFVBoxToABox((C2DBoxF*)(arena+sourceOffset),(C2DBoxF*)(arena+params[4]));
        if(calls!=(params[0]==2?2:4))return 6;event(3);++cases;
    }
    fclose(input);fclose(output);VirtualFree(arena,0,0x8000);
    printf("NATIVE_VIRTUAL_BOX_PASS cases=%u\n",cases);return 0;
}
