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
int main(){for(int valid=0;valid<2;++valid)for(int have=0;have<2;++have){CScratchBuffer p;void* allocation=have?malloc(32):0;CScratchBufferTestAccess::set(p,allocation,true,32,valid!=0);freeCalls=uninitCalls=0;p.Uninitialise();CHECK(uninitCalls==1&&!p.IsValid()&&freeCalls==(valid&&have));CHECK(CScratchBufferTestAccess::data(p)==(valid?0:allocation));CHECK(CScratchBufferTestAccess::locked(p)&&CScratchBufferTestAccess::size(p)==32);if(!valid)free(allocation);}puts("SCRATCH_PASS");return 0;}