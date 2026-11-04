#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indexed_textures_retail.h"
#include "indexed_textures_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall DrawTextures(void*,void*,void*,unsigned,void*,long,long,void*,long);
static const unsigned WIDTH=32768;
struct Snapshot { unsigned args[10];unsigned char bytes[WIDTH]; };
static Snapshot snapshots[3][20];static unsigned char* arena;
static unsigned run,events,offset,mode,textureCount,dirty,cases,total;
static unsigned& word(unsigned at){return *(unsigned*)(arena+at);}
static void require(bool v){if(!v){printf("INDEXED_TEXTURES_FAIL cases=%u run=%u mode=%u events=%u\n",cases,run,mode,events);fflush(stdout);abort();}}
static void record(unsigned kind,const unsigned* args,unsigned n){
    require(events<20);Snapshot& s=snapshots[run][events++];memset(s.args,0,sizeof(s.args));s.args[0]=kind;
    for(unsigned i=0;i<n;++i)s.args[i+1]=args[i];memcpy(s.bytes,arena,WIDTH);++total;
}
static long __stdcall bind(void* self,unsigned stage,void* texture){
    unsigned args[]={(unsigned)self,stage,(unsigned)texture};record(1,args,3);
    if((mode&1)&&stage+1<textureCount)word(0x5100+(stage+1)*4)=0;
    if(mode&2)word(offset+0x3cf0)=(unsigned)(arena+0x6840);
    return mode?0x80004005:0;
}
static long __stdcall state(void* self,unsigned param,unsigned value){
    unsigned args[]={(unsigned)self,param,value};record(2,args,3);
    if(mode&4)word(0x5400+(param-7)*0x14+4)^=0x12345678;
    if(mode&2)word(offset+4)=(unsigned)(arena+0x6840);
    return mode?0x80004005:0;
}
static long __stdcall draw(void* self,unsigned type,unsigned min,unsigned vertices,unsigned primitives,void* indices,unsigned format,void* data,unsigned stride){
    unsigned args[]={(unsigned)self,type,min,vertices,primitives,(unsigned)indices,format,(unsigned)data,stride};record(3,args,9);return mode?0x80004005:0;
}
static void model(){
    for(unsigned stage=0;stage<textureCount;++stage){
        unsigned wrapper=word(0x5100+stage*4),texture=wrapper?*(unsigned*)wrapper:0;
        if(word(offset+0x3d00+stage*4)!=texture){word(offset+0x3d00+stage*4)=texture;bind((void*)word(offset+0x3cf0),stage,(void*)texture);}
        if((int)word(offset+0x3cb8)<(int)(stage+1))word(offset+0x3cb8)=stage+1;
    }
    for(unsigned i=0;i<word(offset+0x280c);++i){
        unsigned char* info=(unsigned char*)word(offset+0x2008+i*4);unsigned* values=(unsigned*)info;
        info[0x10]=0;
        if(values[0]!=values[1]){state((void*)word(offset+4),values[3],values[1]);values[0]=values[1];}
    }
    word(offset+0x280c)=0;
    draw((void*)word(offset+0x3cf0),4,0,9,3,arena+0x5d00,0x65,arena+0x5c00,28);
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,WIDTH,0x3000,4);require(arena==(void*)0x24000000);
    unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+realiseFixup)=(unsigned)(code+256)-(unsigned)(code+realiseFixup+4);
    typedef void (__fastcall* Method)(void*,void*,void*,unsigned,void*,long,long,void*,long);
    for(textureCount=0;textureCount<=8;++textureCount)for(unsigned pattern=0;pattern<4;++pattern)for(mode=0;mode<8;++mode)for(unsigned place=0;place<2;++place)for(dirty=0;dirty<3;dirty+=2){
        offset=place?0x1000:0;unsigned counts[3];
        for(run=0;run<3;++run){
            memset(arena,pattern&1?0x95:0,WIDTH);word(offset+4)=word(offset+0x3cf0)=(unsigned)(arena+0x6800);
            word(0x6800)=word(0x6840)=(unsigned)(arena+0x7000);
            word(0x7000+65*4)=(unsigned)bind;word(0x7000+57*4)=(unsigned)state;word(0x7000+84*4)=(unsigned)draw;
            word(offset+0x3cb8)=pattern?pattern-1:0xffffffff;word(offset+0x280c)=dirty;
            for(unsigned stage=0;stage<8;++stage){
                unsigned wrapper=(unsigned)(arena+0x5200+stage*8),texture=(unsigned)(arena+0x5600+(stage%3)*16);
                word(0x5200+stage*8)=pattern==2&&stage%2?0:texture;
                word(0x5100+stage*4)=pattern==3&&stage%2?0:wrapper;
                word(offset+0x3d00+stage*4)=pattern==1?texture:0;
            }
            for(unsigned s=0;s<2;++s){unsigned at=0x5400+s*0x14;word(offset+0x2008+s*4)=(unsigned)(arena+at);word(at)=pattern==2?1:0;word(at+4)=1;word(at+12)=7+s;arena[at+16]=1;arena[at+17]=1;}
            events=0;
            if(run==2)model();else{Method method=run?DrawTextures:(Method)code;method(arena+offset,0,arena+0x5100,textureCount,arena+0x5c00,28,9,arena+0x5d00,3);}
            record(4,0,0);counts[run]=events;
        }
        require(counts[0]==counts[1]&&counts[1]==counts[2]);
        require(!memcmp(snapshots[0],snapshots[1],counts[0]*sizeof(Snapshot)));require(!memcmp(snapshots[0],snapshots[2],counts[0]*sizeof(Snapshot)));++cases;
    }
    VirtualFree(code,0,0x8000);VirtualFree(arena,0,0x8000);printf("NATIVE_INDEXED_TEXTURES_PASS cases=%u snapshots=%u\n",cases,total);return 0;
}
