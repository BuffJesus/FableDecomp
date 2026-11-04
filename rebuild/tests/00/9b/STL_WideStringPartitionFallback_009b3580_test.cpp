#include <stdio.h>
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
typedef _STL::less<CWideString> Less;
static unsigned long& Key(CWideString& s) { return *reinterpret_cast<unsigned long*>(&s); }
static unsigned long Key(const CWideString& s) { return *reinterpret_cast<const unsigned long*>(&s); }
CWideString::CWideString(const CWideString& s) : storage_(s.storage_) {}
CWideString::~CWideString() {}
CWideString& CWideString::operator=(const CWideString& s) { storage_ = s.storage_; return *this; }
bool CWideString::operator<(const CWideString& s) const { return Key(*this) < Key(s); }
static int made, adjusted, sorted;
namespace _STL {
template<> void FABLE_FASTCALL __make_heap<CWideString*,Less,CWideString,int>(CWideString* a,CWideString* b,Less,CWideString*,int*) {
    ++made;
    for (CWideString* p=a;p!=b;++p) if(Key(*p)>Key(*a)) { unsigned long t=Key(*a);Key(*a)=Key(*p);Key(*p)=t; }
}
template<> void FABLE_FASTCALL __adjust_heap<CWideString*,int,CWideString,Less>(CWideString* a,int,int n,CWideString value,Less) {
    ++adjusted; Key(*a)=Key(value);
    for(int i=1;i<n;++i) if(Key(a[i])>Key(*a)) { unsigned long t=Key(*a);Key(*a)=Key(a[i]);Key(a[i])=t; }
}
template<> void FABLE_FASTCALL sort_heap<CWideString*,Less>(CWideString* a,CWideString* b,Less) {
    ++sorted;
    for(CWideString* p=a;p!=b;++p) for(CWideString* q=p+1;q!=b;++q)
        if(Key(*q)<Key(*p)) { unsigned long t=Key(*p);Key(*p)=Key(*q);Key(*q)=t; }
}
}
static bool Partition() {
    unsigned long raw[]={8,1,6,3,5,2,7,4};
    unsigned long pivotRaw=5;
    CWideString* a=reinterpret_cast<CWideString*>(raw);
    CWideString* cut=_STL::__unguarded_partition(a,a+8,*reinterpret_cast<CWideString*>(&pivotRaw),Less());
    if(cut<=a || cut>=a+8) return false;
    unsigned long sum=0;
    for(CWideString* p=a;p!=a+8;++p) { sum+=Key(*p); if(p<cut ? Key(*p)>5 : Key(*p)<5) return false; }
    return sum==36;
}
static bool Partial() {
    unsigned long raw[]={8,7,6,5,4,3,2,1};
    CWideString* a=reinterpret_cast<CWideString*>(raw);
    _STL::__partial_sort(a,a+3,a+8,static_cast<CWideString*>(0),Less());
    return raw[0]==1 && raw[1]==2 && raw[2]==3 && made==1 && adjusted>0 && sorted==1;
}
int main() {
    if(!Partition()) return 1;
    if(!Partial()) return 2;
    puts("WIDE_PARTITION_FALLBACK_OK"); return 0;
}
