#include "fable_vector_map.h"
#include "fable_stlport_algo.h"
#include <stdio.h>
typedef _STL::pair<unsigned long,long> Entry;
typedef CKeyPairCompareLess<unsigned long,long> Less;
static int Run(const unsigned long* keys, int n, unsigned long pivot)
{
    Entry a[66]; unsigned char seen[64] = {0};
    a[0].first = 123; a[0].second = -7;
    a[n+1].first = 456; a[n+1].second = -9;
    for (int i=0;i<n;++i) { a[i+1].first=keys[i]; a[i+1].second=i; }
    Entry p; p.first=pivot; p.second=-100;
    Entry* cut=_STL::__unguarded_partition(a+1,a+n+1,p,Less());
    if (cut<a+1 || cut>a+n+1) return 1;
    for (int j=0;j<n;++j) {
        Entry& e=a[j+1];
        if (e.second<0 || e.second>=n || seen[e.second]++) return 2;
        if (e.first!=keys[e.second]) return 3;
        if (&e<cut ? e.first>pivot : e.first<pivot) return 4;
    }
    if (a[0].first!=123 || a[0].second!=-7 || a[n+1].first!=456 || a[n+1].second!=-9) return 5;
    return 0;
}
int main()
{
    unsigned long a[64];
    for(int n=1;n<=64;++n) {
        for(int mode=0;mode<5;++mode) {
            for(int i=0;i<n;++i)
                a[i]=mode==0 ? i : mode==1 ? n-i : mode==2 ? 7 : mode==3 ? (i*17)%9 : 0x80000000UL+(i*31)%97;
            if(Run(a,n,a[n/2])) return 1;
        }
    }
    unsigned long edge[]={0xffffffffUL,0,0x80000000UL,0x7fffffffUL,0xffffffffUL,0};
    for(int i=0;i<6;++i) if(Run(edge,6,edge[i])) return 2;
    puts("PARTITION_PASS"); return 0;
}