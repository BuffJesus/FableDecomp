#include <stdio.h>
#include "fable_console.h"
int main(){
 _STL::vector<EGenericVarType> empty;
 _STL::vector<EGenericVarType> emptyCopy(empty);
 if(emptyCopy._M_start||emptyCopy._M_finish||emptyCopy._M_end_of_storage._M_data)return 1;
 _STL::vector<EGenericVarType> original;
 original._M_start=(EGenericVarType*)malloc(16);original._M_finish=original._M_start+3;original._M_end_of_storage._M_data=original._M_start+4;
 original[0]=GENERIC_VAR_STRING;original[1]=GENERIC_VAR_BOOL;original[2]=GENERIC_VAR_ULONG;
 _STL::vector<EGenericVarType> copy(original);
 if(copy.size()!=3||copy.capacity()!=3||copy._M_start==original._M_start)return 2;
 for(unsigned i=0;i<3;++i)if(copy[i]!=original[i])return 3;
 copy[1]=GENERIC_VAR_FLOAT;if(original[1]!=GENERIC_VAR_BOOL)return 4;
 puts("PARAMS_COPY_PASS");return 0;
}