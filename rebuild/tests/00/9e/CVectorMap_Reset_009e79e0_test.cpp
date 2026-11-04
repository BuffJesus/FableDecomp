#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_symbol_map.h"
typedef CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> > Map;
static union{double align;char bytes[16];} object;
static union{double align;char bytes[16*24];} elements;
static Map* owner;static int calls,frees;static void* released;static unsigned long offsets[64];
extern "C" void __cdecl free(void* p){if(owner){++frees;released=p;}}
CCharString::~CCharString(){if(owner){if(calls>=64)abort();offsets[calls++]=(char*)this-elements.bytes;storage_=0;}}
CCharString& CCharString::operator=(const CCharString&){abort();return *this;}
int main(){int cases=0;
for(int kind=0;kind<2;++kind)for(int n=0;n<=16;++n)for(int spare=0;spare<4;++spare)for(int pattern=0;pattern<4;++pattern)for(int nullStore=0;nullStore<(n==0&&spare==0?2:1);++nullStore){
 memset(&object,0x9d,sizeof(object));memset(&elements,0x20+pattern,sizeof(elements));owner=(Map*)&object;
 _STL::pair<CCharString,CSymbolMap::CSymbolInfo>* start=nullStore?0:(_STL::pair<CCharString,CSymbolMap::CSymbolInfo>*)elements.bytes;
 owner->_M_start=start;owner->_M_finish=start+n;owner->_M_end_of_storage._M_data=start+n+spare;owner->Dirty=(pattern&1)!=0;
 unsigned char tail[4];memcpy(tail,object.bytes+12,4);calls=frees=0;released=0;
 if(kind)owner->DeallocateMemory();else owner->CArrayVector<_STL::pair<CCharString,CSymbolMap::CSymbolInfo> >::DeallocateMemory();
 if(kind||n+spare){if(owner->_M_start||owner->_M_finish||owner->_M_end_of_storage._M_data||calls!=3*n||frees!=(start!=0)||(start&&released!=start))return 1;}
 else if(owner->_M_start!=start||owner->_M_finish!=start||owner->_M_end_of_storage._M_data!=start||calls||frees)return 2;
 if(kind)tail[1]=0;
 if(memcmp(tail,object.bytes+12,4))return 3;
 for(int i=0;i<n;++i)for(int j=0;j<3;++j)if(offsets[3*i+j]!=(unsigned long)(i*16+8-j*4))return 4;
 owner=0;++cases;
}printf("SYMBOL_MAP_RESET_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
