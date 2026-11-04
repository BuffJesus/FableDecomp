#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>
namespace _STL { template<class I,class T,class C> void FABLE_FASTCALL __unguarded_linear_insert(I,T,C); }
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
static CCharStringData refs[64];static unsigned long keys[64];static int copies,destroyed,assigned;
CCharString::CCharString(const CCharString& x){storage_=x.storage_;if(storage_)++storage_->owners;++copies;}
CCharString::~CCharString(){if(storage_)--storage_->owners;storage_=0;++destroyed;}
CCharString& CCharString::operator=(const CCharString& x){if(x.storage_)++x.storage_->owners;if(storage_)--storage_->owners;storage_=x.storage_;++assigned;return *this;}
#ifdef FABLE_SINGLE_METHOD
#define METHOD_BEGIN FABLE_SINGLE_METHOD
#define METHOD_END (FABLE_SINGLE_METHOD+1)
#else
#define METHOD_BEGIN 0
#define METHOD_END 3
#endif
int main(){int cases=0;static union{double align;char bytes[33*16];} storage;
 for(int count=1;count<=32;++count)for(int pattern=0;pattern<6;++pattern)for(int method=METHOD_BEGIN;method<METHOD_END;++method){
  Entry* a=(Entry*)&storage;int n=count+1;unsigned values[33];int expected[33];
  for(int i=0;i<n;++i){unsigned value;
   if(method<2){value=i<count?i*2+(method==0?2:0):pattern==0?0:pattern==1?1:pattern==2?count:pattern==3?count*2+4:pattern==4?count*2:(count*7)%(count*2);}
   else value=i==0?0:pattern==0?n-i:pattern==1?0:pattern==2?i%2:pattern==3?(i*13+count)%17:pattern==4?i:n;
   values[i]=value;expected[i]=i;keys[i]=value;refs[i].Data.PData=(char*)&keys[i];refs[i].Data.StringLength=4;refs[i].owners=4;
   for(int j=0;j<3;++j)*(CCharStringData**)((char*)&a[i]+4*j)=&refs[i];a[i].second.Val=i;
  }
  for(int i=1;i<n;++i){int x=expected[i],j=i;while(j&&values[x]<values[expected[j-1]]){expected[j]=expected[j-1];--j;}expected[j]=x;}
  copies=destroyed=assigned=0;CCompareString<CSymbolMap::CSymbolInfo> comp;
#if !defined(FABLE_SINGLE_METHOD) || FABLE_SINGLE_METHOD==0
  if(method==0)_STL::__linear_insert(a,a+count,Entry(a[count]),comp);
#endif
#if !defined(FABLE_SINGLE_METHOD) || FABLE_SINGLE_METHOD==1
  if(method==1)_STL::__unguarded_linear_insert(a+count,Entry(a[count]),comp);
#endif
#if !defined(FABLE_SINGLE_METHOD) || FABLE_SINGLE_METHOD==2
  if(method==2)_STL::__unguarded_insertion_sort_aux(a+1,a+n,(Entry*)0,comp);
#endif
  if(copies!=destroyed)return 1;
  for(int i=0;i<n;++i){if(a[i].second.Val!=expected[i])return 2;for(int j=0;j<3;++j)if(*(CCharStringData**)((char*)&a[i]+4*j)!=&refs[expected[i]])return 3;}
  for(int i=0;i<n;++i)if(refs[i].owners!=4)return 4;
  for(int i=0;i<n;++i)a[i].~Entry();for(int i=0;i<n;++i)if(refs[i].owners!=1)return 5;++cases;
 }
 printf("SYMBOL_INSERTION_CHAIN_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
