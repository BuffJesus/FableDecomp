#include "fable_counted_pointer.h"
#include "fable_disk_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) __declspec(noreturn) void __stdcall ExitProcess(unsigned);
static void Fail(const char* e,int n) {fprintf(stderr,"DISK_RESET_FAILURE %d %s\n",n,e);fflush(stderr);ExitProcess(80);}
#define CHECK(x) do {if(!(x))Fail(#x,__LINE__);}while(0)
static unsigned char oldBlock[44],newBlock[44],oldFile[60],newFile[60];
static void* oldInfo;static void* allocated;static unsigned oldDeletes,infoDeletes,allocations;static bool refuse;
static void* __fastcall DeleteSlot(void* file,void*,unsigned flags) {CHECK(file==oldFile+16 && flags==1 && !oldDeletes);++oldDeletes;return file;}
extern "C" void __fastcall DiskDelete(void*);
#pragma comment(linker,"/alternatename:@DiskDelete@4=?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@CIXPAX@Z")
void* __cdecl operator new(size_t size) {CHECK(size==12 && !allocations);++allocations;return refuse?0:allocated;}
void __cdecl operator delete(void* p) {CHECK(p==oldInfo && !infoDeletes);++infoDeletes;}
int main() {
 unsigned cases=0;void* vtable[1]={reinterpret_cast<void*>(DeleteSlot)};
 for(unsigned old=0;old<3;++old)for(unsigned incoming=0;incoming<2;++incoming)for(unsigned failure=0;failure<2;++failure)for(unsigned seed=0;seed<3;++seed) {
  memset(oldBlock,0xa0+seed,44);memset(newBlock,0xb0+seed,44);memset(oldFile,0xc0+seed,60);memset(newFile,0xd0+seed,60);
  *reinterpret_cast<void***>(oldFile+16)=vtable;
  oldInfo=oldBlock+16;allocated=newBlock+16;oldDeletes=infoDeletes=allocations=0;refuse=failure!=0;
  CCPPointerInfo* info=reinterpret_cast<CCPPointerInfo*>(oldInfo);info->RefCount=old==1?1:3;info->DeleteFunc=DiskDelete;info->Data=oldFile+16;
  unsigned long ownerWords[2]={reinterpret_cast<unsigned long>(oldFile+16),old?reinterpret_cast<unsigned long>(info):0};
  CCountedPointer<CDiskFileWin32>* owner=reinterpret_cast<CCountedPointer<CDiskFileWin32>*>(ownerWords);
  CDiskFileWin32* next=incoming?reinterpret_cast<CDiskFileWin32*>(newFile+16):0;owner->Reset(next);
  CHECK(owner->Data==next && owner->Info==(incoming&&!failure?allocated:0));
  CHECK(oldDeletes==(old==1) && infoDeletes==(old==1) && allocations==incoming);
  CHECK(info->RefCount==(old==1?0:old==2?2:3));
  if(owner->Info)CHECK(owner->Info->RefCount==1 && owner->Info->Data==next && owner->Info->DeleteFunc==DiskDelete);
  for(unsigned i=0;i<16;++i) {CHECK(oldBlock[i]==0xa0+seed && oldBlock[28+i]==0xa0+seed);CHECK(newBlock[i]==0xb0+seed && newBlock[28+i]==0xb0+seed);CHECK(oldFile[i]==0xc0+seed && oldFile[44+i]==0xc0+seed);CHECK(newFile[i]==0xd0+seed && newFile[44+i]==0xd0+seed);}
  ++cases;
 }
 DiskDelete(0);printf("DISK_RESET_HOST_PASS cases=%u\n",cases);return 0;
}
