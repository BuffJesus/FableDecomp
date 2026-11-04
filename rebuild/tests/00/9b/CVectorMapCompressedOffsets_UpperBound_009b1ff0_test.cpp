#include "fable_vector_map.h"
#include <stdio.h>
typedef _STL::pair<unsigned long,unsigned long> Entry;
typedef CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> > Map;
int main(){
 double raw[sizeof(Map)/8+1];Map* map=reinterpret_cast<Map*>(raw);
 unsigned long keys[]={0,3,3,7,0x7fffffffUL,0x80000000UL,0xffffffffUL};Entry a[7];
 for(int i=0;i<7;++i){a[i].first=keys[i];a[i].second=100+i;}
 unsigned long queries[]={0,1,2,3,4,6,7,8,0x7ffffffeUL,0x7fffffffUL,0x80000000UL,0xfffffffeUL,0xffffffffUL};
 for(int n=0;n<=7;++n){map->_M_start=a;map->_M_finish=a+n;
  for(int j=0;j<13;++j){int expected=0;while(expected<n && keys[expected]<=queries[j])++expected;
   if(static_cast<const Map*>(map)->UpperBound(queries[j])!=a+expected)return 1;
  }
 }
 puts("UPPER_PASS");return 0;
}