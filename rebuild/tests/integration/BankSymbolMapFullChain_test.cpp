#include "fable_game_text_bank.h"
#include "fable_symbol_map.h"
#include "test_support/base_class_stubs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int stringDataCount,enumerations;
char g_FableEmptyNarrowString_0129AAF4[]="";
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
static void release(CCharStringData* p){if(p&&!--p->owners){free(p->Data.PData);free(p);--stringDataCount;}}
CCharString::CCharString(){storage_=0;}
CCharString::CCharString(const char* text,long len){if(len<0)len=strlen(text);storage_=(CCharStringData*)malloc(sizeof(CCharStringData));memset(storage_,0,sizeof(CCharStringData));storage_->Data.PData=(char*)calloc((len+4)&~3,1);memcpy(storage_->Data.PData,text,len);storage_->Data.StringLength=len;storage_->owners=1;++stringDataCount;}
CCharString::CCharString(const CCharString& x){storage_=x.storage_;if(storage_)++storage_->owners;}
CCharString::~CCharString(){release(storage_);storage_=0;}
CCharString& CCharString::operator=(const CCharString& x){if(x.storage_)++x.storage_->owners;release(storage_);storage_=x.storage_;return *this;}
struct EnumerationBoundary{virtual CCharString Get(unsigned long type)const{++enumerations;return CCharString(type&1?"odd":"even");}};
static void trap(){abort();}
static bool valid(int i,int packed){return i>0&&(packed&1?i%4!=0:i%3!=0);}
static unsigned type(int i,int packed){return packed&2?0xffffff00+i:i*13;}
int main(){int cases=0;int baseline=stringDataCount;EnumerationBoundary boundary;void* table[20];for(int i=0;i<20;++i)table[i]=(void*)&trap;table[19]=(*(void***)&boundary)[0];
 for(int n=0;n<=33;++n)for(int pattern=0;pattern<4;++pattern)for(int packed=0;packed<4;++packed)for(int append=0;append<2;++append)for(int duplicates=0;duplicates<2;++duplicates)for(int repeat=0;repeat<2;++repeat){
  {CCharString keys[34];char names[34][16];CBankFile::CRuntimeData runtime[34];unsigned long validWords[34],typeWords[34];
   for(int i=0;i<34;++i){sprintf(names[i],pattern==2&&i%5==0?"[n%02d]":"n%02d",pattern==1?i%5:i);keys[i]=CCharString(names[i]);runtime[i].Valid=i%3!=0;runtime[i].DataType=(unsigned char)(i*13);if(i){validWords[i-1]=i%4!=0;typeWords[i-1]=0xffffff00+i;}}
   union{double align;char data[0x110];} bankMem;memset(&bankMem,0,sizeof(bankMem));CBankFile* bank=(CBankFile*)&bankMem;*(void***)bank=table;bank->Size=n;bank->Symbols._M_start=keys;bank->Symbols._M_finish=keys+(pattern==3?0:n);bank->Symbols._M_end_of_storage._M_data=keys+34;bank->RuntimeData._M_start=runtime;bank->RuntimeData._M_finish=runtime+n;bank->RuntimeData._M_end_of_storage._M_data=runtime+34;bank->PackedValid.Size=packed&1;bank->PackedValid.Bits=32;bank->PackedValid.PackedInts=validWords;bank->PackedDataType.Size=packed&2;bank->PackedDataType.Bits=32;bank->PackedDataType.PackedInts=typeWords;
   CCharString seedGroup("seed-group"),seedName("seed");int beforeMap=stringDataCount;CSymbolMap* map=new CSymbolMap;map->CheckForDuplicateSymbols=duplicates!=0;map->AddSymbolDirty(seedGroup,seedName,-7);map->UpdateDirty();enumerations=0;
   bank->CreateSymbolMap(*map,append!=0);bank->CreateSymbolMap(*map,repeat!=0);
   int wanted=append&&repeat?1:0,bankEntries=0;for(int i=1;i<n;++i)if(valid(i,packed)&&(pattern==3||!strchr(names[i],'[')))++bankEntries;wanted+=bankEntries*(repeat+1);
   if(map->SLONGMap.size()!=(unsigned)wanted||map->SLONGMap.capacity()!=(unsigned)wanted||map->SLONGMap.Dirty||enumerations!=bankEntries*2)return 1;
   unsigned occurrences[34]={0};int seedCount=0,unique=0;
   for(unsigned i=0;i<map->SLONGMap.size();++i){const _STL::pair<CCharString,CSymbolMap::CSymbolInfo>& row=map->SLONGMap[i];long index=row.second.Val;const char* name=row.first.GetConstCharPtr();
    if(i&&row.first.FastCompare(map->SLONGMap[i-1].first))return 2;
    if(strcmp(name,row.second.EntryName.GetConstCharPtr()))return 3;
    if(index==-7){if(strcmp(name,"seed")||strcmp(row.second.GroupName.GetConstCharPtr(),"seed-group"))return 4;++seedCount;}
    else {if(index<=0||index>=n||!valid(index,packed)||strcmp(name,pattern==3?"(Unknown)":names[index])||strchr(name,'[')||strcmp(row.second.GroupName.GetConstCharPtr(),type(index,packed)&1?"odd":"even"))return 5;++occurrences[index];}
    bool seen=false;for(unsigned j=0;j<i;++j)if(!strcmp(name,map->SLONGMap[j].first.GetConstCharPtr()))seen=true;if(!seen)++unique;
    if(!map->SymbolExists(row.first))return 6;unsigned first=0;while(strcmp(map->SLONGMap[first].first.GetConstCharPtr(),name))++first;if(map->GetSymbolSLONG(row.first)!=map->SLONGMap[first].second.Val)return 7;
   }
   if(seedCount!=(append&&repeat?1:0)||map->NamesUsed._M_t._M_node_count!=(unsigned)(duplicates?unique:0))return 8;
   for(int i=1;i<n;++i)if(occurrences[i]!=(unsigned)(valid(i,packed)&&(pattern==3||!strchr(names[i],'['))?repeat+1:0))return 9;
   {CCharString missing("missing!");if(map->SymbolExists(missing)||map->GetSymbolSLONG(missing))return 10;}
   map->Clear();if(map->SLONGMap.size()||map->SLONGMap.capacity()||map->SLONGMap.Dirty||map->NamesUsed._M_t._M_node_count)return 11;delete map;if(stringDataCount!=beforeMap)return 12;
  }
  if(stringDataCount!=baseline)return 13;++cases;
 }
 printf("BANK_SYMBOL_MAP_FULL_CHAIN_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
