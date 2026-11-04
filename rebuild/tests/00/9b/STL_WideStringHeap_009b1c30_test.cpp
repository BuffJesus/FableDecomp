#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::less<CWideString> Less;
static unsigned long key(const CWideString& s){return *reinterpret_cast<const unsigned long*>(&s);}
CWideString::CWideString(const CWideString& s):storage_(s.storage_){}
CWideString::~CWideString(){}
CWideString& CWideString::operator=(const CWideString& s){storage_=s.storage_;return *this;}
bool CWideString::operator<(const CWideString& s)const{return key(*this)<key(s);}
static void run(unsigned count,unsigned mode){
    unsigned long values[40]={0},sorted[40]={0};
    for(unsigned i=0;i<count;++i)values[i]=mode==0?i:mode==1?count-i:mode==2?(i*7+3)%5:(i*19+11)%29;
    memcpy(sorted,values,count*4);
    for(unsigned i=0;i<count;++i)for(unsigned j=i+1;j<count;++j)if(sorted[j]<sorted[i]){unsigned long t=sorted[i];sorted[i]=sorted[j];sorted[j]=t;}
    CWideString* first=reinterpret_cast<CWideString*>(values);
    _STL::__make_heap(first,first+count,Less(),static_cast<CWideString*>(0),static_cast<int*>(0));
    for(unsigned i=1;i<count;++i)if(values[(i-1)/2]<values[i])abort();
    _STL::sort_heap(first,first+count,Less());
    if(memcmp(values,sorted,count*4))abort();
}
int main(){
    for(unsigned count=0;count<=15;++count)for(unsigned mode=0;mode<4;++mode)run(count,mode);
    run(32,2);run(32,3);
    puts("WIDESTRING_HEAP_OK cases=66");return 0;
}
