#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hires_pool_selection_aliases.h"
#include "hires_pool_selection_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall ResizeHires(void*,void*,long);
extern "C" void* SurfaceGlobal;extern "C" const unsigned char FormatTable[1692];
static unsigned char* arena;static unsigned char* nativeCode;
static unsigned p[7],stage,originalDelay,delayCalls,frameCalls,poolCalls;static long selected;
static void* formatPointer;
static FILE* output;
static unsigned& At(unsigned offset){return *(unsigned*)(arena+offset);}
static void Record(unsigned tag,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0,unsigned e=0){
    if(tag<=stage)abort();stage=tag;unsigned words[6]={tag,a,b,c,d,e};fwrite(words,4,6,output);fwrite(&SurfaceGlobal,4,1,output);fwrite(arena,1,16384,output);
}
extern "C" void __fastcall Frame(void* self,void*) {
    if(self!=arena+0x3000||selected<0||++frameCalls!=1)abort();
    if(p[3]==2)At(0x3008)=0;
    if(p[3]==7)*(void**)((unsigned char*)At(0x958)+0x280)=arena+0x3400;
    Record(6,(unsigned)self,selected);
}
extern "C" bool __fastcall ResizePool(void* self,void*,unsigned count){
    if(self!=arena+0x304c+0x54*selected||count!=p[5]||++poolCalls!=1)abort();
    if(*(unsigned*)((unsigned char*)At(0x958)+0x26c)||*(unsigned*)((unsigned char*)At(0x958)+0x19c))abort();
    *(unsigned*)((unsigned char*)self+0x3c)=count;
    if(p[3]==6)At(0x958)=(unsigned)(arena+0x1c00);Record(7,(unsigned)self,count,p[2]);return p[2]!=0;
}
static unsigned Target(unsigned address){switch(address){
    case 0x9fd0c0:return (unsigned)(nativeCode+144);case 0x9fc160:return (unsigned)(nativeCode+224);case 0x1436eac:return (unsigned)&SurfaceGlobal;case 0xb2ad80:return (unsigned)(nativeCode+464);case 0x9e3830:return (unsigned)(nativeCode+400);case 0x9e3820:return (unsigned)(nativeCode+384);case 0x129ba40:return (unsigned)FormatTable;case 0x129ba48:return (unsigned)(FormatTable+8);case 0xa698f0:return (unsigned)(nativeCode+240);case 0x9fd100:return (unsigned)(nativeCode+192);case 0xa6abe0:return (unsigned)(nativeCode+480);case 0xa69730:return (unsigned)(nativeCode+544);case 0xa698b0:return (unsigned)&Frame;case 0xa6a670:return (unsigned)&ResizePool;default:abort();return 0;
}}
int main(int argc,char** argv){
    if(argc!=4||SurfaceGlobal)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;nativeCode=code;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned value=Target(retailFixups[i][1]);if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;}
    typedef bool (__fastcall *Fn)(void*,void*,long);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&ResizeHires;unsigned cases=0;unsigned char table[1692];memcpy(table,FormatTable,1692);
    while(fread(p,4,7,input)==7){
        if(fread(arena,1,16384,input)!=16384)return 6;stage=delayCalls=frameCalls=poolCalls=0;SurfaceGlobal=arena+0x2000;formatPointer=0;
        unsigned resourceFrame=At(0x1194)+At(0x1268),queueFrame=At(0x11d0)+At(0x1268);
        unsigned compositionFrame=At(0x252c)+At(0x2600),compositionQueue=At(0x2568)+At(0x2600);originalDelay=*(unsigned*)((unsigned char*)At(0x958)+0x26c);
        selected=-1;for(unsigned j=0;j<At(0x3008);++j)if(At(0x300c+4*j)>=p[1]){selected=j;break;}
        bool result=fn(arena,0,(long)p[0]);if(frameCalls!=(selected>=0?1:0)||poolCalls!=frameCalls)abort();if(result!=(selected>=0&&p[2]!=0)||At(0x1194)!=resourceFrame||At(0x11d0)!=queueFrame)abort();
        if(At(0x252c)!=compositionFrame||At(0x2568)!=compositionQueue)abort();unsigned char* bank=(unsigned char*)At(0x958);if(*(unsigned*)(bank+0x26c)!=originalDelay||*(unsigned*)(bank+0x19c)!=originalDelay)abort();if(memcmp(table,FormatTable,1692))abort();Record(8,result);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_HIRES_POOL_SELECTION_PASS cases=%u\n",cases);return 0;
}
