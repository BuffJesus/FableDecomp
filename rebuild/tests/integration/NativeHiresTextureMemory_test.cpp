#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hires_texture_memory_aliases.h"
#include "hires_texture_memory_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall ResizeHires(void*,void*,long);
extern "C" void* SurfaceGlobal;
static unsigned char* arena;
static unsigned p[7],stage,originalDelay,delayCalls;
static void* formatPointer;
static FILE* output;
static unsigned& At(unsigned offset){return *(unsigned*)(arena+offset);}
static void Record(unsigned tag,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0,unsigned e=0){
    if(tag!=++stage)abort();unsigned words[6]={tag,a,b,c,d,e};fwrite(words,4,6,output);fwrite(&SurfaceGlobal,4,1,output);fwrite(arena,1,16384,output);
}
extern "C" void __fastcall ChangeScene(void* self,void*){
    if(self!=arena+0x1000)abort();if(p[3]==1)SurfaceGlobal=arena+0x2400;Record(1,(unsigned)self);
}
extern "C" void __fastcall Composition(void* self,void*){
    if(self!=SurfaceGlobal)abort();if(p[3]==2)At(0x958)=(unsigned)(arena+0x1800);Record(2,(unsigned)self);
}
extern "C" void __fastcall PixelInit(void* self,void*,unsigned format){
    if(format!=0x31545844)abort();formatPointer=self;*(unsigned*)self=7+p[3];if(p[3]==3)At(0x958)=(unsigned)(arena+0x1800);Record(3,format,*(unsigned*)self);
}
extern "C" unsigned __fastcall TextureSize(unsigned x,unsigned y,bool mipmaps,void* format){
    if(x!=512||y!=512||!mipmaps||format!=formatPointer||*(unsigned*)format!=7+p[3])abort();if(p[3]==4)At(0x958)=(unsigned)(arena+0x1800);Record(4,x,y,mipmaps,*(unsigned*)format,p[1]);return p[1];
}
extern "C" void __fastcall SetDelay(void* self,void*,unsigned delay){
    if(self!=(void*)At(0x958))abort();unsigned char* bank=(unsigned char*)self;
    if(++delayCalls==1){if(delay)abort();originalDelay=*(unsigned*)(bank+0x26c);}else if(delayCalls!=2||delay!=originalDelay)abort();
    *(unsigned*)(bank+0x26c)=delay;*(unsigned*)(bank+0x19c)=delay;
    if(delayCalls==1&&p[3]==5)At(0x958)=(unsigned)(arena+0x1c00);
    if(delayCalls==1&&p[3]==7)*(void**)(bank+0x280)=arena+0x3400;
    Record(delayCalls==1?5:7,(unsigned)self,delay);
}
extern "C" bool __fastcall ResizePool(void* self,void*,unsigned size,unsigned count){
    if(self!=*(void**)((unsigned char*)At(0x958)+0x280)||size!=p[1]||count!=p[5])abort();if(p[3]==6)At(0x958)=(unsigned)(arena+0x1c00);Record(6,(unsigned)self,size,count,p[2]);return p[2]!=0;
}
static unsigned Target(unsigned address){switch(address){
    case 0x9fd0c0:return (unsigned)&ChangeScene;case 0x1436eac:return (unsigned)&SurfaceGlobal;case 0xb2ad80:return (unsigned)&Composition;case 0x9e3830:return (unsigned)&PixelInit;case 0xa698f0:return (unsigned)&TextureSize;case 0x9fd100:return (unsigned)&SetDelay;case 0xa6abe0:return (unsigned)&ResizePool;default:abort();return 0;
}}
int main(int argc,char** argv){
    if(argc!=4||SurfaceGlobal)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned value=Target(retailFixups[i][1]);if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;}
    typedef bool (__fastcall *Fn)(void*,void*,long);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&ResizeHires;unsigned cases=0;
    while(fread(p,4,7,input)==7){
        if(fread(arena,1,16384,input)!=16384)return 6;stage=delayCalls=0;SurfaceGlobal=arena+0x2000;formatPointer=0;
        bool result=fn(arena,0,(long)p[0]);if(result!=(p[2]!=0)||delayCalls!=2)abort();Record(8,result);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_HIRES_TEXTURE_MEMORY_PASS cases=%u\n",cases);return 0;
}
