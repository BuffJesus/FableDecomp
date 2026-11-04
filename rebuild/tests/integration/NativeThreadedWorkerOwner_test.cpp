// Owned Windows worker submission probe; no installed-game process is started.
#include "fable_threaded_file_object.h"
#include "fable_file_installer.h"
#include "NativeThreadedReadAPC_win32.h"
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
__declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int,size_t);
__declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
__declspec(dllimport) DWORD __stdcall WaitForSingleObjectEx(HANDLE,DWORD,BOOL);
__declspec(dllimport) DWORD __stdcall GetCurrentThreadId();
__declspec(dllimport) int __stdcall GetThreadPriority(HANDLE);
__declspec(dllimport) HANDLE __stdcall GetCurrentThread();
__declspec(dllimport) BOOL __stdcall DuplicateHandle(HANDLE,HANDLE,HANDLE,HANDLE*,DWORD,BOOL,DWORD);
void __cdecl FixtureBeginThread(void*);
__declspec(dllimport) BOOL __stdcall WriteFileEx(HANDLE,const void*,DWORD,OVERLAPPED*,IOCompletion);
void __fastcall FixtureProcess(CThreadedFileManager*,void*);
void __stdcall FixtureReadCompletion(DWORD,DWORD,OVERLAPPED*);
typedef void (__fastcall* HardError)(const char*);
extern HardError FixtureHardError;
}
#pragma comment(linker,"/alternatename:@FixtureProcess@8=?ProcessObjects@CThreadedFileManager@@AAEXXZ")
#pragma comment(linker,"/alternatename:_FixtureBeginThread=?BeginThread@CThreadedFileManager@@CAXPAX@Z")
#pragma comment(linker,"/alternatename:_FixtureReadCompletion@12=?OnFileReadCompletion@CThreadedFileManager@@CGXKKPAU_OVERLAPPED@@@Z")
#pragma comment(linker,"/alternatename:_FixtureHardError=?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA")
#pragma section(".coldmgr",read,write)
extern "C" __declspec(allocate(".coldmgr")) __declspec(align(4096)) unsigned char FixtureColdGlobalManager[4096]={0};
#pragma comment(linker,"/alternatename:?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A=_FixtureColdGlobalManager")
static void Fail(const char* expr,int line) {
 fprintf(stderr,"NATIVE_WORKER_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK(x) do {if(!(x)) Fail(#x,__LINE__);}while(0)
CCountedFileInstaller g_CFileInstallerSingleton_013CA818={0,0};
void* __cdecl FableFileInstallerAllocate(fable_u32) {Fail("unexpected installer allocation",__LINE__);return 0;}
CFileInstaller::CFileInstaller() {Fail("unexpected installer constructor",__LINE__);}
CFileInstaller::~CFileInstaller() {Fail("unexpected installer destructor",__LINE__);}
void CCountedFileInstaller::operator=(CFileInstaller*) {Fail("unexpected singleton assignment",__LINE__);}
extern "C" bool FixtureInstallerDirty;
#pragma comment(linker,"/alternatename:_FixtureInstallerDirty=?DirtyDisc@CFileInstaller@@1_NA")
static unsigned long* g_installer;
static DWORD g_afterCycle;
double __fastcall GFGetTime();
extern bool g_FableTimerInitialised_013CAA10;
extern bool g_FableHighResolutionTimer_013CAA00;
extern __int64 g_FableTimerFrequency_013CAA08;
static unsigned g_removed,g_finished,g_begun;
static HANDLE g_finishEvent;
static DWORD g_mainThread,g_workerThread;
static HANDLE g_ownedWorker;
static bool g_allow;
static CThreadedFileManager::CFileObject* g_request;
static unsigned long* g_manager;
void CThreadedFileManager::RemoveObjectFromPendingMap(CFileObject* object) {
 CHECK(object==g_request && reinterpret_cast<unsigned long*>(this)==g_manager);
 CHECK(object->LoadingStatus==(g_begun ? FILE_LOADING_STATUS_PENDING_LOCKED : FILE_LOADING_STATUS_PENDING));
 CHECK(g_manager[(0x14c+8)/4]==1);
 CHECK(GetCurrentThreadId()!=g_mainThread);g_workerThread=GetCurrentThreadId();
 CHECK(GetThreadPriority(GetCurrentThread())==1);
 unsigned d=object->PFile->deviceId_,p=object->Desc.Priority;
 unsigned q=1+(d*7+p)*4;unsigned long* h=reinterpret_cast<unsigned long*>(g_manager[q]);
 void* oldNode=reinterpret_cast<void*>(h[1]);CHECK(oldNode);
 h[1]=0;h[2]=h[3]=reinterpret_cast<DWORD>(h);g_manager[q+1]=0;g_manager[q+3]=reinterpret_cast<DWORD>(h);free(oldNode);
 CHECK(!g_ownedWorker && DuplicateHandle(GetCurrentProcess(),GetCurrentThread(),GetCurrentProcess(),&g_ownedWorker,0,0,2));
 ++g_removed;
}
typedef _STL::map<unsigned long,CThreadedFileManager::CFileObject*>::_Rep_type FixtureUidTree;
static unsigned g_completionCalls;
static void __fastcall Complete(void* self,void*,bool success) {
 CHECK(self && success && GetCurrentThreadId()==g_workerThread);
 CHECK(g_request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_READ_COMPLETED);
 CHECK(g_manager[(0x14c+8)/4]==0 && g_installer[(4+8)/4]==0 && g_installer[0x7c/4]==g_afterCycle);++g_completionCalls;
}
static bool __fastcall Begin(void* self,void*,void** dst,EThreadedFilePriority priority) {
 CHECK(self && dst==&g_request->Desc.PDst && priority==g_request->Desc.Priority);
 CHECK(g_request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED);
 CHECK(g_manager[(0x14c+8)/4]==0);
 ++g_begun;return g_allow;
}
static unsigned __stdcall Launch(void* context) {FixtureBeginThread(context);return 0;}
static unsigned char Payload(unsigned i) {return static_cast<unsigned char>(i*37+23);}
static DWORD Run(HANDLE file,unsigned priority,unsigned device,unsigned offset,unsigned length,bool callback,bool write,DWORD paused,bool running,bool dirty) {
 unsigned long installer[0xa0/4];memset(installer,0,sizeof(installer));g_installer=installer;
 CFileInstaller* installerObject=reinterpret_cast<CFileInstaller*>(installer);
 installerObject->Running=running;installerObject->Paused=paused;FixtureInstallerDirty=dirty;
 HANDLE installerEvent=CreateEventA(0,1,0,0),pauseStart=CreateEventA(0,1,1,0),pauseFinish=CreateEventA(0,1,0,0);
 CHECK(installerEvent && pauseStart && pauseFinish);
 installerObject->Event=installerEvent;installerObject->PauseStartEvent=pauseStart;installerObject->PauseFinishEvent=pauseFinish;
 InitializeCriticalSection(installerObject->CriticalSection);
 g_CFileInstallerSingleton_013CA818.object=installerObject;
 DWORD afterPause=paused+1;g_afterCycle=afterPause ? afterPause-1:0;
 CThreadedFileManager* owner=new CThreadedFileManager;unsigned long* manager=reinterpret_cast<unsigned long*>(owner);
 EnterCriticalSection(manager+0x14c/4);
 for(unsigned i=0;i<15;++i) {
  unsigned at=i==14 ? 0x13c/4 : 1+i*4;unsigned long* header=reinterpret_cast<unsigned long*>(manager[at]);
  CHECK(header && !manager[at+1] && !header[1] && header[2]==reinterpret_cast<DWORD>(header) && header[3]==reinterpret_cast<DWORD>(header));
  if(i<14)CHECK(manager[at+3]==reinterpret_cast<DWORD>(header));
  for(unsigned j=0;j<i;++j)CHECK(manager[j==14 ? 0x13c/4 : 1+j*4]!=manager[at]);
 }
 CHECK(!manager[0x138/4] && manager[0x168/4]==1 && !manager[0x16c/4]);
 unsigned long* node=static_cast<unsigned long*>(calloc(6,4));CHECK(node);
 unsigned long fileStorage[0x1c/4];memset(fileStorage,0,sizeof(fileStorage));fileStorage[1]=device;fileStorage[2]=reinterpret_cast<DWORD>(file);
 OVERLAPPED* allocatedOverlap=static_cast<OVERLAPPED*>(GlobalAlloc(0,sizeof(OVERLAPPED)));CHECK(allocatedOverlap);OVERLAPPED& ov=*allocatedOverlap;memset(&ov,0xab,sizeof(ov));unsigned char buffer[8192+32];memset(buffer,0xcd,sizeof(buffer));
 reinterpret_cast<unsigned char*>(fileStorage)[0x18]=write;
 if(write)for(unsigned i=0;i<length;++i)buffer[16+i]=Payload(offset+i)^0x5a;
 CThreadedFileManager::CFileObject& request=*new CThreadedFileManager::CFileObject;memset(&request,0,sizeof(request));
 request.Desc.PDst=buffer+16;request.Desc.FilePos=offset;request.Desc.Size=length;request.Desc.Priority=static_cast<EThreadedFilePriority>(priority);
 request.PFile=reinterpret_cast<CThreadedFile*>(fileStorage);request.PManager=reinterpret_cast<CThreadedFileManager*>(manager);request.Overlapped=&ov;
 request.Desc.QueueWhenFinished=true;
 void* vtable[3]={0,reinterpret_cast<void*>(Begin),reinterpret_cast<void*>(Complete)};void* cb=vtable;request.Desc.PCallback=callback ? reinterpret_cast<CThreadedFileCallback*>(&cb):0;
 g_request=&request;g_manager=manager;g_allow=true;g_begun=g_removed=g_finished=g_completionCalls=0;
 {
  unsigned q=1+(device*7+priority)*4;unsigned long* h=reinterpret_cast<unsigned long*>(manager[q]);
  CHECK(h && manager[q+1]==0 && manager[q+3]==reinterpret_cast<DWORD>(h));
  node[0]=1;node[1]=reinterpret_cast<DWORD>(h);node[4]=node[5]=reinterpret_cast<DWORD>(&request);
  h[1]=h[2]=h[3]=reinterpret_cast<DWORD>(node);manager[q+1]=1;
 }
 for(unsigned p=0;p<7;++p)manager[0xe4/4+p]=1-device;
 reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(manager)+0x100)[priority]=9.0;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[0]=0.25f;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[1]=0.5f;
 unsigned long* uidHeader=reinterpret_cast<unsigned long*>(manager[0x13c/4]);unsigned long* uidNode=static_cast<unsigned long*>(calloc(6,4));CHECK(uidHeader && uidNode);
 uidNode[0]=1;uidNode[1]=reinterpret_cast<DWORD>(uidHeader);uidNode[4]=1234;uidNode[5]=reinterpret_cast<DWORD>(&request);
 uidHeader[1]=uidHeader[2]=uidHeader[3]=reinterpret_cast<DWORD>(uidNode);manager[0x140/4]=1;request.UID=1234;
 CHECK(manager[0x164/4] && reinterpret_cast<unsigned char*>(manager)[0x148]);
 g_finishEvent=CreateEventA(0,1,0,0);CHECK(g_finishEvent);g_mainThread=GetCurrentThreadId();g_workerThread=0;g_ownedWorker=0;
 const CThreadedFileManager& view=*owner;
 CHECK(view.ObjectExists(request.UID) && !view.IsObjectFinished(request.UID));
 LeaveCriticalSection(manager+0x14c/4);owner->KeepUpdating();
 DWORD started=GetTickCount();
 while(!view.IsObjectFinished(request.UID)) {
  CHECK(GetTickCount()-started<5000);Sleep(1);
 }
 CHECK(view.ObjectExists(request.UID));
 CHECK(view.IsObjectFinished(0x7fffffffUL) && !view.ObjectExists(0x7fffffffUL));
 g_finished=1;
 DWORD hash=2166136261UL;
 {
  CHECK(g_removed==1 && request.LoadingStatus==4 && manager[0x138/4]==0);
  CHECK(ov.Offset==offset && ov.OffsetHigh==0 && ov.hEvent==&request);
  unsigned transferred=offset>=0x9000 ? 0 : ((0x9000-offset)<length ? (0x9000-offset):length);
  for(unsigned i=0;i<transferred;++i) {CHECK(buffer[16+i]==(write ? (Payload(offset+i)^0x5a):Payload(offset+i)));hash=(hash^buffer[16+i])*16777619UL;}
  for(unsigned i=0;i<16;++i)CHECK(buffer[i]==0xcd);
  for(unsigned i=16+transferred;i<sizeof(buffer);++i)CHECK(buffer[i]==0xcd);
 }
 reinterpret_cast<CThreadedFileManager*>(manager)->Uninitialise();
 HANDLE thread=g_ownedWorker;CHECK(thread && WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);
 CHECK(g_workerThread && g_workerThread!=g_mainThread);
 CHECK(g_finished==1 && g_begun==unsigned(callback && !write));
 CHECK(g_completionCalls==unsigned(callback));
 CHECK(!reinterpret_cast<unsigned char*>(manager)[0x148] && reinterpret_cast<unsigned char*>(manager)[0x149] && reinterpret_cast<unsigned char*>(manager)[0x14a]);
 CHECK(manager[0x164/4]==0);
 CHECK(manager[0x140/4]==1 && uidHeader[1]==reinterpret_cast<DWORD>(uidNode));
 CHECK(installerObject->Paused==g_afterCycle && installerObject->Running==running && FixtureInstallerDirty==dirty);
 bool pauseSignal=running && afterPause<=1 && !dirty,unpauseSignal=running && afterPause==1;
 CHECK((WaitForSingleObject(installerEvent,0)==WAIT_OBJECT_0)==(pauseSignal || unpauseSignal));
 CHECK((WaitForSingleObject(pauseFinish,0)==WAIT_OBJECT_0)==unpauseSignal);
 CHECK(g_installer[(4+8)/4]==0);
 DeleteCriticalSection(installerObject->CriticalSection);
 CHECK(CloseHandle(installerEvent) && CloseHandle(pauseStart) && CloseHandle(pauseFinish));
 g_CFileInstallerSingleton_013CA818.object=0;g_installer=0;
 delete owner;
 CHECK(CloseHandle(thread) && CloseHandle(g_finishEvent));g_ownedWorker=0;g_finishEvent=0;g_request=0;g_manager=0;return hash;
}
int main(int argc,char** argv) {
 CHECK(argc==3 && !FixtureHardError);
 // File-wrapper globals are cold link dependencies; any accidental use faults.
 DWORD oldProtection;CHECK(VirtualProtect(FixtureColdGlobalManager,sizeof(FixtureColdGlobalManager),1,&oldProtection));
 CHECK(!g_FableTimerInitialised_013CAA10);
 HANDLE input=CreateFileA(argv[1],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
 HANDLE output=CreateFileA(argv[2],GENERIC_READ|0x40000000UL,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
 CHECK(input!=INVALID_HANDLE_VALUE && output!=INVALID_HANDLE_VALUE);
 unsigned count=0;
 DWORD pauseCounts[3]={0,1,0xffffffffUL};
 for(unsigned pi=0;pi<3;++pi)for(unsigned running=0;running<2;++running)for(unsigned dirty=0;dirty<2;++dirty)for(unsigned w=0;w<2;++w)for(unsigned p=0;p<7;++p)for(unsigned d=0;d<2;++d)for(unsigned cb=0;cb<2;++cb) {
  Run(w ? output:input,p,d,p*17,17+p,cb!=0,w!=0,pauseCounts[pi],running!=0,dirty!=0);++count;
 }
 CHECK(CloseHandle(input) && CloseHandle(output));
 CHECK(g_FableTimerInitialised_013CAA10 && g_FableHighResolutionTimer_013CAA00 && g_FableTimerFrequency_013CAA08>0);
 CHECK(GFGetTime()>0.0);
 printf("NATIVE_WORKER_OWNER_PASS cases=%u reads=336 writes=336 joins=672 shutdowns=672 callbacks=336 exit_objects=672\n",count);return 0;
}
