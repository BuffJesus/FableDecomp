#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
static void require(bool ok,int line){if(!ok){fprintf(stderr,"BANK_PACK_RUNTIME_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
struct Block {unsigned char* p;unsigned n;bool freed;};static Block blocks[16];static unsigned used,deallocations;
void* __cdecl operator new[](size_t n){CHECK(n&&n<=1024&&used<16);unsigned char* p=(unsigned char*)malloc(n+32);CHECK(p!=0);memset(p,0x7d,n+32);Block& b=blocks[used++];b.p=p+16;b.n=n;b.freed=false;return b.p;}
void __cdecl operator delete[](void* p){if(!p)return;Block* b=0;for(unsigned i=0;i<used;++i)if(blocks[i].p==p&&!blocks[i].freed)b=&blocks[i];CHECK(b!=0);for(int i=0;i<16;++i)CHECK(b->p[i-16]==0x7d&&b->p[b->n+i]==0x7d);b->freed=true;free(b->p-16);}
static unsigned char bank[0x110],bankBefore[0x110],dataBefore[1200];static unsigned capacityBytes;
static unsigned* Words(void* p){return static_cast<unsigned*>(p);}
// Observed stand-in for the separately accepted runtime release body (009D3D90).
extern "C" void __fastcall FixtureDeallocateRuntime(void* self,void*){CHECK(self==bank+0x20);unsigned* v=Words(self);CHECK(v[0]&&memcmp(reinterpret_cast<void*>(v[0]),dataBefore,capacityBytes)==0);free(reinterpret_cast<void*>(v[0]));v[0]=v[1]=v[2]=0;++deallocations;}
#pragma comment(linker,"/alternatename:?DeallocateMemory@?$CArrayVector@UCRuntimeData@CBankFile@@@@QAEXXZ=@FixtureDeallocateRuntime@8")
extern "C" void __fastcall Pack(void*,void*);
#pragma comment(linker,"/alternatename:@Pack@8=?PackRuntimeData@CBankFile@@AAEXXZ")
static void Record(unsigned i,unsigned mode,unsigned* r){
 if(mode==0){r[0]=r[1]=r[2]=r[3]=0;}
 else if(mode==1){r[0]=i*16;r[1]=i;r[2]=i&3;r[3]=1;}
 else if(mode==2){r[0]=(i*0x9e3779b9UL)^0x5a5a1234UL;r[1]=i*0x85ebca6bUL;r[2]=(i*37)&0xff;r[3]=i&1;}
 else if(mode==3){r[0]=0x1000;r[1]=0x200;r[2]=7;r[3]=1;}
 else{r[0]=i&1?0xffffff00UL+i:0;r[1]=i&1?0x80000000UL:1;r[2]=i&1?0xff:0;r[3]=i%3==0;}
}
int main(){unsigned cases=0,none=0,packedCases=0,runtimeCases=0,counts[]={0,1,2,3,4,9,33,100},extras[]={0,1,3,60},offsets[]={0x38,0x48,0x58,0x68};
 for(unsigned c=0;c<8;++c)for(unsigned e=0;e<4;++e)for(unsigned mode=0;mode<5;++mode)for(unsigned old=0;old<2;++old){
  unsigned count=counts[c],capacity=count+extras[e],i,j;used=deallocations=0;capacityBytes=12*capacity;memset(bank,0x6b,sizeof(bank));
  unsigned char* data=capacity?(unsigned char*)malloc(capacityBytes):0;CHECK(!capacity||data);if(data)memset(data,0x59,capacityBytes);
  unsigned rows[100][4];for(i=0;i<count;++i){Record(i,mode,rows[i]);memcpy(data+12*i,rows[i],8);data[12*i+8]=(unsigned char)rows[i][2];data[12*i+9]=(unsigned char)rows[i][3];data[12*i+10]=data[12*i+11]=0xee;}
  if(capacityBytes)memcpy(dataBefore,data,capacityBytes);Words(bank+0x20)[0]=(unsigned)data;Words(bank+0x20)[1]=(unsigned)(data+12*count);Words(bank+0x20)[2]=(unsigned)(data+capacityBytes);
  for(j=0;j<4;++j){unsigned* a=Words(bank+offsets[j]);a[0]=old?(unsigned)new unsigned long[2]:0;a[1]=0x76543210+j;bank[offsets[j]+8]=(unsigned char)(0xf0+j);a[3]=0xa5a5a5a5;}
  memcpy(bankBefore,bank,sizeof(bank));Pack(bank,0);
  for(i=0;i<sizeof(bank);++i)if(i<0x20||(i>=0x2c&&i<0x38)||i>=0x78)CHECK(bank[i]==bankBefore[i]);
  if(count<=1){CHECK(!memcmp(bank,bankBefore,sizeof(bank))&&!deallocations&&(!capacityBytes||!memcmp(data,dataBefore,capacityBytes)));for(j=0;j<4;++j)delete[] (unsigned long*)Words(bank+offsets[j])[0];free(data);++none;++cases;continue;}
  unsigned bias[4],bits[4],words[4],expected[4][100],total=0;
  for(j=0;j<4;++j){bias[j]=0xffffffffUL;unsigned combined=0;for(i=1;i<count;++i)if(rows[i][j]<bias[j])bias[j]=rows[i][j];for(i=1;i<count;++i)combined|=rows[i][j]-bias[j];bits[j]=0;while(combined){++bits[j];combined>>=1;}words[j]=(bits[j]*(count-1)+31)/32;total+=16+4*words[j];memset(expected[j],0,sizeof(expected[j]));
   if(bits[j]<32)for(i=1;i<count;++i)for(unsigned bit=0;bit<bits[j];++bit)if((rows[i][j]-bias[j])&(1UL<<bit))expected[j][((i-1)*bits[j]+bit)/32]|=1UL<<(((i-1)*bits[j]+bit)%32);}
  bool kept=total<12*(capacity+1);CHECK(deallocations==(kept?1u:0u));
  if(kept){
   CHECK(!Words(bank+0x20)[0]&&!Words(bank+0x20)[1]&&!Words(bank+0x20)[2]);
   for(j=0;j<4;++j){CPackedUIntArray* a=reinterpret_cast<CPackedUIntArray*>(bank+offsets[j]);CHECK(a->Size==count-1&&a->Bits==bits[j]&&a->Bias==bias[j]&&a->Sizeof()==16+4*words[j]&&!memcmp(bank+offsets[j]+9,bankBefore+offsets[j]+9,3));
    if(words[j])CHECK(a->PackedInts!=0);else CHECK(!a->PackedInts);
    if(bits[j]<32){if(words[j])CHECK(!memcmp(a->PackedInts,expected[j],4*words[j]));for(i=1;i<count;++i)CHECK((*a)[i-1]==rows[i][j]);}
    delete[] a->PackedInts;}
   ++packedCases;
  }else{
   CHECK(!memcmp(bank+0x20,bankBefore+0x20,12)&&!memcmp(data,dataBefore,capacityBytes));
   for(j=0;j<4;++j){unsigned* a=Words(bank+offsets[j]);CHECK(!a[0]&&!a[1]&&!a[3]&&!bank[offsets[j]+8]);}
   free(data);++runtimeCases;
  }
  for(i=0;i<used;++i)CHECK(blocks[i].freed);++cases;
 }
 CHECK(none&&packedCases&&runtimeCases);printf("BANK_PACK_RUNTIME_PASS cases=%u none=%u packed=%u runtime=%u\n",cases,none,packedCases,runtimeCases);return 0;
}
