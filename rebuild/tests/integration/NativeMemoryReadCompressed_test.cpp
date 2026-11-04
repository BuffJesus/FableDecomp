#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_compressed_aliases.h"
#include "memory_compressed_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ReadCompressed(void*,void*,void*,long);
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,unsigned a,unsigned b){if(++events>16)exit(10);unsigned h[]={kind,a,b};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,2048,output)!=2048)exit(9);}
extern "C" unsigned __fastcall Position(void* self){
    unsigned result=at(4);record(1,(unsigned)self,result);
    if(mode==1)return 0x7fffffff;
    if(mode==2)return 0xfffffffc;
    if(mode==3){at(4)=5;at(24)=(unsigned)(arena+0x720);}
    return result;
}
extern "C" void __fastcall Overflow(void* self,void*,void* dest,long size){
    if(size<0||size>16)exit(20);unsigned ptr=at(12);if(ptr<(unsigned)arena+0x400||ptr+size>(unsigned)arena+0x780)exit(21);
    memcpy(dest,(void*)ptr,size);at(12)+=size;at(4)+=size;at(20)=0;
    unsigned relative=(unsigned)dest-(unsigned)arena;record(2,relative<2048?relative:0,size);
}
extern "C" int __cdecl Decompress(const unsigned char* src,unsigned sourceSize,unsigned char* dest,unsigned* length,void* work){
    if(work||*length>16||dest!=arena+0x100)exit(22);record(3,(unsigned)src,sourceSize);
    unsigned size=*length;memset(dest,0xa7,size);*length=mode>=4?size:size-3;
    // The decompressor is an explicit callback boundary. Stabilize the tail
    // window after exercising wrapped/invalid const-data positions.
    at(12)=(unsigned)(arena+0x700);at(4)=12;at(20)=mode==6?0:16;
    record(4,*length,size);return mode==7?-1:0;
}
extern "C" void __fastcall Error(){record(5,0,0);if(mode==5)at(4)=0x7fffffff;}
extern "C" void (__fastcall* ErrorCallback)()=Error;
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<6;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0x993ca0:v=(unsigned)Overflow;break;case 0xc069d0:v=(unsigned)Decompress;break;case 0x13bc9fc:v=(unsigned)&ErrorCallback;break;default:return 8;}*(unsigned*)(code+off)=retailFixups[i][1]==0x13bc9fc?v:v-(unsigned)(code+off+4);}
    typedef void (__fastcall *Fn)(void*,void*,void*,long);Fn fn=argv[3][0]=='r'?(Fn)code:ReadCompressed;
    unsigned p[2],cases=0;while(fread(p,4,2,in)==2){if(fread(arena,1,2048,in)!=2048)return 5;mode=p[0];events=0;ErrorCallback=mode==7?0:Error;at(0)=(unsigned)(arena+0x600);at(0x608)=(unsigned)Position;fn(arena,0,arena+0x100,(long)p[1]);record(6,0,0);++cases;}
    fclose(in);fclose(output);printf("NATIVE_MEMORY_READ_COMPRESSED_PASS cases=%u\n",cases);return 0;
}
