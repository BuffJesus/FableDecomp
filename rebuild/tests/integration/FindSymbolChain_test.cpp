#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
int main(){static union{double align;char bytes[16*16];} data;static union{double align;char bytes[sizeof(CSymbolMap)];} mapStore;Entry* entries=(Entry*)&data;CSymbolMap* map=(CSymbolMap*)&mapStore;CCharStringData strings[16],queryData;unsigned long words[16],queryWord;int cases=0;
 for(int n=0;n<=16;++n)for(int q=0;q<20;++q)for(int alias=0;alias<2;++alias)for(int variant=0;variant<3;++variant){
  memset(&mapStore,0xa7,sizeof(mapStore));map->SLONGMap._M_start=entries;map->SLONGMap._M_finish=entries+n;map->SLONGMap._M_end_of_storage._M_data=entries+16;
  for(int i=0;i<n;++i){words[i]=(i-1)/2+1;strings[i].Data.PData=(char*)&words[i];strings[i].Data.StringLength=4;*(void**)&entries[i].first=i?&strings[i]:0;entries[i].second.Val=variant==0?0:variant==1?i*13+7:-i*17-1;}
  queryWord=q-1;queryData.Data.PData=(char*)&queryWord;queryData.Data.StringLength=4;void* query=q?&queryData:0;int wanted=0;
  while(wanted<n&&(q&&(!wanted||words[wanted]<queryWord)))++wanted;
  bool found=wanted<n&&(!q?!wanted:wanted&&words[wanted]==queryWord);
  if(alias&&found)query=*(void**)&entries[wanted].first;
  char before[sizeof(mapStore)];memcpy(before,&mapStore,sizeof(mapStore));
  if(((const CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> >&)map->SLONGMap).LowerBound(*(CCharString*)&query)!=entries+wanted)return 1;
  if(map->SymbolExists(*(CCharString*)&query)!=found)return 2;
  if(map->GetSymbolSLONG(*(CCharString*)&query)!=(found?entries[wanted].second.Val:0))return 3;
  long output=0x12345678;
  if(map->FindSymbol(*(CCharString*)&query,output)!=found)return 5;
  if(output!=(found?entries[wanted].second.Val:0x12345678))return 6;
  if(memcmp(before,&mapStore,sizeof(mapStore)))return 4;++cases;
 }
 printf("FIND_SYMBOL_CHAIN_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
