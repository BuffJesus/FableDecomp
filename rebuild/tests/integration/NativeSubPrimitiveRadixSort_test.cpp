#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subprimitive_radix_sort_aliases.h"
#include "subprimitive_radix_sort_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __cdecl _chkstk();
extern "C" void __fastcall RadixSort(void*,void*,void*);
static unsigned char *arena,initial[65536],expected[65536];
static unsigned cases,snapshots,count,oldSize,mode,calls;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
static unsigned Ptr(unsigned o){return (unsigned)(arena+o);}
static unsigned Off(unsigned p){return p-(unsigned)arena;}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static void Resize(unsigned char*a){
    unsigned start=W(a,0x180);
    if(count>oldSize&&mode==1){start=Ptr(0x7000);W(a,0x180)=start;W(a,0x188)=Ptr(0xa000);}
    if(count>=oldSize&&count>oldSize)memset(a+Off(start)+oldSize*8,0xab,(count-oldSize)*8);
    W(a,0x184)=start+count*8;
}
extern "C" void* __fastcall Erase(void*self,void*,void*first,void*last){
    REQUIRE(calls++==0&&self==arena+0x180&&count<oldSize&&first==(void*)(W(arena,0x180)+count*8)&&last==(void*)W(arena,0x184)&&!memcmp(arena,initial,sizeof(initial)));Resize(arena);return first;
}
extern "C" void __fastcall Fill(void*self,void*,void*position,unsigned n,const void*value){
    REQUIRE(calls++==0&&self==arena+0x180&&count>=oldSize&&position==(void*)W(arena,0x184)&&n==count-oldSize&&value&&!memcmp(arena,initial,sizeof(initial)));Resize(arena);
}
static void Model(){
    Resize(expected);unsigned dst=Off(W(expected,0x180));
    // Independent stable insertion sort of the complete low30-bit key.
    for(unsigned i=0;i<count;++i){unsigned key=W(initial,0x1000+i*8),value=W(initial,0x1004+i*8),j=i;
        while(j&&(W(expected,dst+(j-1)*8)&0x3fffffff)>(key&0x3fffffff)){W(expected,dst+j*8)=W(expected,dst+(j-1)*8);W(expected,dst+j*8+4)=W(expected,dst+(j-1)*8+4);--j;}
        W(expected,dst+j*8)=key;W(expected,dst+j*8+4)=value;
    }
    memcpy(expected+0x1000,expected+dst,count*8);
}
int main(){
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
    unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[]={(unsigned)&_chkstk,(unsigned)&Erase,(unsigned)&Fill};
    for(unsigned i=0;i<3;++i){const unsigned*f=retailFixups[i];*(unsigned*)(code+f[0])=targets[f[1]]-(unsigned)(code+f[0]+4);}
    const unsigned sizes[]={0,1,2,15,16,17,255,256,257,1023,1024,1025};
    for(unsigned n=0;n<12;++n)for(unsigned pattern=0;pattern<8;++pattern)for(unsigned old=0;old<4;++old)for(mode=0;mode<2;++mode){
        count=sizes[n];oldSize=old==0?0:old==1?(count?count-1:0):old==2?count:count+1;
        for(i=0;i<65536;++i)arena[i]=(unsigned char)(i*13+pattern*29);
        W(arena,0x100)=Ptr(0x1000);W(arena,0x104)=Ptr(0x1000+count*8);W(arena,0x108)=Ptr(0x4000);
        W(arena,0x180)=Ptr(0x4000);W(arena,0x184)=Ptr(0x4000+oldSize*8);W(arena,0x188)=Ptr(0x7000);
        unsigned random=0x87654321+mode;
        for(i=0;i<count;++i){random=random*1664525+1013904223;unsigned key;
            switch(pattern){case 0:key=0;break;case 1:key=0xffffffff;break;case 2:key=i;break;case 3:key=count-i;break;case 4:key=(i%4)*0x40100401;break;case 5:key=random;break;case 6:key=(i&1)?0x3fffffff:0xc0000000;break;default:key=(random%7)*0x100400+((i%3)<<30);}
            W(arena,0x1000+i*8)=key;W(arena,0x1004+i*8)=i;
        }
        memcpy(initial,arena,sizeof(initial));memcpy(expected,initial,sizeof(initial));Model();
        for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));calls=0;typedef void(__fastcall*Fn)(void*,void*,void*);(run?RadixSort:(Fn)code)(arena,0,arena+0x100);if(calls!=1||memcmp(arena,expected,sizeof(expected))){fprintf(stderr,"run=%u calls=%u count=%u old=%u mode=%u\n",run,calls,count,oldSize,mode);for(unsigned q=0;q<65536;++q)if(arena[q]!=expected[q]){fprintf(stderr,"difference=%x actual=%x expected=%x\n",q,arena[q],expected[q]);break;}}REQUIRE(calls==1&&!memcmp(arena,expected,sizeof(expected)));++snapshots;}++cases;
    }
    printf("NATIVE_SUBPRIMITIVE_RADIX_SORT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
