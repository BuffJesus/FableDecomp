#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
typedef _STL::less<CWideString> Less;
struct Storage {wchar_t text[32];unsigned refs,id;};
static unsigned live,nextId;
static Storage* storage(const CWideString& s){return reinterpret_cast<Storage*>(*reinterpret_cast<CWideStringData* const*>(&s));}
static const wchar_t* text(const CWideString& s){return storage(s)?storage(s)->text:L"";}
static void release(Storage* p){if(p&&!--p->refs){--live;free(p);}}
CWideString::CWideString():storage_(0){}
CWideString::CWideString(const CWideString& s):storage_(s.storage_){if(storage(*this))++storage(*this)->refs;}
CWideString::~CWideString(){release(storage(*this));}
CWideString& CWideString::operator=(const CWideString& s){Storage* p=storage(s);if(p)++p->refs;release(storage(*this));storage_=s.storage_;return *this;}
const CWideString& CWideString::operator=(const wchar_t* s){Storage* p=(Storage*)calloc(1,sizeof(Storage));wcscpy(p->text,s);p->refs=1;p->id=nextId++;++live;release(storage(*this));storage_=reinterpret_cast<CWideStringData*>(p);return *this;}
bool CWideString::operator<(const CWideString& s)const{return wcscmp(text(*this),text(s))<0;}
static int __cdecl compare(const void* a,const void* b){return wcscmp((const wchar_t*)a,(const wchar_t*)b);}
static void run(unsigned count,unsigned mode,unsigned route){
    if(live)abort();nextId=0;
    CWideString values[64];wchar_t expected[64][32];
    for(unsigned i=0;i<count;++i){unsigned key=mode==0?i:mode==1?count-i:mode==2?(i*7+3)%5:(i*19+11)%29;if(mode==2&&i%4==0)expected[i][0]=0;else swprintf(expected[i],L"name_%02u",key);values[i]=expected[i];}
    qsort(expected,count,sizeof(expected[0]),compare);
    if(route==0)_STL::sort(values,values+count,Less());
    else if(route==1){_STL::__introsort_loop(values,values+count,static_cast<CWideString*>(0),0,Less());_STL::__final_insertion_sort(values,values+count,Less());}
    else _STL::__partial_sort(values,values+3,values+count,static_cast<CWideString*>(0),Less());
    unsigned sorted=route==2?3:count;unsigned seen[64]={0};
    for(unsigned i=0;i<count;++i){Storage* p=storage(values[i]);if(!p||p->refs!=1||p->id>=count||seen[p->id]++)abort();if(i<sorted&&wcscmp(text(values[i]),expected[i]))abort();}
    if(live!=count)abort();
}
int main(){unsigned sizes[]={0,1,16,17,32,64};for(unsigned n=0;n<6;++n)for(unsigned mode=0;mode<4;++mode)run(sizes[n],mode,0);for(unsigned mode=0;mode<4;++mode){run(32,mode,1);run(32,mode,2);}if(live)abort();puts("WIDESTRING_SORT_COMPOSED_OK cases=32");return 0;}
