// Actual request allocation, critical section, event and overlap lifetime; map slots controlled.
#include "fable_threaded_file_object.h"
#include "NativeThreadedReadAPC_win32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
}
static void Fail(const char* e,int line) {fprintf(stderr,"NATIVE_MANAGER_ADD_FAILURE %d %s\n",line,e);fflush(stderr);ExitProcess(81);}
#define CHECK(x) do {if(!(x))Fail(#x,__LINE__);}while(0)
static unsigned char* g_block;
static CThreadedFileManager::CFileObject* g_allocated;
static CThreadedFileManager::CFileObject* g_uidSlot;
static CThreadedFileManager::CFileObject* g_pendingSlot;
static unsigned char* g_manager;
static unsigned g_uid,g_device,g_priority,g_calls,g_frees;
void* __cdecl operator new(size_t size) {
 CHECK(size==64 && !g_block);g_block=static_cast<unsigned char*>(malloc(96));CHECK(g_block);memset(g_block,0xcd,96);
 g_allocated=reinterpret_cast<CThreadedFileManager::CFileObject*>(g_block+16);return g_allocated;
}
void __cdecl operator delete(void* p) {
 CHECK(p==g_allocated && g_block);for(unsigned i=0;i<16;++i)CHECK(g_block[i]==0xcd && g_block[80+i]==0xcd);
 free(g_block);g_block=0;g_allocated=0;++g_frees;
}
namespace _STL {
template<> CThreadedFileManager::CFileObject*& map<unsigned long,CThreadedFileManager::CFileObject*>::operator[](const unsigned long& uid) {
 CHECK(reinterpret_cast<unsigned char*>(this)==g_manager+0x13c && uid==g_uid && g_calls==0 && !g_uidSlot);++g_calls;return g_uidSlot;
}
template<> CThreadedFileManager::CFileObject*& map<CThreadedFileManager::CPendingObjectSortKey,CThreadedFileManager::CFileObject*>::operator[](const CThreadedFileManager::CPendingObjectSortKey& key) {
 CHECK(reinterpret_cast<unsigned char*>(this)==g_manager+4+(g_device*7+g_priority)*16 && key.Object==g_allocated);
 CHECK(g_calls==1 && g_uidSlot==g_allocated && !g_pendingSlot);++g_calls;return g_pendingSlot;
}
}
static void Put(void* p,unsigned offset,unsigned long value) {memcpy(static_cast<unsigned char*>(p)+offset,&value,4);}
int main() {
 const unsigned uids[4]={0,1,0xfffffffe,0xffffffff};unsigned cases=0;
 for(unsigned u=0;u<4;++u)for(unsigned device=0;device<2;++device)for(unsigned priority=1;priority<7;++priority)for(unsigned seed=0;seed<3;++seed) {
  double managerWords[51],fileWords[8];unsigned char* manager=reinterpret_cast<unsigned char*>(managerWords)+16;
  unsigned char* file=reinterpret_cast<unsigned char*>(fileWords)+16;memset(managerWords,0xa5,sizeof(managerWords));memset(fileWords,0xb6,sizeof(fileWords));
  InitializeCriticalSection(manager+0x14c);HANDLE event=CreateEventA(0,0,0,0);CHECK(event);
  Put(manager,0x164,reinterpret_cast<unsigned long>(event));Put(manager,0x168,uids[u]);Put(file,4,device);
  CThreadedFileObjectDesc desc;unsigned char* bytes=reinterpret_cast<unsigned char*>(&desc);for(unsigned j=0;j<24;++j)bytes[j]=static_cast<unsigned char>(j*17+seed*31);
  desc.QueueWhenFinished=seed%2!=0;desc.Priority=static_cast<EThreadedFilePriority>(priority);
  unsigned char expectedManager[sizeof(managerWords)],expectedFile[sizeof(fileWords)],expectedObject[64];
  memcpy(expectedManager,managerWords,sizeof(managerWords));memcpy(expectedFile,fileWords,sizeof(fileWords));memset(expectedObject,0xcd,64);
  memcpy(expectedObject,&desc,24);Put(expectedObject,0x18,uids[u]);Put(expectedObject,0x1c,0);Put(expectedObject,0x24,reinterpret_cast<unsigned long>(manager));Put(expectedObject,0x28,reinterpret_cast<unsigned long>(file));
  g_manager=manager;g_uid=uids[u];g_device=device;g_priority=priority;g_calls=0;g_uidSlot=g_pendingSlot=0;
  unsigned returned=reinterpret_cast<CThreadedFileManager*>(manager)->AddObject(desc,*reinterpret_cast<CThreadedFile*>(file));
  CHECK(returned==uids[u] && g_calls==2 && g_uidSlot==g_allocated && g_pendingSlot==g_allocated && g_allocated->Overlapped);
  Put(expectedObject,0x20,reinterpret_cast<unsigned long>(g_allocated->Overlapped));CHECK(memcmp(expectedObject,g_allocated,64)==0);
  for(unsigned j=0;j<20;++j)CHECK(reinterpret_cast<unsigned char*>(g_allocated->Overlapped)[j]==0);
  CHECK(WaitForSingleObject(event,0)==WAIT_OBJECT_0 && WaitForSingleObject(event,0)==WAIT_TIMEOUT);
  Put(expectedManager,16+0x168,uids[u]+1);CHECK(memcmp(expectedManager,managerWords,sizeof(managerWords))==0 && memcmp(expectedFile,fileWords,sizeof(fileWords))==0);
  delete g_allocated;CHECK(!g_block && !g_allocated && g_frees==cases+1);DeleteCriticalSection(manager+0x14c);CHECK(CloseHandle(event));++cases;
 }
 CHECK(cases==144 && g_frees==144);printf("NATIVE_MANAGER_ADD_PASS cases=144 frees=144 maps=288\n");return 0;
}
