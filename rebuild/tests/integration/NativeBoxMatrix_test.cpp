#include <stdio.h>
#include <string.h>
#include <float.h>
#include "box_matrix_aliases.h"
#include "box_matrix_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Calculate(void*,void*,void*);
extern "C" bool __fastcall Clip(void*,void*,const void*);
extern "C" void* CameraGlobal;
extern "C" const float Delta;
static unsigned char* code;
static unsigned char* arena;
static FILE* output;
static unsigned kind;
static const float half=0.5f;
static void record(unsigned event,unsigned value){
    unsigned data[]={event,value,(unsigned)CameraGlobal,_statusfp()};fwrite(data,4,4,output);fwrite(arena,1,8192,output);
}
extern "C" bool __fastcall ClipBoundary(void* self,void*,const void* matrix){
    record(1,(unsigned)self);fwrite(matrix,1,48,output);
    memset(arena+0x600,0xa7,0x228);CameraGlobal=arena+0x600;
    return kind==6;
}
static unsigned resolve(unsigned target){
    if(target==0x1436ea0)return(unsigned)&CameraGlobal;
    if(target==0x129ba3c)return(unsigned)&Delta;
    if(target==0x122f59c)return(unsigned)&half;
    if(target==0xadebc0)return(unsigned)(code+256);
    if(target==0xb30610)return(unsigned)ClipBoundary;
    throw 1;
}
int main(int argc,char** argv){
    if(argc!=4||CameraGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned offset=retailFixups[i][0],target=resolve(retailFixups[i][1]);
        if(retailFixups[i][2])target-=(unsigned)(code+offset+4);
        *(unsigned*)(code+offset)=target;
    }
    typedef void (__fastcall *Calculator)(void*,void*,void*);
    typedef bool (__fastcall *Clipper)(void*,void*,const void*);
    Calculator calculate=argv[3][0]=='r'?(Calculator)(code+256):Calculate;
    Clipper clip=argv[3][0]=='r'?(Clipper)code:Clip;unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,8192,in)!=8192)return 5;
        kind=p[0];CameraGlobal=arena;_clearfp();_control87(p[2],_MCW_PC|_MCW_RC);unsigned result=0;
        if(kind<5)calculate(arena+0x1000,0,arena+p[1]);else result=clip((void*)0x1badb002,0,arena+0x1000);
        record(2,result);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_BOX_MATRIX_PASS cases=%u\n",cases);return 0;
}
