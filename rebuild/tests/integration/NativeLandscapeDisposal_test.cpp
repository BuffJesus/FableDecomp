#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#include "fable_memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include "landscape_disposal_aliases.h"
#include "landscape_disposal_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Drain(void*);
extern "C" void __fastcall Toggle(void*,void*,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mode,depth,events;
#include "fable_surface_types.h"
static unsigned active;
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
bool g_FableAllocatorShutdownRequested_013CAA94;
static void record(unsigned kind,void* owner,void* arg){
 if(++events>1000)exit(15);unsigned h[]={kind,(unsigned)owner,(unsigned)arg,(unsigned)g_FableMemoryAllocator_013CAA90,g_FableAllocatorShutdownRequested_013CAA94,depth};
 if(fwrite(h,4,6,output)!=6||fwrite(arena,1,8192,output)!=8192)exit(9);
}
void __fastcall CMemoryAllocator::EnterSynchronisation(){record(1,0,0);++depth;if(mode==1)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(arena+0x800);}
void __fastcall CMemoryAllocator::LeaveSynchronisation(){record(5,0,0);if(depth!=1)exit(10);--depth;}
void CMemoryAllocator::InternalDeallocate(void* p){record(2,this,p);if(mode==3)g_FableAllocatorShutdownRequested_013CAA94=true;if(mode==2)*(unsigned*)((unsigned char*)this+0x40c)=0;}
CMemoryAllocator::~CMemoryAllocator(){record(3,this,0);if(mode==5)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(arena+0x1000);}
void __fastcall CMemoryAllocator::operator delete(void* p){record(4,p,0);}
// Public wrappers preserve the actual protected allocator callback ABI.
extern "C" void __fastcall TextureBoundary(void*);
extern "C" void __fastcall EnterBoundary();extern "C" void __fastcall LeaveBoundary();
extern "C" void __fastcall DeallocateBoundary(void*,void*,void*);extern "C" void __fastcall DestroyBoundary(void*);extern "C" void __fastcall DeleteBoundary(void*);
static void resource(void* p){
 record(7,p,0);active=(unsigned)p;
 if(mode==3){
  // Replace the current list head with its successor before pop_front rereads it.
  for(unsigned i=0;i<9;++i){unsigned* head=*(unsigned**)(arena+0x10+i*4);unsigned* node=(unsigned*)head[0];if(node!=head&&node[2]==(unsigned)p&&node[0]!=(unsigned)head){unsigned* next=(unsigned*)node[0];head[0]=(unsigned)next;next[1]=(unsigned)head;break;}}
 }
 if(mode==4)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(arena+0x800);
 *(unsigned*)((unsigned char*)p+4)=0x76543210;
}
void CTexture::Uninitialise(){resource(this);}
extern "C" void* __fastcall DestroyResource(void* p,void*,unsigned flags){if(flags)exit(14);resource(p);return p;}
extern "C" void __cdecl DisposalFree(void* p){record(8,p,0);memset(p,0xdd,12);}
static void* resourceVtable[]={DestroyResource};
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;
  switch(retailFixups[i][1]){case 0xbd9ce0:v=(unsigned)code;break;case 0x9f2000:v=(unsigned)EnterBoundary;break;case 0x9f2010:v=(unsigned)LeaveBoundary;break;case 0x9f2920:v=(unsigned)DeallocateBoundary;break;case 0x9f2310:v=(unsigned)DestroyBoundary;break;case 0x9f1f90:v=(unsigned)DeleteBoundary;break;case 0x9f1f10:v=(unsigned)(code+512);break;case 0x9f9f70:v=(unsigned)TextureBoundary;break;case 0xbfea14:v=(unsigned)DisposalFree;break;case 0x13caa90:v=(unsigned)&g_FableMemoryAllocator_013CAA90;break;case 0x13caa94:v=(unsigned)&g_FableAllocatorShutdownRequested_013CAA94;break;default:return 8;}
  *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;
 }
 typedef void (__fastcall *Call)(void*);Call run=argv[3][0]=='r'?(Call)code:Drain;typedef void (__fastcall *PoolCall)(void*,void*,unsigned long);PoolCall toggle=argv[3][0]=='r'?(PoolCall)(code+544):Toggle;
 unsigned p[3],cases=0;
 while(fread(p,4,3,in)==3){if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];depth=events=0;g_FableMemoryAllocator_013CAA90=p[1]?(CMemoryAllocator*)(arena+0x200):0;g_FableAllocatorShutdownRequested_013CAA94=p[2]!=0;
  for(unsigned i=0;i<27;++i)*(void**)(arena+0x1800+i*16)=resourceVtable;
  toggle(arena,0,mode&1);record(6,0,0);run(arena);record(9,0,0);if(depth)return 6;++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_LANDSCAPE_DISPOSAL_PASS cases=%u\n",cases);return 0;
}
