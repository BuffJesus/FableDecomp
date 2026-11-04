#include "fable_packed_uint_array.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void __fastcall ClearPackedBankArray(CPackedUIntArray*);
static void require(bool ok){if(!ok){fprintf(stderr,"PACKED_INITIALISE_FAILURE\n");exit(80);}}
struct Block {unsigned char* p;unsigned n;bool freed;};static Block blocks[4];static unsigned used;
void* __cdecl operator new[](size_t n){require(n&&n<=1024&&used<4);unsigned char* p=(unsigned char*)malloc(n+32);require(p!=0);memset(p,0x7d,n+32);Block& b=blocks[used++];b.p=p+16;b.n=n;b.freed=false;return b.p;}
void __cdecl operator delete[](void* p){if(!p)return;Block* b=0;for(unsigned i=0;i<used;++i)if(blocks[i].p==p&&!blocks[i].freed)b=&blocks[i];require(b!=0);for(int i=0;i<16;++i)require(b->p[i-16]==0x7d&&b->p[b->n+i]==0x7d);b->freed=true;free(b->p-16);}
static unsigned mask(unsigned bits){return bits==32?0xffffffffUL:bits?(1UL<<bits)-1:0;}
int main(){unsigned cases=0,carry32=0;unsigned widths[]={0,1,7,8,9,15,16,17,24,31,32,40},counts[]={0,1,2,3,7,16,33};long strides[]={0,5,12,-5};
 for(unsigned w=0;w<12;++w)for(unsigned c=0;c<7;++c)for(unsigned t=0;t<4;++t)for(unsigned mode=0;mode<5;++mode)for(unsigned old=0;old<2;++old){
  union{double align;unsigned char bytes[16];} storage;memset(&storage,0x6b,sizeof(storage));CPackedUIntArray* packed=(CPackedUIntArray*)&storage;used=0;packed->PackedInts=old?new unsigned long[2]:0;packed->Size=0x76543210;packed->Bits=255;packed->Bias=0xa5a5a5a5;
  unsigned char bytes[1024],before[1024];memset(bytes,0x59,1024);unsigned count=counts[c],sourceBits=widths[w]>32?32:widths[w];long stride=strides[t];unsigned char* start=bytes+16+(stride<0&&count?5*(count-1):0);
  for(unsigned i=0;i<count;++i){unsigned value=mode==0?7:mode==1?i:mode==2?(i&1?0xffffffffUL:0):mode==3?(i*0x9e3779b9UL)^0x80ff21:0xffffff00UL+i;memcpy(start+i*stride,&value,4);}memcpy(before,bytes,1024);
  unsigned values[33],bias=count?0xffffffffUL:0,combined=0,bits=0,expected[33]={0};
  for(unsigned i=0;i<count;++i){unsigned value=0;memcpy(&value,start+i*stride,(sourceBits+7)/8);values[i]=value&mask(sourceBits);if(values[i]<bias)bias=values[i];}
  for(unsigned i=0;i<count;++i)combined|=values[i]-bias;while(combined){++bits;combined>>=1;}unsigned words=(bits*count+31)/32;
  if(bits==32){for(unsigned i=0;i<count;++i)expected[i]=(values[i]-bias)|(i?values[i-1]-bias:0);++carry32;}
  else for(unsigned i=0;i<count;++i)for(unsigned bit=0;bit<bits;++bit)if((values[i]-bias)&(1UL<<bit))expected[(i*bits+bit)/32]|=1UL<<((i*bits+bit)%32);
  packed->Initialise(start,widths[w],stride,count);
  require(packed->Size==count&&packed->Bias==bias&&packed->Bits==bits&&packed->Sizeof()==16+words*4&&memcmp(bytes,before,1024)==0&&storage.bytes[9]==0x6b&&storage.bytes[10]==0x6b&&storage.bytes[11]==0x6b);
  if(words)require(packed->PackedInts&&memcmp(packed->PackedInts,expected,words*4)==0);else require(!packed->PackedInts);
  for(unsigned i=0;i<count;++i){unsigned decoded=0;for(unsigned bit=0;bit<bits;++bit)if(expected[(i*bits+bit)/32]&(1UL<<((i*bits+bit)%32)))decoded|=1UL<<bit;require((*packed)[i]==decoded+bias);}
  ClearPackedBankArray(packed);require(!packed->PackedInts&&!packed->Size&&!packed->Bits&&!packed->Bias);for(unsigned i=0;i<used;++i)require(blocks[i].freed);++cases;
 }
 require(carry32>0);printf("PACKED_INITIALISE_PASS cases=%u carry32=%u\n",cases,carry32);return 0;
}
