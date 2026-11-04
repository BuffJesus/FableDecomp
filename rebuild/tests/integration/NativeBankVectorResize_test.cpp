#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
#define CHECK(x) do {if(!(x)){fprintf(stderr,"VECTOR_RESIZE_FAILURE line=%d\n",__LINE__);exit(80);}}while(0)
static unsigned kind,oldSize,newSize,width,fills,erases,constructs,destroys;static unsigned* vectorWords;static unsigned char* data;static const void* value;
static void Fill(void* self,void* position,unsigned n,const void* fillValue){CHECK(self==vectorWords && position==data+oldSize*width && n==newSize-oldSize);CHECK(kind==3?*static_cast<const unsigned*>(fillValue)==0x11223344:fillValue==value);unsigned char saved[12];memcpy(saved,fillValue,width);for(unsigned i=0;i<n;++i)memcpy(static_cast<unsigned char*>(position)+i*width,saved,width);vectorWords[1]+=n*width;++fills;}
namespace _STL {
template<> void vector<CBankFile::CRuntimeData>::_M_fill_insert(CBankFile::CRuntimeData* p,size_t n,const CBankFile::CRuntimeData& v){CHECK(kind==0);Fill(this,p,n,&v);}
template<> void vector<CBankFileEntryUpdateData*>::_M_fill_insert(CBankFileEntryUpdateData** p,size_t n,CBankFileEntryUpdateData* const& v){CHECK(kind==1);Fill(this,p,n,&v);}
template<> void vector<CCharString>::_M_fill_insert(CCharString* p,size_t n,const CCharString& v){CHECK(kind==3);Fill(this,p,n,&v);}
}
CCharString::CCharString(){*reinterpret_cast<unsigned*>(this)=0x11223344;++constructs;}
CCharString::~CCharString(){CHECK(*reinterpret_cast<unsigned*>(this)==0x11223344);++destroys;}
extern "C" void* __fastcall EraseStrings(void* self,void*,void* first,void* last){CHECK(kind==3 && self==vectorWords && first==data+newSize*width && last==data+oldSize*width);vectorWords[1]=reinterpret_cast<unsigned>(first);++erases;return first;}
#pragma comment(linker,"/alternatename:?erase@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEPAVCCharString@@PAV3@0@Z=@EraseStrings@16")
extern "C" void __fastcall RuntimeResize(void*,void*,unsigned,const void*);
extern "C" void __fastcall UpdateResize(void*,void*,unsigned,const void*);
extern "C" void __fastcall ChecksumResize(void*,void*,unsigned,const void*);
extern "C" void __fastcall SymbolResize(void*,void*,unsigned);
#pragma comment(linker,"/alternatename:@RuntimeResize@16=?resize@?$vector@UCRuntimeData@CBankFile@@V?$allocator@UCRuntimeData@CBankFile@@@_STL@@@_STL@@QAEXIABUCRuntimeData@CBankFile@@@Z")
#pragma comment(linker,"/alternatename:@UpdateResize@16=?resize@?$vector@PAVCBankFileEntryUpdateData@@V?$allocator@PAVCBankFileEntryUpdateData@@@_STL@@@_STL@@QAEXIABQAVCBankFileEntryUpdateData@@@Z")
#pragma comment(linker,"/alternatename:@ChecksumResize@16=?resize@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXIABK@Z")
#pragma comment(linker,"/alternatename:@SymbolResize@12=?resize@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEXI@Z")
int main(){unsigned cases=0;unsigned sizes[]={0,1,3,7,9};
 for(kind=0;kind<4;++kind)for(unsigned a=0;a<5;++a)for(unsigned z=0;z<5;++z)for(unsigned alias=0;alias<2;++alias){
  oldSize=sizes[a];newSize=sizes[z];width=kind==0?12:4;unsigned words[5]={0xcdcdcdcd,0,0,0,0xcdcdcdcd};unsigned char storage[224],expected[224];for(unsigned i=0;i<sizeof(storage);++i)storage[i]=static_cast<unsigned char>(i*37+13);memcpy(expected,storage,sizeof(storage));data=storage+16;vectorWords=words+1;vectorWords[0]=reinterpret_cast<unsigned>(data);vectorWords[1]=vectorWords[0]+oldSize*width;vectorWords[2]=vectorWords[0]+16*width;
  unsigned pattern[]={0x12345678,0x87654321,0x11224488};value=alias?static_cast<const void*>(data):static_cast<const void*>(pattern);unsigned char saved[12];if(kind==3){unsigned sentinel=0x11223344;memcpy(saved,&sentinel,4);}else memcpy(saved,value,width);if(newSize>oldSize)for(i=oldSize;i<newSize;++i)memcpy(expected+16+i*width,saved,width);
  fills=erases=constructs=destroys=0;
  switch(kind){case 0:RuntimeResize(vectorWords,0,newSize,value);break;case 1:UpdateResize(vectorWords,0,newSize,value);break;case 2:ChecksumResize(vectorWords,0,newSize,value);break;case 3:SymbolResize(vectorWords,0,newSize);break;}
  CHECK(vectorWords[0]==reinterpret_cast<unsigned>(data) && vectorWords[1]==vectorWords[0]+newSize*width && vectorWords[2]==vectorWords[0]+16*width && words[0]==0xcdcdcdcd && words[4]==0xcdcdcdcd);CHECK(!memcmp(storage,expected,sizeof(storage)));CHECK(fills==(kind!=2 && newSize>=oldSize?1:0) && erases==(kind==3 && newSize<oldSize?1:0) && constructs==(kind==3?1:0) && destroys==constructs);++cases;
 }
 printf("NATIVE_BANK_VECTOR_RESIZE_PASS cases=%u\n",cases);return 0;
}
