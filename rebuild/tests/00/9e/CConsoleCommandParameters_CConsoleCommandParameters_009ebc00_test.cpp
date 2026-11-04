#include <stdio.h>
#include "fable_console.h"
CConsoleCommandParameters::~CConsoleCommandParameters(){}
int main(){
 {CConsoleCommandParameters p(GENERIC_VAR_STRING,GENERIC_VAR_BOOL,GENERIC_VAR_ULONG);
 if(p.ParamTypes.size()!=3||p.ParamTypes.capacity()<p.ParamTypes.size())return 1;
 if(p.ParamTypes[0]!=GENERIC_VAR_STRING||p.ParamTypes[1]!=GENERIC_VAR_BOOL||p.ParamTypes[2]!=GENERIC_VAR_ULONG)return 2;
 }
 {CConsoleCommandParameters p(GENERIC_VAR_NULL,GENERIC_VAR_NULL,GENERIC_VAR_NULL);
 if(p.ParamTypes.size()!=3)return 3;
 for(unsigned i=0;i<p.ParamTypes.size();++i)if(p.ParamTypes[i]!=GENERIC_VAR_NULL)return 4;
 }
 puts("PARAMS3_PASS");return 0;
}