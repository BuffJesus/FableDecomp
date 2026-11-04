#include "fable_symbol_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
typedef CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> > Map;
struct Ref{int owners;};static Ref refs[8];static int copies,destroyed;
static Ref* raw(const CCharString& s){return *(Ref* const*)&s;}
CCharString::CCharString(const CCharString& other){storage_=other.storage_;if(raw(*this))++raw(*this)->owners;++copies;}
CCharString::~CCharString(){if(raw(*this))--raw(*this)->owners;storage_=0;++destroyed;}
CCharString& CCharString::operator=(const CCharString& other){if(this!=&other){if(raw(other))++raw(other)->owners;if(raw(*this))--raw(*this)->owners;storage_=other.storage_;}return *this;}
int main(){int cases=0;
for(int n=0;n<=16;++n)for(int spare=0;spare<5;++spare)for(int pattern=0;pattern<4;++pattern){
 union{double align;char bytes[16];} storage;memset(&storage,0x9d,sizeof(storage));Map* map=(Map*)&storage;int cap=n+spare;Entry* original=cap?(Entry*)malloc(cap*sizeof(Entry)):0;map->_M_start=original;map->_M_finish=original+n;map->_M_end_of_storage._M_data=original+cap;map->Dirty=(pattern&1)!=0;
 for(int r=0;r<8;++r)refs[r].owners=1;
 for(int i=0;i<n;++i){for(int j=0;j<3;++j){Ref* p=pattern==0?0:&refs[(pattern==1?0:i*3+j)%8];*(Ref**)((char*)&original[i]+j*4)=p;if(p)++p->owners;}original[i].second.Val=i*31-7;}
 int expected[8];for(int r=0;r<8;++r)expected[r]=refs[r].owners;unsigned char tail[4];memcpy(tail,storage.bytes+12,4);copies=destroyed=0;
 map->DeallocateUnusedMemory();Entry* current=map->_M_start;
 if(map->size()!=(unsigned)n||map->capacity()!=(unsigned)(spare?n:cap)||copies!=(spare?3*n:0)||destroyed!=copies)return 1;
 if(spare&&cap&&current==original)return 2;
 for(int i=0;i<n;++i){for(int j=0;j<3;++j){Ref* p=pattern==0?0:&refs[(pattern==1?0:i*3+j)%8];if(*(Ref**)((char*)&current[i]+j*4)!=p)return 3;}if(current[i].second.Val!=i*31-7)return 4;}
 for(int r=0;r<8;++r)if(refs[r].owners!=expected[r])return 5;
 if(memcmp(tail,storage.bytes+12,4))return 6;
 int priorCopies=copies,priorDestroyed=destroyed;map->DeallocateUnusedMemory();if(map->_M_start!=current||copies!=priorCopies||destroyed!=priorDestroyed)return 7;
 map->DeallocateMemory();if(map->_M_start||map->_M_finish||map->_M_end_of_storage._M_data||destroyed!=priorDestroyed+3*n)return 8;
 for(int r=0;r<8;++r)if(refs[r].owners!=1)return 9;
 ++cases;
}
printf("SYMBOL_MAP_MEMORY_CHAIN_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
