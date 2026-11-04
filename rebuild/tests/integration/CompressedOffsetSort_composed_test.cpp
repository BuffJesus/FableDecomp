#include "fable_vector_map.h"
#include "fable_stlport_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::pair<unsigned long,unsigned long> Entry;
typedef CKeyPairCompareLess<unsigned long,unsigned long> Less;
// RETAIL_EXPECTATIONS
static unsigned long key(unsigned i,unsigned n,unsigned mode){
    const unsigned long limits[]={0,0x80000000,0xffffffff,7,0x7fffffff,0xfffffffe};
    return mode==0?i:mode==1?n-i:mode==2?(i*7+3)%5:limits[i%6];
}
int main(){
    const unsigned counts[]={0,1,2,16,17,32,64};unsigned fixture=0;
    for(unsigned c=0;c<7;++c)for(unsigned mode=0;mode<4;++mode)for(unsigned route=0;route<3;++route,++fixture){
        unsigned n=counts[c],middle=n>1?n/2:n;
        Entry buffer[66];buffer[0].first=buffer[65].first=0xdeadbeef;buffer[0].second=buffer[65].second=0x12345678;
        Entry* first=buffer+1;Entry* last=first+n;
        last->first=0xdeadbeef;last->second=0x12345678;
        for(unsigned i=0;i<n;++i){first[i].first=key(i,n,mode);first[i].second=0x80000000+i;}
        if(route==0)_STL::sort(first,last,Less());
        else if(route==1){_STL::__introsort_loop(first,last,static_cast<Entry*>(0),0,Less());_STL::__final_insertion_sort(first,last,Less());}
        else _STL::__partial_sort(first,first+middle,last,static_cast<Entry*>(0),Less());
        if(buffer[0].first!=0xdeadbeef||buffer[0].second!=0x12345678||last->first!=0xdeadbeef||last->second!=0x12345678)abort();
        unsigned seen[64]={0};
        for(unsigned i=0;i<n;++i){unsigned id=first[i].second-0x80000000;if(id>=n||seen[id]++||first[i].first!=key(id,n,mode))abort();if(first[i].first!=retail[fixture][i*2]||first[i].second!=retail[fixture][i*2+1])abort();}
    }
    puts("COMPRESSED_OFFSET_SORT_OK cases=84");return 0;
}
