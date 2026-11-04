#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
#define CHECK(x) do {if(!(x)){fprintf(stderr,"BANK_ALLOCATION_FAILURE line=%d\n",__LINE__);exit(80);}}while(0)
static unsigned char bank[0x130];static unsigned flags,count,freeCount,resizeCalls,treeCalls,stringDestructions;
static void* oldBlocks[4];static unsigned char oldMask;
static unsigned* Words(void* p){return static_cast<unsigned*>(p);}
static void SetArray(unsigned offset,unsigned n,unsigned width){unsigned* v=Words(bank+offset);void* p=n?malloc(n*width):0;CHECK(!n || p);if(p)memset(p,0xcd,n*width);v[0]=reinterpret_cast<unsigned>(p);v[1]=v[2]=v[0]+n*width;}
extern "C" void __cdecl FixtureBankAllocateFree(void* p){CHECK(p);unsigned i=0;while(i<4 && oldBlocks[i]!=p)++i;CHECK(i<4 && !(oldMask&(1<<i)));oldMask|=1<<i;++freeCount;free(p);}
static void Resize(void* self,unsigned n,unsigned width,unsigned offset,unsigned flag){CHECK(self==bank+offset && n==count && (!flag || flags&flag));unsigned* v=Words(self);CHECK(!v[0] && !v[1] && !v[2]);SetArray(offset,n,width);++resizeCalls;}
namespace _STL {
template<> void vector<CBankFile::CRuntimeData>::resize(size_t n,const CBankFile::CRuntimeData& value){CHECK(!value.DataOffset && !value.DataSize && !value.DataType && !value.Valid);Resize(this,n,12,0x20,0);for(unsigned i=0;i<n;++i){CBankFile::CRuntimeData* p=reinterpret_cast<CBankFile::CRuntimeData*>(Words(this)[0])+i;p->DataOffset=p->DataSize=0;p->DataType=0;p->Valid=false;}}
template<> void vector<CCharString>::resize(size_t n){Resize(this,n,4,8,8);}
template<> void vector<unsigned long>::resize(size_t n,const unsigned long& value){CHECK(!value);Resize(this,n,4,0x14,16);}
template<> void vector<CBankFileEntryUpdateData*>::resize(size_t n,CBankFileEntryUpdateData* const& value){CHECK(!value);Resize(this,n,4,0x2c,32);}
}
void (_STL::vector<CCharString>::*g_KeepStringResize)(size_t)=&_STL::vector<CCharString>::resize;
CCharString::~CCharString(){++stringDestructions;}
extern "C" void* __fastcall StringErase(void* self,void*,void* first,void* last){CHECK(self==bank+8 && first==reinterpret_cast<void*>(Words(self)[0]) && last==reinterpret_cast<void*>(Words(self)[1]));stringDestructions+=(static_cast<char*>(last)-static_cast<char*>(first))/4;Words(self)[1]=Words(self)[0];return first;}
extern "C" void __fastcall TreeErase(void* self,void*,void* root){CHECK(self==bank+0xac || self==bank+0xc4);unsigned* header=reinterpret_cast<unsigned*>(Words(self)[0]);CHECK(root==reinterpret_cast<void*>(header[1]) && Words(self)[1]==3);++treeCalls;}
#pragma comment(linker,"/alternatename:?erase@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEPAVCCharString@@PAV3@0@Z=@StringErase@16")
#pragma comment(linker,"/alternatename:?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@K@_STL@@@2@@Z=@TreeErase@12")
extern "C" void __fastcall Allocate(void*,void*,unsigned);
#pragma comment(linker,"/alternatename:@Allocate@12=?AllocateBuffers@CBankFile@@AAEXK@Z")
extern "C" void* __fastcall RuntimeCopy(void*,void*,void*,const void*,int*);
#pragma comment(linker,"/alternatename:@RuntimeCopy@20=??$__copy@PAUCRuntimeData@CBankFile@@PAU12@H@_STL@@YIPAUCRuntimeData@CBankFile@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z")
int main(){unsigned cases=0;unsigned offsets[]={0x20,8,0x14,0x2c};unsigned widths[]={12,4,4,4};
 unsigned copyCases=0;for(unsigned c=0;c<3;++c)for(unsigned same=0;same<2;++same){unsigned n=c==2?5:c;unsigned from[23],to[23];for(unsigned i=0;i<23;++i){from[i]=i*37+13;to[i]=0xcdcdcdcd;}unsigned* out=same?from+4:to+4;char tag=0;CHECK(RuntimeCopy(from+4,from+4+n*3,out,&tag,0)==out+n*3);for(i=0;i<23;++i){CHECK(from[i]==i*37+13);CHECK(to[i]==(!same && i>=4 && i<4+n*3?i*37+13:0xcdcdcdcd));}++copyCases;}
 for(unsigned n=0;n<3;++n)for(unsigned f=0;f<8;++f)for(unsigned old=0;old<2;++old)for(unsigned maps=0;maps<4;++maps){
  memset(bank,0xa5,sizeof(bank));count=n==2?7:n;flags=f*8;Words(bank)[1]=99;Words(bank+0x78)[0]=flags;freeCount=resizeCalls=treeCalls=stringDestructions=oldMask=0;
  for(unsigned i=0;i<4;++i){SetArray(offsets[i],old?3:0,widths[i]);oldBlocks[i]=reinterpret_cast<void*>(Words(bank+offsets[i])[0]);}
  unsigned headers[2][5];for(i=0;i<2;++i){for(unsigned j=0;j<5;++j)headers[i][j]=0x12340000+i*16+j;unsigned* map=Words(bank+(i?0xc4:0xac));map[0]=reinterpret_cast<unsigned>(headers[i]);map[1]=maps&(1<<i)?3:0;}
  Allocate(bank,0,count);CHECK(Words(bank)[1]==count && freeCount==old*4 && oldMask==(old?15:0) && stringDestructions==old*3);CHECK(resizeCalls==1+!!(flags&8)+!!(flags&16)+!!(flags&32));CHECK(treeCalls==!!(maps&1)+!!(maps&2));
  for(i=0;i<2;++i){unsigned* map=Words(bank+(i?0xc4:0xac));if(maps&(1<<i)){CHECK(!map[1] && !headers[i][1] && headers[i][2]==map[0] && headers[i][3]==map[0]);}else CHECK(headers[i][1]==0x12340001+i*16);}
  for(i=0;i<4;++i){unsigned* v=Words(bank+offsets[i]);unsigned expected=i==0 || flags&(1<<(i+2))?count:0;CHECK(v[1]-v[0]==expected*widths[i] && v[2]==v[1]);if(v[0])free(reinterpret_cast<void*>(v[0]));}
  for(i=0x110;i<sizeof(bank);++i)CHECK(bank[i]==0xa5);++cases;
 }
 printf("NATIVE_BANK_ALLOCATION_PASS cases=%u copy_cases=%u\n",cases,copyCases);return 0;
}
