#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffered_triangles_retail.h"
#include "buffered_triangles_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall DrawTris(void*,void*,unsigned long,unsigned long);
extern "C" void __fastcall DrawIndexed(void*,void*,unsigned long,unsigned long,unsigned long,unsigned long);
static const unsigned WIDTH=32768;
struct Snapshot { unsigned args[10];unsigned char bytes[WIDTH]; };
static Snapshot snapshots[3][20];static unsigned char* arena;
static unsigned run,events,offset,mode,textureCount,dirty,cases,total,kind,pattern;
static unsigned& word(unsigned at){return *(unsigned*)(arena+at);}
static void require(bool v){if(!v){printf("BUFFERED_TRIANGLES_FAIL cases=%u run=%u mode=%u events=%u\n",cases,run,mode,events);fflush(stdout);abort();}}
static void record(unsigned kind,const unsigned* args,unsigned n){
    require(events<20);Snapshot& s=snapshots[run][events++];memset(s.args,0,sizeof(s.args));s.args[0]=kind;
    for(unsigned i=0;i<n;++i)s.args[i+1]=args[i];memcpy(s.bytes,arena,WIDTH);++total;
}
static long __stdcall state(void* self,unsigned param,unsigned value){
    unsigned args[]={(unsigned)self,param,value};record(2,args,3);
    if(mode&1)word(0x5400+(param-7)*0x14+4)^=0x12345678;
    if(mode&2)word(offset+4)=word(offset+0x3cf0)=(unsigned)(arena+0x6840);
    return mode?0x80004005:0;
}
static long __stdcall drawTris(void* self,unsigned type,unsigned start,unsigned count){unsigned args[]={(unsigned)self,type,start,count};record(1,args,4);return mode?0x80004005:0;}
static long __stdcall drawIndexed(void* self,unsigned type,int base,unsigned min,unsigned vertices,unsigned start,unsigned count){unsigned args[]={(unsigned)self,type,(unsigned)base,min,vertices,start,count};record(3,args,7);return mode?0x80004005:0;}
static void model(){
    for(unsigned i=0;i<word(offset+0x280c);++i){
        unsigned char* info=(unsigned char*)word(offset+0x2008+i*4);unsigned* values=(unsigned*)info;
        info[0x10]=0;
        if(values[0]!=values[1]){state((void*)word(offset+4),values[3],values[1]);values[0]=values[1];}
    }
    word(offset+0x280c)=0;
    if(kind)drawIndexed((void*)word(offset+0x3cf0),4,0,pattern*17,pattern==3?0xffffffff:pattern*11,pattern*5,textureCount);
    else drawTris((void*)word(offset+0x3cf0),4,pattern*17,textureCount);
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,WIDTH,0x3000,4);require(arena==(void*)0x24000000);
    unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned f=0;f<2;++f){unsigned at=realiseFixups[f];*(unsigned*)(code+at)=(unsigned)(code+256)-(unsigned)(code+at+4);}
    typedef void (__fastcall* Tris)(void*,void*,unsigned long,unsigned long);
    typedef void (__fastcall* Indexed)(void*,void*,unsigned long,unsigned long,unsigned long,unsigned long);
    for(textureCount=0;textureCount<=8;++textureCount)for(pattern=0;pattern<8;++pattern)for(mode=0;mode<4;++mode)for(unsigned place=0;place<2;++place)for(dirty=0;dirty<3;dirty+=2)for(kind=0;kind<2;++kind){
        offset=place?0x1000:0;unsigned counts[3];
        for(run=0;run<3;++run){
            memset(arena,pattern&1?0x95:0,WIDTH);word(offset+4)=word(offset+0x3cf0)=(unsigned)(arena+0x6800);
            word(0x6800)=word(0x6840)=(unsigned)(arena+0x7000);
            word(0x7000+57*4)=(unsigned)state;word(0x7000+81*4)=(unsigned)drawTris;word(0x7000+82*4)=(unsigned)drawIndexed;
            word(offset+0x3cb8)=pattern?pattern-1:0xffffffff;word(offset+0x280c)=dirty;
            for(unsigned s=0;s<2;++s){unsigned at=0x5400+s*0x14;word(offset+0x2008+s*4)=(unsigned)(arena+at);word(at)=pattern==2?1:0;word(at+4)=1;word(at+12)=7+s;arena[at+16]=1;arena[at+17]=1;}
            events=0;
            if(run==2)model();else if(kind){Indexed method=run?DrawIndexed:(Indexed)(code+64);method(arena+offset,0,pattern*17,pattern==3?0xffffffff:pattern*11,pattern*5,textureCount);}
            else{Tris method=run?DrawTris:(Tris)code;method(arena+offset,0,pattern*17,textureCount);}
            record(4,0,0);counts[run]=events;
        }
        require(counts[0]==counts[1]&&counts[1]==counts[2]);
        require(!memcmp(snapshots[0],snapshots[1],counts[0]*sizeof(Snapshot)));require(!memcmp(snapshots[0],snapshots[2],counts[0]*sizeof(Snapshot)));++cases;
    }
    VirtualFree(code,0,0x8000);VirtualFree(arena,0,0x8000);printf("NATIVE_BUFFERED_TRIANGLES_PASS cases=%u snapshots=%u\n",cases,total);return 0;
}
