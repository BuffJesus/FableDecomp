#include "../src/compiled/00/4a/CSymbolMap_ContainerLifetime_004ad4a0.cpp"
#include <stdio.h>
#include <string.h>
#include "fable_symbol_map.h"
#include "test_support/base_class_stubs.h"
#include "test_support/string_vector_fixture.h"
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CSymbolMap::CSymbolMap(){}

static int lowerCalls,compareCalls;
bool CCharString::FastCompare(const CCharString& other)const{++compareCalls;return strcmp(GetConstCharPtr(),other.GetConstCharPtr())<0;}
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
template<> const Entry* CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> >::LowerBound(const CCharString& key)const{++lowerCalls;const Entry* p=begin();while(p!=end()&&strcmp(p->first.GetConstCharPtr(),key.GetConstCharPtr())<0)++p;return p;}
#define CHECK(x) do{if(!(x)){printf("FAIL %d\n",__LINE__);return 1;}}while(0)
int main(){ {CSymbolMap map;CCharString group("group");CCharString empty("aa");CHECK(!map.SymbolExists(empty));CHECK(lowerCalls==1&&compareCalls==0);
 const char* names[]={"bb","dd","ff"};long vals[]={37,0,-81};map.SLONGMap._M_start=(Entry*)malloc(3*sizeof(Entry));map.SLONGMap._M_finish=map.SLONGMap._M_start;map.SLONGMap._M_end_of_storage._M_data=map.SLONGMap._M_start+3;
 for(int i=0;i<3;++i){CCharString s(names[i]);new(map.SLONGMap._M_finish++)Entry(s,CSymbolMap::CSymbolInfo(group,s,vals[i]));}
 const char* probes[]={"aa","bb","cc","dd","ee","ff","gg"};long expected[]={0,37,0,0,0,-81,0};
 for(int j=0;j<7;++j){CCharString s(probes[j]);lowerCalls=compareCalls=0;CHECK(map.SymbolExists(s)==(j==1||j==3||j==5));CHECK(lowerCalls==1&&compareCalls==(j==6?0:1));CHECK(map.SLONGMap.size()==3);}
 }CHECK(live==0&&dataLive==0);puts("SYMBOL_SEARCH_PASS");return 0;}
