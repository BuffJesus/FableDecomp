#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fable_system_manager.h"
#include "test_support/base_class_stubs.h"
static bool baseResult=true,allocationFails;static int initCalls,uninitCalls,allocCalls,freeCalls;static size_t requested;
CInitBaseClass::CInitBaseClass():Valid(false){} CInitBaseClass::~CInitBaseClass(){} void CInitBaseClass::Validate()const{}
bool CInitBaseClass::Initialise(){++initCalls;Valid=baseResult;return baseResult;}bool CInitBaseClass::IsValid()const{return Valid;}void CInitBaseClass::Uninitialise(){++uninitCalls;Valid=false;}
void* operator new[](size_t size){++allocCalls;requested=size;return allocationFails?0:malloc(size?size:1);}void operator delete[](void* p){++freeCalls;free(p);}
struct CScratchBufferTestAccess{static void set(CScratchBuffer& p,void* b,bool locked,long n,bool valid){p.Buffer=(fable_u8*)b;p.Locked=locked;p.NoBytes=n;p.Valid=valid;}static void* data(CScratchBuffer& p){return p.Buffer;}static long size(CScratchBuffer& p){return p.NoBytes;}static bool locked(CScratchBuffer& p){return p.Locked;}};
#define CHECK(x) do{if(!(x)){printf("FAIL %d\n",__LINE__);return 1;}}while(0)
int main(){for(int b=0;b<2;++b)for(int fail=0;fail<2;++fail){CScratchBuffer p;baseResult=b!=0;allocationFails=fail!=0;initCalls=allocCalls=0;CScratchBufferTestAccess::set(p,(void*)0x1234,true,77,false);bool result=p.Initialise(4096);CHECK(result==(b&&!fail)&&initCalls==1&&allocCalls==b);CHECK(result?(CScratchBufferTestAccess::data(p)!=0&&CScratchBufferTestAccess::size(p)==4096&&!CScratchBufferTestAccess::locked(p)):(CScratchBufferTestAccess::data(p)==0&&CScratchBufferTestAccess::size(p)==77&&CScratchBufferTestAccess::locked(p)));if(result){CHECK(requested==4096);delete[](fable_u8*)CScratchBufferTestAccess::data(p);}}puts("SCRATCH_PASS");return 0;}