#include "fable_game_text_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::pair<unsigned long,unsigned long> Pair;
typedef _STL::pair<const CCharString,unsigned long> StringPair;
typedef _STL::_Rb_tree<CCharString,StringPair,_STL::_Select1st<StringPair>,_STL::less<CCharString>,_STL::allocator<StringPair> > Tree;
static void require(bool ok){if(!ok){fprintf(stderr,"INIT_CRC_FAILURE\n");exit(80);}}
struct Block {unsigned char* p;size_t n;bool freed;};static Block blocks[64];static unsigned allocated;
extern "C" void* __cdecl CRCMalloc(size_t n){require(n&&n<2048&&allocated<64);unsigned char* p=(unsigned char*)malloc(n+32);require(p!=0);memset(p,0x7d,n+32);Block& b=blocks[allocated++];b.p=p+16;b.n=n;b.freed=false;return b.p;}
extern "C" void __cdecl CRCFree(void* p){if(!p)return;Block* b=0;for(unsigned i=0;i<allocated;++i)if(blocks[i].p==p&&!blocks[i].freed)b=&blocks[i];require(b!=0);for(int i=0;i<16;++i)require(b->p[i-16]==0x7d&&b->p[b->n+i]==0x7d);b->freed=true;free(b->p-16);}
char g_FableEmptyNarrowString_0129AAF4[1]={0};
template<> unsigned long& _STL::map<CCharString,unsigned long>::operator[](const CCharString&){require(false);static unsigned long n;return n;}
template<> Tree::iterator Tree::insert_equal(const StringPair&){require(false);return Tree::iterator();}
CCharString::CCharString(const CCharString&){require(false);}
CCharString::~CCharString(){require(false);}
CCharString CCharString::GetAsLowerCase()const{require(false);return *this;}
CCharString CBankFile::ConvertPathToFilename(const CCharString& s)const{require(false);return s;}
extern "C" void __fastcall CallInit(CBankFile*,void*,unsigned long,const CCharString&,const CCharString&);
#pragma comment(linker,"/alternatename:@CallInit@20=?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z")
static unsigned crc(const unsigned char* p,unsigned n){unsigned value=0;for(unsigned i=0;i<n;++i){value^=p[i];for(int bit=0;bit<8;++bit)value=(value>>1)^((value&1)?0xedb88320UL:0);}return value;}
int main(){unsigned cases=0;const unsigned caps[]={0,1,7,33},lengths[]={0,1,2,3,7,15,31,64};
 for(unsigned count=0;count<=32;++count)for(unsigned cap=0;cap<4;++cap)for(unsigned pattern=0;pattern<4;++pattern)for(unsigned nulls=0;nulls<2;++nulls){
  union {double align;char bytes[sizeof(CBankFile)];} storage;memset(&storage,0,sizeof(storage));CBankFile* bank=(CBankFile*)&storage;bank->OpenFlags=256;allocated=0;
  unsigned* vector=(unsigned*)((char*)bank+0xd0);if(caps[cap]){vector[0]=(unsigned)CRCMalloc(caps[cap]*8);vector[1]=vector[0];vector[2]=vector[0]+caps[cap]*8;}
  Pair expected[32];unsigned char bytes[65],before[65];CCharStringData data;
  for(unsigned i=0;i<count;++i){unsigned length=lengths[(i+pattern)%8];for(unsigned j=0;j<65;++j)bytes[j]=(unsigned char)(pattern==0?0:pattern==1?j:pattern==2?(j*71+i*19)^0xa5:255-j);memcpy(before,bytes,65);
   data.Data.PData=(char*)bytes;data.Data.StringLength=length;data.Data.DataLength=65;data.Data.UseFastExtend=true;data.owners=1;void* value=nulls&&(i&1)?0:&data;unsigned payload=0xffffffffUL-i;
   expected[i].first=crc(bytes,value?length:0);expected[i].second=payload;CallInit(bank,0,payload,*(CCharString*)&value,*(CCharString*)&value);
   require(vector[1]-vector[0]==(i+1)*8&&vector[1]<=vector[2]&&bank->SymbolCRCIndices.Dirty&&memcmp((void*)vector[0],expected,(i+1)*8)==0&&data.owners==1&&data.Data.PData==(char*)bytes&&data.Data.StringLength==length&&memcmp(bytes,before,65)==0);
  }
  require(bank->SymbolCRCIndices.Dirty==(count!=0));CRCFree((void*)vector[0]);for(unsigned i=0;i<allocated;++i)require(blocks[i].freed);++cases;
 }
 printf("NATIVE_INIT_CRC_PASS cases=%u\n",cases);return 0;
}
