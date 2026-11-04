#include "test_support/string_vector_fixture.h"
static int overflowCalls;static const void* observedVector;static CCharString* observedPos;static const CCharString* observedValue;static size_t observedCount;static bool observedEnd;
template<> void _STL::vector<CCharString>::_M_insert_overflow(CCharString* pos,const CCharString& x,const _STL::__false_type&,size_t n,bool atEnd){
 ++overflowCalls;observedVector=this;observedPos=pos;observedValue=&x;observedCount=n;observedEnd=atEnd;
}
template<> CCharString* FABLE_FASTCALL _STL::uninitialized_fill_n<CCharString>(CCharString* first,size_t n,const CCharString& value){
 for(size_t i=0;i<n;++i)new(first+i)CCharString(value);return first+n;
}
static int run(int pos,int count,bool tight){
 unsigned long space[16];CCharString* a=(CCharString*)space;
 const char* words[]={"alpha","beta","gamma","delta"};
 for(int i=0;i<4;++i)new(a+i)CCharString(words[i]);
 _STL::vector<CCharString> v;v._M_start=a;v._M_finish=a+4;v._M_end_of_storage._M_data=a+(tight?4:16);
 overflowCalls=0;const CCharString* alias=&a[2];
 v._M_fill_insert(a+pos,count,*alias);
 if(tight && count){if(overflowCalls!=1||observedVector!=&v||observedPos!=a+pos||observedValue!=alias||observedCount!=count||observedEnd)return 1;}
 else{
  if(overflowCalls||v.size()!=4+count||live!=4+count)return 2;
  for(i=0;i<4+count;++i){const char* expected=i<pos?words[i]:i<pos+count?"gamma":words[i-count];if(strcmp(a[i].GetConstCharPtr(),expected))return 3;}
 }
 for(CCharString* p=v._M_start;p!=v._M_finish;++p)p->~CCharString();v._M_start=v._M_finish=v._M_end_of_storage._M_data=0;
 return live||dataLive?4:0;
}
int main(){
 if(run(1,1,false)||run(3,5,false)||run(4,2,false)||run(0,0,false)||run(1,2,true)||run(0,0,true))return 1;
 puts("STRING_FILL_PASS");return 0;
}