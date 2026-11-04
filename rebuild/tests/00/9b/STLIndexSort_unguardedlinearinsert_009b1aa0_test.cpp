#include "fable_vector_map.h"
#include "fable_stlport_algo.h"
typedef _STL::pair<unsigned long,long> Entry;
typedef CKeyPairCompareLess<unsigned long,long> Less;
#include <stdio.h>
namespace _STL { template<class I,class T,class C> void FABLE_FASTCALL __unguarded_linear_insert(I,T,C); }
int main() {
 unsigned long values[]={0,1,2,3,6,7,8,0x7fffffffUL,0x80000000UL,0xffffffffUL};
 for(int n=1;n<=24;++n) for(int k=0;k<10;++k) {
  Entry a[27]; a[0].first=0xabcdef;a[0].second=-2;
  for(int i=0;i<n;++i){a[i+1].first=i*2;a[i+1].second=i;}
  a[n+2].first=0x123456;a[n+2].second=-3;
  Entry val;val.first=values[k];val.second=100;
  _STL::__unguarded_linear_insert(a+n+1,val,Less());
  int pos=0;while(pos<n && (unsigned long)(pos*2)<=val.first)++pos;
  for(int j=0;j<=n;++j){
   int id=j<pos?j:j==pos?100:j-1;
   if(a[j+1].second!=id || a[j+1].first!=(id==100?val.first:(unsigned long)id*2))return 1;
  }
  if(a[0].first!=0xabcdef || a[0].second!=-2 || a[n+2].first!=0x123456 || a[n+2].second!=-3)return 2;
 }
 puts("LINEAR_PASS");return 0;
}
