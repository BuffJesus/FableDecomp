#include "fable_vector_map.h"
#include "fable_stlport_algo.h"
typedef _STL::pair<unsigned long,long> Entry;
typedef CKeyPairCompareLess<unsigned long,long> Less;
#include <stdio.h>
int main() {
 Entry a[35]; unsigned long keys[32];
 for(int n=0;n<=32;++n) for(int mode=0;mode<4;++mode) {
  a[0].first=0x1234; a[0].second=-2;
  a[1].first=0; a[1].second=-1;
  for(int i=0;i<n;++i) { keys[i]= mode==0?n-i:mode==1?i:mode==2?7:0x80000000UL+(i*13)%17; a[i+2].first=keys[i];a[i+2].second=i; }
  a[n+2].first=0x5678;a[n+2].second=-3;
  _STL::__unguarded_insertion_sort_aux(a+2,a+n+2,(Entry*)0,Less());
  unsigned char seen[32]={0};
  for(int j=0;j<n;++j) {
   Entry& e=a[j+2]; if(e.second<0 || e.second>=n || seen[e.second]++ || e.first!=keys[e.second])return 1;
   if(j && a[j+1].first>e.first)return 2;
   if(j && a[j+1].first==e.first && a[j+1].second>e.second)return 3;
  }
  if(a[0].first!=0x1234 || a[0].second!=-2 || a[1].first!=0 || a[1].second!=-1 || a[n+2].first!=0x5678 || a[n+2].second!=-3)return 4;
 }
 puts("UNGUARDED_PASS");return 0;
}