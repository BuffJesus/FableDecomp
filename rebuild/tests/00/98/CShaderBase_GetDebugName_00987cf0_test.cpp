#include "fable_vertex_shader.h"
#include <stdio.h>
#include <string.h>
static int calls,failures,mode;static CCharString* result;
static void check(bool b){if(!b)++failures;}
CCharString::CCharString(const char* text,long length){++calls;result=this;check(strcmp(text,"(Unknown)")==0 && length==-1);storage_=reinterpret_cast<CCharStringData*>(0x123400+mode*4);}
CCharString::CCharString(const CCharString& other):storage_(other.storage_){}
CCharString::~CCharString(){}
class Probe : public CShaderBase {
public:CCharString Name() const {return GetDebugName();}
private:virtual unsigned long CountReferences(){return 0;}
};
int main(){unsigned scenarios=0;Probe probe;
 for(mode=0;mode<256;++mode){calls=0;CCharString text=probe.Name();check(calls==1 && *reinterpret_cast<unsigned long*>(&text)==0x123400+mode*4);++scenarios;}
 printf("SHADER_DEBUG_NAME_BEHAVIOR_%s scenarios=%u failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
