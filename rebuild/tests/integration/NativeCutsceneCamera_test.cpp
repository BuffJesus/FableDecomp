#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cutscene_camera_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall AppendCamera(void*,void*,const void*);
typedef void (__fastcall *AppendFn)(void*,void*,const void*);
static unsigned char *arena,expected[2048],previous[2048];
static unsigned count,capacity,placement,alias,bits,fill,cases,calls;
static unsigned engine,vectorOffset,source;
#define REQUIRE(x) do { if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);} }while(0)
static unsigned& W(unsigned char* a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Offset(unsigned address){return address-(unsigned)arena;}
static void CopyModel(unsigned char* out,unsigned dst,unsigned src) {
    unsigned char value[56];memcpy(value,out+src,56);
    for(unsigned i=0;i<56;++i)if(i!=38&&i!=39)out[dst+i]=value[i];
}
extern "C" void __fastcall Overflow(void* self,void*,void* pos,const void* value,const void* tag,unsigned amount,unsigned atEnd) {
    REQUIRE(self==arena+vectorOffset&&pos==(void*)W(arena,vectorOffset+4));
    REQUIRE(value==arena+source&&tag&&amount==1&&atEnd==1);
    REQUIRE(!memcmp(arena,expected,2048));++calls;
    // Explicit allocator/overflow boundary: supply fresh storage and append.
    unsigned old=Offset(W(arena,vectorOffset)),finish=Offset(W(arena,vectorOffset+4));
    unsigned size=(finish-old)/56,next=1024;
    for(unsigned i=0;i<size;++i) {CopyModel(arena,next+i*56,old+i*56);CopyModel(expected,next+i*56,old+i*56);}
    CopyModel(arena,next+size*56,source);CopyModel(expected,next+size*56,source);
    W(arena,vectorOffset)=W(expected,vectorOffset)=(unsigned)(arena+next);
    W(arena,vectorOffset+4)=W(expected,vectorOffset+4)=(unsigned)(arena+next+(size+1)*56);
    W(arena,vectorOffset+8)=W(expected,vectorOffset+8)=(unsigned)(arena+next+(size+3)*56);
}
int main() {
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,4096,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+24)=(unsigned)(code+64)-(unsigned)(code+28);
    *(unsigned*)(code+54)=(unsigned)&Overflow-(unsigned)(code+58);
    const unsigned patterns[]={0,0x80000000,1,0x7f800000,0x7fc12345,0x7f812345,0xffffffff};
    for(count=0;count<4;++count)for(unsigned spare=0;spare<3;++spare)
    for(placement=0;placement<2;++placement)for(alias=0;alias<2;++alias)
    for(bits=0;bits<7;++bits)for(fill=0;fill<4;++fill) {
        capacity=count+spare;engine=placement*128;vectorOffset=engine+60;
        source=alias&&count?512:1792;
        for(unsigned run=0;run<2;++run) {
            for(unsigned i=0;i<2048;++i)arena[i]=(unsigned char)(i*13+fill*71);
            for(i=0;i<14;++i)W(arena,source+i*4)=patterns[(bits+i)%7];
            W(arena,vectorOffset)=(unsigned)(arena+512);
            W(arena,vectorOffset+4)=(unsigned)(arena+512+count*56);
            W(arena,vectorOffset+8)=(unsigned)(arena+512+capacity*56);
            memcpy(expected,arena,2048);calls=0;
            for(unsigned step=0;step<2;++step) {
                if(W(expected,vectorOffset+4)!=W(expected,vectorOffset+8)) {
                    CopyModel(expected,Offset(W(expected,vectorOffset+4)),source);
                    W(expected,vectorOffset+4)+=56;
                }
                (run?AppendCamera:(AppendFn)code)(arena+engine,0,arena+source);
                REQUIRE(!memcmp(arena,expected,2048));
            }
            REQUIRE(calls==(spare<2?1:0));
            if(run)REQUIRE(!memcmp(arena,previous,2048));
            memcpy(previous,arena,2048);
        }
        ++cases;
    }
    printf("NATIVE_CUTSCENE_CAMERA_PASS cases=%u snapshots=%u\n",cases,cases*4);return 0;
}
