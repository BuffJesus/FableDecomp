#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_attachment_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* AttachmentGlobal;
extern "C" void __fastcall Attach(void*,void*,void*,void*,void*,void*,long,unsigned long);
typedef void (__fastcall *AttachFn)(void*,void*,void*,void*,void*,void*,long,unsigned long);
static unsigned char arena[256],expected[256],last[256];
static void *manager,*handle,*primitive,*info,*parent;
static unsigned cases,snapshots,stage,calls,mode;static long bone;static unsigned long flags;
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static void Record(){REQUIRE(!memcmp(arena,expected,256)&&AttachmentGlobal==manager);++snapshots;}
extern "C" void __fastcall SetAttached(void* self,void*,void* h,void* p,void* i,void* par,long b,unsigned long f){
    REQUIRE(self==manager&&h==handle&&p==primitive&&i==info&&par==parent&&b==bone&&f==flags&&calls++==stage);Record();
    if(mode==1)AttachmentGlobal=manager=manager==arena+128?arena+192:arena+128;
    if(mode==2){unsigned off=(unsigned char*)handle-arena;arena[off+4]=expected[off+4]=(unsigned char)(flags+stage);AttachmentGlobal=manager=arena+192;}
    Record();
}
int main(){
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+2)=(unsigned)&AttachmentGlobal;
    *(unsigned*)(code+7)=(unsigned)&SetAttached-(unsigned)(code+11);
    const unsigned values[]={0,1,0x80000000,0xffffffff};
    for(unsigned placement=0;placement<2;++placement)for(unsigned alias=0;alias<4;++alias)
    for(unsigned b=0;b<4;++b)for(unsigned f=0;f<4;++f)for(mode=0;mode<3;++mode){
        for(unsigned run=0;run<2;++run){
            for(unsigned k=0;k<256;++k)arena[k]=(unsigned char)(k*37+f*19);
            memcpy(expected,arena,256);AttachmentGlobal=manager=arena+128+placement*64;
            handle=arena;primitive=alias==1?handle:arena+16;info=alias==2?primitive:arena+32;parent=alias==3?handle:arena+48;
            bone=(long)values[b];flags=values[f];calls=0;
            for(stage=0;stage<3;++stage){(run?Attach:(AttachFn)code)(arena+96,0,handle,primitive,info,parent,bone,flags);Record();}
            REQUIRE(calls==3);if(run)REQUIRE(!memcmp(arena,last,256));memcpy(last,arena,256);
        }++cases;
    }
    printf("NATIVE_PRIMITIVE_ATTACHMENT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
