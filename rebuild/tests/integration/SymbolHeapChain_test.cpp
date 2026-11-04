#include "fable_symbol_map.h"
#include "fable_stlport_heap.h"
#include <stdio.h>
#include <string.h>
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
static CCharStringData refs[40];static unsigned long keys[40];static int copies,destroyed;
CCharString::CCharString(const CCharString& x){storage_=x.storage_;if(storage_)++storage_->owners;++copies;}
CCharString::~CCharString(){if(storage_)--storage_->owners;storage_=0;++destroyed;}
CCharString& CCharString::operator=(const CCharString& x){if(x.storage_)++x.storage_->owners;if(storage_)--storage_->owners;storage_=x.storage_;return *this;}
static unsigned value[40];static bool nulls[40];static int owners[40];
static bool less(unsigned a,unsigned b){if(nulls[a]!=nulls[b])return nulls[a];return !nulls[a]&&value[a]<value[b];}
static void init(Entry* a,int n,int pattern){
 for(int i=0;i<n;++i){value[i]=pattern==0?i:pattern==1?n-i:pattern==2?7:pattern==3?i%3:(i*13+n)%17;nulls[i]=pattern==5&&i%3==0;keys[i]=value[i];refs[i].Data.PData=(char*)&keys[i];refs[i].Data.StringLength=4;refs[i].owners=nulls[i]?1:4;owners[i]=refs[i].owners;
  for(int j=0;j<3;++j)*(CCharStringData**)((char*)&a[i]+4*j)=nulls[i]?0:&refs[i];a[i].second.Val=i;
 }copies=destroyed=0;
}
static int validate(Entry* a,int n){bool seen[40]={false};if(copies!=destroyed)return 1;for(int i=0;i<n;++i){int id=a[i].second.Val;if(id<0||id>=n||seen[id])return 2;seen[id]=true;for(int j=0;j<3;++j)if(*(CCharStringData**)((char*)&a[i]+4*j)!=(nulls[id]?0:&refs[id]))return 3;}for(int i=0;i<n;++i)if(refs[i].owners!=owners[i])return 4;for(int i=0;i<n;++i)a[i].~Entry();for(int i=0;i<n;++i)if(refs[i].owners!=1)return 5;return 0;}
int main(){static union{double align;char data[40*16];} storage;Entry* a=(Entry*)&storage;int cases=0;CCompareString<CSymbolMap::CSymbolInfo> comp;
 for(int n=0;n<=32;++n)for(int pattern=0;pattern<6;++pattern)for(int middle=n?1:0;middle<=n;++middle){
  init(a,n,pattern);int expected[40];for(int i=0;i<n;++i)expected[i]=i;
  for(int i=1;i<n;++i){int v=expected[i],j=i;while(j&&less(v,expected[j-1])){expected[j]=expected[j-1];--j;}expected[j]=v;}
  _STL::__partial_sort(a,a+middle,a+n,(Entry*)0,comp);
  for(int i=0;i<middle;++i){if(less(a[i].second.Val,expected[i])||less(expected[i],a[i].second.Val))return 1;}
  if(middle)for(int i=middle;i<n;++i)if(less(a[i].second.Val,a[middle-1].second.Val))return 2;
  if(validate(a,n))return 3;++cases;
 }
 printf("SYMBOL_HEAP_CHAIN_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
