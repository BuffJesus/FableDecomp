// Retail x86 ABI; this VC7.1 toolchain has no Windows SDK headers.
typedef void* HANDLE;
struct CRITICAL_SECTION {void* DebugInfo;long LockCount,RecursionCount;HANDLE OwningThread,LockSemaphore;unsigned long SpinCount;};
extern "C" {
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) int __stdcall TryEnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
__declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
__declspec(dllimport) __declspec(noreturn) void __stdcall ExitProcess(unsigned);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cold_aliases.h"
extern "C" void* __fastcall FixtureBankCtor(void*,void*);
extern "C" void* __fastcall FixtureAsyncCtor(void*,void*);
#pragma comment(linker,"/alternatename:@FixtureBankCtor@8=??0CBankFile@@QAE@XZ")
#pragma comment(linker,"/alternatename:@FixtureAsyncCtor@8=??0CBankFileAsync@@QAE@XZ")
int g_CWideStringInstanceCount_013BCA20;
int g_CCharStringInstanceCount_013BD800;
static unsigned char* g_object;
static unsigned char g_fill;
static unsigned g_allocations;
static void* g_blocks[7];
static unsigned g_sizes[7];
static bool g_async;
static void Fail(const char* s,int line){fprintf(stderr,"BANK_INITIALIZATION_HOST_FAILURE line=%d expr=%s\n",line,s);fflush(stderr);ExitProcess(80);}
#define CHECK(x) do{if(!(x))Fail(#x,__LINE__);}while(0)
static unsigned long Word(const void* p,unsigned offset){return *reinterpret_cast<const unsigned long*>(static_cast<const unsigned char*>(p)+offset);}
extern "C" void __cdecl ColdBankVirtual(){Fail("cold virtual or destructor called",__LINE__);}
extern "C" void* __cdecl FixtureBankInitMalloc(size_t size){
 const unsigned expected[]={24,24,24,52,20,20,16};
 CHECK(g_object && g_allocations<(g_async?7u:4u) && size==expected[g_allocations]);
 if(g_async){
  unsigned long fill=static_cast<unsigned long>(g_fill)*0x01010101;
  CHECK(Word(g_object,0x140)==fill && g_object[0x148]==g_fill && g_object[0x149]==g_fill);
 }
 unsigned char* raw=static_cast<unsigned char*>(malloc(size+32));CHECK(raw);memset(raw,g_fill,size+32);
 g_blocks[g_allocations]=raw+16;g_sizes[g_allocations]=size;++g_allocations;return raw+16;
}
static void Run(bool async,unsigned char fill,unsigned seed){
 unsigned long storage[(0x164+32)/4];memset(storage,fill,sizeof(storage));g_object=reinterpret_cast<unsigned char*>(storage)+16;g_async=async;g_fill=fill;g_allocations=0;
 g_CCharStringInstanceCount_013BD800=seed;g_CWideStringInstanceCount_013BCA20=~seed;
 void* result=async?FixtureAsyncCtor(g_object,0):FixtureBankCtor(g_object,0);CHECK(result==g_object);
 for(unsigned packed=0x38;packed<=0x68;packed+=0x10)CHECK(!Word(g_object,packed) && !Word(g_object,packed+4) && !g_object[packed+8] && !Word(g_object,packed+12));
 CHECK(!g_object[0x10c]);
 CHECK(g_allocations==(async?7u:4u));CHECK(Word(g_object,0x98)==1 && Word(g_object,0x9c)==0x73 && Word(g_object,0x108)==0x10000);
 CHECK(static_cast<unsigned>(g_CCharStringInstanceCount_013BD800)==seed+2 && static_cast<unsigned>(g_CWideStringInstanceCount_013BCA20)==~seed+2);
 const unsigned offsets[]={0xac,0xb8,0xc4,0xfc,0x128,0x134,0x144};
 for(unsigned i=0;i<g_allocations;++i){
  unsigned char* p=static_cast<unsigned char*>(g_blocks[i]);CHECK(Word(g_object,offsets[i])==reinterpret_cast<unsigned long>(p));
  if(g_sizes[i]==16)CHECK(Word(p,0)==reinterpret_cast<unsigned long>(p) && Word(p,4)==reinterpret_cast<unsigned long>(p));
  else CHECK(!p[0] && !Word(p,4) && Word(p,8)==reinterpret_cast<unsigned long>(p) && Word(p,12)==reinterpret_cast<unsigned long>(p) && !Word(g_object,offsets[i]+4));
  for(unsigned k=0;k<16;++k)CHECK(p[static_cast<int>(k)-16]==fill && p[g_sizes[i]+k]==fill);
 }
 if(async){
  CHECK(!Word(g_object,0x140) && !g_object[0x148] && !g_object[0x149]);
  CRITICAL_SECTION* cs=reinterpret_cast<CRITICAL_SECTION*>(g_object+0x14c);
  CHECK(cs->LockCount==-1 && cs->RecursionCount==0 && !cs->OwningThread);
  EnterCriticalSection(cs);CHECK(cs->RecursionCount==1 && cs->OwningThread==reinterpret_cast<HANDLE>(GetCurrentThreadId()));
  CHECK(TryEnterCriticalSection(cs) && cs->RecursionCount==2);LeaveCriticalSection(cs);LeaveCriticalSection(cs);DeleteCriticalSection(cs);
 }
 const unsigned objectSize=async?0x164:0x110;
 for(unsigned k=0;k<16;++k)CHECK(g_object[static_cast<int>(k)-16]==fill && g_object[objectSize+k]==fill);
 for(unsigned i=0;i<g_allocations;++i)free(static_cast<unsigned char*>(g_blocks[i])-16);
 g_object=0;
}
int main(){
 const unsigned char fills[]={0,0x95,0xcd,0xff};const unsigned seeds[]={0,7,0xffffffff};
 unsigned cases=0;for(unsigned a=0;a<2;++a)for(unsigned f=0;f<4;++f)for(unsigned s=0;s<3;++s){Run(a!=0,fills[f],seeds[s]);++cases;}
 printf("BANK_INITIALIZATION_CHAIN_HOST_PASS cases=%u actual_base_helpers=1 allocation_order=1 windows_locks=12\n",cases);return 0;
}
