#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subprimitive_radix_aliases.h"
#include "subprimitive_radix_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __cdecl _chkstk();
extern "C" void __fastcall Radix(void*,void*,const void*,void*,unsigned long,unsigned long);
static unsigned char *arena,initial[65536],expected[65536];
static unsigned cases,snapshots;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
// Independent stable insertion sort by the selected ten-bit digit.
static void Model(unsigned src,unsigned dst,unsigned count,unsigned radix){
    for(unsigned i=0;i<count;++i){
        unsigned key=W(expected,src+i*8),value=W(expected,src+i*8+4),digit=(key>>(radix*10))&1023,j=i;
        while(j&&((W(expected,dst+(j-1)*8)>>(radix*10))&1023)>digit){W(expected,dst+j*8)=W(expected,dst+(j-1)*8);W(expected,dst+j*8+4)=W(expected,dst+(j-1)*8+4);--j;}
        W(expected,dst+j*8)=key;W(expected,dst+j*8+4)=value;
    }
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+retailFixup)=(unsigned)&_chkstk-(unsigned)(code+retailFixup+4);
    const unsigned sizes[]={0,1,2,3,15,16,17,255,256,257,1023,1024,1025};
    for(unsigned n=0;n<13;++n)for(unsigned pattern=0;pattern<8;++pattern)for(unsigned fill=0;fill<4;++fill)for(unsigned action=0;action<4;++action){
        unsigned count=sizes[n],random=0x87654321+fill;
        for(unsigned i=0;i<65536;++i)arena[i]=(unsigned char)(i*13+fill*29);
        for(i=0;i<count;++i){random=random*1664525+1013904223;unsigned key;
            switch(pattern){case 0:key=0;break;case 1:key=0xffffffff;break;case 2:key=i;break;case 3:key=count-i;break;case 4:key=(i%4)*0x40100401;break;case 5:key=random;break;case 6:key=(i&1)?0x3fffffff:0xc0000000;break;default:key=(random%7)*0x100400+((i%3)<<30);}
            W(arena,0x1000+i*8)=key;W(arena,0x1004+i*8)=i;
        }
        memcpy(initial,arena,sizeof(initial));
        for(unsigned run=0;run<2;++run){
            memcpy(arena,initial,sizeof(initial));memcpy(expected,initial,sizeof(initial));
            unsigned src=0x1000,dst=0x4000;
            for(unsigned pass=0;pass<(action==3?3:1);++pass){
                unsigned radix=action==3?pass:action;Model(src,dst,count,radix);
                typedef void(__fastcall*Fn)(void*,void*,const void*,void*,unsigned long,unsigned long);
                (run?Radix:(Fn)code)(arena,0,arena+src,arena+dst,count,radix);
                REQUIRE(!memcmp(arena,expected,sizeof(expected)));++snapshots;unsigned t=src;src=dst;dst=t;
            }
            if(action==3)for(i=1;i<count;++i){unsigned prev=W(arena,src+(i-1)*8)&0x3fffffff,next=W(arena,src+i*8)&0x3fffffff;REQUIRE(prev<=next);if(prev==next)REQUIRE(W(arena,src+(i-1)*8+4)<W(arena,src+i*8+4));}
        }++cases;
    }
    printf("NATIVE_SUBPRIMITIVE_RADIX_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
