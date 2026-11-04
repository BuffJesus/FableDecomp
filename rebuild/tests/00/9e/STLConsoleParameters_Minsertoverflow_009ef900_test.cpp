#include <stdio.h>
#include "fable_console.h"
static bool check(const _STL::vector<EGenericVarType>& v,const int* expected,unsigned count,unsigned capacity){
 if(v.size()!=count||v.capacity()!=capacity)return false;
 for(unsigned i=0;i<count;++i)if(v[i]!=expected[i])return false;return true;
}
int main(){
 _STL::vector<EGenericVarType> v;EGenericVarType value=GENERIC_VAR_STRING;
 v._M_insert_overflow(v.begin(),value,_STL::__false_type(),1,true);int a[]={5};if(!check(v,a,1,1))return 1;
 value=GENERIC_VAR_BOOL;v._M_insert_overflow(v.end(),value,_STL::__false_type(),1,true);int b[]={5,4};if(!check(v,b,2,2))return 2;
 v._M_insert_overflow(v.begin()+1,v[0],_STL::__false_type(),3,false);int c[]={5,5,5,5,4};if(!check(v,c,5,5))return 3;
 value=GENERIC_VAR_NULL;v._M_insert_overflow(v.begin()+2,value,_STL::__false_type(),0,false);if(!check(v,c,5,10))return 4;
 value=GENERIC_VAR_ULONG;v._M_insert_overflow(v.end(),value,_STL::__false_type(),2,true);int d[]={5,5,5,5,4,2,2};if(!check(v,d,7,10))return 5;
 puts("PARAMS_OVERFLOW_PASS");return 0;
}