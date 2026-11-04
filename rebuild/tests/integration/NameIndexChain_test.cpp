#include "fable_definition_manager.h"
#include "fable_crc.h"
#include <stdio.h>
#include <string.h>
fable_u32 g_FableCRCTable_0129A168[256];
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static long Value(int i,int v){return v==0?0:v==1?i*13+7:-i*17-1;}
class TestManager:public CDefinitionManager{public:using CDefinitionManager::PDefinitionSymbolMap;using CDefinitionManager::NameCRCToIndexMap;};
int main(){
 for(unsigned long i=0;i<256;++i){unsigned long x=i;for(int b=0;b<8;++b)x=x&1?(x>>1)^0xedb88320:x>>1;g_FableCRCTable_0129A168[i]=x;}
 typedef _STL::pair<unsigned long,long> Row;
 typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
 union{double align;char bytes[sizeof(CDefinitionManager)];} ds;
 union{double align;char bytes[sizeof(CSymbolMap)];} ss;
 union{double align;char bytes[16*sizeof(Row)];} rows;
 union{double align;char bytes[16*sizeof(Entry)];} entries;
 TestManager* d=(TestManager*)&ds;CSymbolMap* s=(CSymbolMap*)&ss;Row* r=(Row*)&rows;Entry* e=(Entry*)&entries;
 CCharStringData strings[16],query;unsigned long words[16],qw;int cases=0;
 for(int n=0;n<=16;++n)for(int q=0;q<20;++q)for(int v=0;v<3;++v)for(int route=0;route<3;++route){
  memset(&ds,0xa7,sizeof(ds));memset(&ss,0xa7,sizeof(ss));
  *(CSymbolMap**)&d->PDefinitionSymbolMap=s;
  d->NameCRCToIndexMap._M_start=r;d->NameCRCToIndexMap._M_finish=r+(route==1?0:n);d->NameCRCToIndexMap._M_end_of_storage._M_data=r+16;
  s->SLONGMap._M_start=e;s->SLONGMap._M_finish=e+n;s->SLONGMap._M_end_of_storage._M_data=e+16;
  for(int i=0;i<n;++i){words[i]=i;strings[i].Data.PData=(char*)&words[i];strings[i].Data.StringLength=4;*(void**)&e[i].first=&strings[i];e[i].second.Val=Value(i,v);unsigned long key=i+(route==2?20:0);r[i].first=FableCRC_Calc_004014A0(0,&key,4);r[i].second=Value(i,v);}
  for(int i=1;i<n;++i)for(int j=i;j>0&&r[j].first<r[j-1].first;--j){Row t=r[j];r[j]=r[j-1];r[j-1]=t;}
  qw=q-2;query.Data.PData=(char*)&qw;query.Data.StringLength=q==1?0:4;void* qp=q?&query:0;
  char before[sizeof(ds)];memcpy(before,&ds,sizeof(ds));
  long expected=q>=2&&q-2<n&&route!=2?Value(q-2,v):0;
  if(d->GetDefGlobalIndexFromName(*(CCharString*)&qp)!=expected)return 1;
  if(memcmp(before,&ds,sizeof(ds)))return 2;
  ++cases;
 }
 printf("NAME_INDEX_CHAIN_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;
}
