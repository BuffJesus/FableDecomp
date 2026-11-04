#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "draw_distance_quality_aliases.h"
#include "draw_distance_quality_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall SetQuality(void*,void*,unsigned,void*);
static unsigned char* arena;
static unsigned p[4],calls;
static FILE* output;
static unsigned& At(unsigned offset){return *(unsigned*)(arena+offset);}
static void Record(unsigned tag,unsigned value){fwrite(&tag,4,1,output);fwrite(&value,4,1,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall Invalidate(void* self,void*) {
    if(calls>=3 || self!=*(void**)(arena+0x800+4*calls))abort();++calls;
    if(p[2]==1 && calls==1)At(0x33c)=(unsigned)(arena+0x804);
    if(p[2]==2 && calls==1)At(0x33c)=(unsigned)(arena+0x80c);
    if(p[2]==3)At(0x338)=(unsigned)(arena+0x880);
    if(p[2]==4){At(0x290)=(unsigned)(arena+0xb40);arena[p[3]+0x73]^=0xa5;At(p[3]+0x48)^=0xffffffff;}
    if(p[2]==5 && calls==1)At(0x804)=(unsigned)(arena+0x980);
    ++*(unsigned*)((unsigned char*)self+4);Record(1,(unsigned)self);
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+99)=(unsigned)&Invalidate-(unsigned)(code+103);
    typedef void (__fastcall *Fn)(void*,void*,unsigned,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&SetQuality;unsigned cases=0;
    while(fread(p,4,4,input)==4){
        if(fread(arena,1,4096,input)!=4096)return 6;calls=0;fn(arena,0,p[0],arena+p[3]);
        unsigned expected=p[1];if(expected && p[2]==1)expected=1;if(expected && p[2]==2)expected=3;
        if(calls!=expected || arena[0x98]!=1 || *((unsigned char*)At(0x290)+8)!=arena[p[3]+0x73])abort();Record(2,calls);++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_DRAW_DISTANCE_QUALITY_PASS cases=%u\n",cases);return 0;
}
