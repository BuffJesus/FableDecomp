#include <stdio.h>
#include "fable_console.h"
CConsoleCommandParameters::~CConsoleCommandParameters(){}
int main(){
 {CConsoleCommandParameters p(GENERIC_VAR_STRING,GENERIC_VAR_BOOL);
 if(p.ParamTypes.size()!=2||p.ParamTypes.capacity()<p.ParamTypes.size())return 1;
 if(p.ParamTypes[0]!=GENERIC_VAR_STRING||p.ParamTypes[1]!=GENERIC_VAR_BOOL)return 2;
 }
 {CConsoleCommandParameters p(GENERIC_VAR_NULL,GENERIC_VAR_NULL);
 if(p.ParamTypes.size()!=2)return 3;
 for(unsigned i=0;i<p.ParamTypes.size();++i)if(p.ParamTypes[i]!=GENERIC_VAR_NULL)return 4;
 }
 puts("PARAMS2_PASS");return 0;
}