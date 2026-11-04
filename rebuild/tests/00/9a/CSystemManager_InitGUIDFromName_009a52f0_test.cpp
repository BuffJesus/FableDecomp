#include <stdio.h>
#include <string.h>
#include "fable_gfuninitialise.h"
#include "test_support/base_class_stubs.h"
CInitBaseClass::CInitBaseClass():Valid(false){}
CInitBaseClass::~CInitBaseClass(){}
void CInitBaseClass::Validate()const{}
CWideString::CWideString(){*(void**)this=0;}CWideString::~CWideString(){}
CCharString::CCharString(){*(void**)this=0;}CCharString::~CCharString(){}
struct CSystemManagerTestAccess{static void guid(CSystemManager& m,const char* n){m.InitGUIDFromName(n);}};
int main(){const char* inputs[]={"","a","BBB GUID","123456789012345","1234567890123456","12345678901234567890","ab\0ignored"};for(int k=0;k<7;++k){unsigned long storage[sizeof(CSystemManager)/4];memset(storage,0xa5,sizeof(storage));CSystemManager& m=*reinterpret_cast<CSystemManager*>(storage);unsigned char before[sizeof(m)];memset(m.ApplicationGUID,0xcc,16);memcpy(before,&m,sizeof(m));CSystemManagerTestAccess::guid(m,inputs[k]);size_t n=strlen(inputs[k]);for(int i=0;i<16;++i)if(((char*)m.ApplicationGUID)[i]!=(i<(int)n?inputs[k][i]:0))return 1;memcpy(before+12,m.ApplicationGUID,16);if(memcmp(before,&m,sizeof(m)))return 2;}puts("SYSTEM_GUID_PASS");return 0;}