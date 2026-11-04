typedef unsigned long DWORD;
typedef void* HANDLE;
enum {FALSE=0,WAIT_OBJECT_0=0,ERROR_INVALID_HANDLE=6};
extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
__declspec(dllimport) HANDLE __stdcall CreateEventA(void*,int,int,const char*);
__declspec(dllimport) HANDLE __stdcall CreateThread(void*,unsigned long,DWORD(__stdcall*)(void*),void*,unsigned long,unsigned long*);
__declspec(dllimport) DWORD __stdcall WaitForSingleObject(HANDLE,DWORD);
__declspec(dllimport) int __stdcall CloseHandle(HANDLE);
__declspec(dllimport) int __stdcall GetHandleInformation(HANDLE,DWORD*);
__declspec(dllimport) DWORD __stdcall GetLastError();
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fable_file_installer_active_files.h"
#include "fable_retail_banks.h"
static void require(bool ok){if(!ok)abort();}
static unsigned clearCalls,listCalls;
struct Worker {unsigned char* bytes;HANDLE event;unsigned flag;};
static DWORD __stdcall FinishWorker(void* p){
 Worker& worker=*static_cast<Worker*>(p);require(WaitForSingleObject(worker.event,10000)==WAIT_OBJECT_0);
 void* cs=worker.bytes+4;EnterCriticalSection(cs);
 require(worker.bytes[0x7a]==1);worker.bytes[worker.flag]=1;LeaveCriticalSection(cs);return 0;
}
// This fixture validates OS shutdown; container bodies remain recorded boundaries.
void CFileInstaller::ClearBuffers(){
 unsigned char* cs=reinterpret_cast<unsigned char*>(this)+4;
 require(*reinterpret_cast<long*>(cs+8)==1);EnterCriticalSection(cs);require(*reinterpret_cast<long*>(cs+8)==2);LeaveCriticalSection(cs);++clearCalls;
}
void InstallerActiveFileList::Clear(){require(head->next==head&&head->previous==head);++listCalls;}
void InstallerCountedFileList::Clear(){require(head->next==head&&head->previous==head);++listCalls;}
void InstallerCompletedFileSet::Erase(Node*){abort();}
void CBasicRetailBankAliasMap::Erase(CNode*){abort();}
void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk* first,InstallerBufferChunk* last,void*){require(!first&&!last);}
int main(){
 unsigned cases=0,threads=0;
 for(unsigned cycle=0;cycle!=32;++cycle)for(unsigned mode=0;mode!=3;++mode){
  unsigned long storage[sizeof(CFileInstaller)/4];memset(storage,0,sizeof(storage));unsigned char* bytes=reinterpret_cast<unsigned char*>(storage);
  InitializeCriticalSection(bytes+4);
  for(unsigned off=0x38;off<=0x40;off+=4){void** head=static_cast<void**>(malloc(8));require(head!=0);head[0]=head[1]=head;*reinterpret_cast<void***>(bytes+off)=head;}
  *reinterpret_cast<void**>(bytes+0x44)=malloc(20);*reinterpret_cast<void**>(bytes+0x94)=malloc(24);require(*reinterpret_cast<void**>(bytes+0x44)&&*reinterpret_cast<void**>(bytes+0x94));
  HANDLE events[4];for(unsigned i=0;i!=4;++i){events[i]=CreateEventA(0,FALSE,FALSE,0);require(events[i]!=0);*reinterpret_cast<HANDLE*>(bytes+0x84+i*4)=events[i];}
  bytes[0x78]=mode==0;bytes[0x79]=mode!=2;
  Worker workers[2];HANDLE running[2]={0,0};
  for(i=0;i!=mode;++i){workers[i].bytes=bytes;workers[i].event=events[i];workers[i].flag=0x78+i;running[i]=CreateThread(0,0,FinishWorker,&workers[i],0,0);require(running[i]!=0);++threads;}
  reinterpret_cast<CFileInstaller*>(bytes)->CFileInstaller::~CFileInstaller();
  require(bytes[0x78]&&bytes[0x79]&&bytes[0x7a]&&clearCalls==cases+1&&listCalls==(cases+1)*3);
  for(i=0;i!=mode;++i){require(WaitForSingleObject(running[i],10000)==WAIT_OBJECT_0);require(CloseHandle(running[i])!=0);}
  for(i=0;i!=4;++i){DWORD flags;require(!GetHandleInformation(events[i],&flags)&&GetLastError()==ERROR_INVALID_HANDLE);}
  ++cases;
 }
 printf("INSTALLER_DESTRUCTOR_OS_OK cases=%u fixture_threads=%u events=%u\n",cases,threads,cases*4);return 0;
}
