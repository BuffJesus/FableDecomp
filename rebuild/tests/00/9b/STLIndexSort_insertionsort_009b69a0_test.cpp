#include "fable_vector_map.h"
#include "fable_stlport_algo.h"
typedef _STL::pair<unsigned long,long> Entry;
typedef CKeyPairCompareLess<unsigned long,long> Less;
#include <stdio.h>
static Entry* start; static int calls; static int bad;
template<> void FABLE_FASTCALL _STL::__linear_insert(Entry* first,Entry* last,Entry val,Less) {
 ++calls;
 if(first!=start || last!=start+calls || val.first!=last->first || val.second!=last->second) bad=1;
}
int main() {
 Entry a[20];for(int i=0;i<20;++i){a[i].first=0xffffffffUL-i;a[i].second=-i;}
 start=a+2;
 _STL::__insertion_sort(start,start,Less());
 _STL::__insertion_sort(start,start+1,Less());
 if(calls) return 1;
 _STL::__insertion_sort(start,start+17,Less());
 if(calls!=16 || bad) return 2;
 puts("INSERTION_PASS");return 0;
}