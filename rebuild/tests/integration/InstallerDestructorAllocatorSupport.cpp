#include "fable_memory_allocator.h"
#include "fable_variable_size_allocator.h"
#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
extern "C" __declspec(dllimport) int __stdcall HeapFree(void*,unsigned long,void*);
extern "C" __declspec(dllimport) int __stdcall HeapValidate(void*,unsigned long,const void*);
struct MemoryInfo{void* base;void* allocation;unsigned long allocationProtection,regionSize,state,protection,type;};
extern "C" __declspec(dllimport) unsigned long __stdcall VirtualQuery(const void*,MemoryInfo*,unsigned long);
class Access:public CMemoryAllocator{public:static CAllocatorInfo* Table(){return AllocatorInfo;}};
static void require(bool ok){if(!ok)abort();}
extern "C" void* __cdecl AllocateInstallerBuffer(unsigned long size){
 unsigned char* aligned=static_cast<unsigned char*>(CMemoryAllocator::AllocateAligned(size,16));require(aligned!=0);return *reinterpret_cast<void**>(aligned-4);
}
extern "C" void __cdecl CheckInstallerAllocatorEmpty(){
 require(g_FableMemoryAllocator_013CAA90!=0);
 require(*reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(g_FableMemoryAllocator_013CAA90)+0x40c)==0);
 require(HeapValidate(GetProcessHeap(),0,0)!=0);
}
extern "C" void __cdecl FinishInstallerAllocator(){
 void* chunks[128];unsigned kinds[128],count=0;
 for(unsigned row=0;row<30;++row){
  CMemoryAllocatorBase* owner=Access::Table()[row].PAllocator;
  if(row<28){for(CMemoryAllocatorFixedSize::CMemoryAreaChunk* c=static_cast<CMemoryAllocatorFixedSize*>(owner)->PFirstChunk;c;c=c->PNextChunk){require(count<128);chunks[count]=c->ThisChunkMem;kinds[count++]=0;}}
  else{for(CMemoryAllocatorVariableSize::CMemoryAreaChunk* c=static_cast<CMemoryAllocatorVariableSize*>(owner)->PFirstChunk;c;c=c->PNextChunk){require(count<128);chunks[count]=c;kinds[count++]=1;}}
 }
 FableMemoryAllocatorShutdown_009f2d20();require(g_FableMemoryAllocator_013CAA90==0);
 for(unsigned i=0;i<count;++i){if(kinds[i]){MemoryInfo info;require(VirtualQuery(chunks[i],&info,sizeof(info))==sizeof(info));if(info.state==0x1000)require(VirtualFree(chunks[i],0,0x8000)!=0);}else require(HeapFree(GetProcessHeap(),1,chunks[i])!=0);}
 require(HeapValidate(GetProcessHeap(),0,0)!=0);
}
