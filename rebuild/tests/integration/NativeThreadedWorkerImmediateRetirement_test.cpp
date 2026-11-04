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
static bool g_allow;static unsigned g_finalPriority;
static CThreadedFileManager::CFileObject* g_request;
static unsigned long* g_manager;
static void* g_overlap;
static unsigned g_overlapFrees,g_requestDeletes;
static unsigned g_expectedOffset,g_expectedPriority;
typedef void* (__stdcall* GlobalFreeFunction)(void*);
extern "C" GlobalFreeFunction FixtureGlobalFreeImport;
#pragma comment(linker,"/alternatename:_FixtureGlobalFreeImport=__imp__GlobalFree@4")
static GlobalFreeFunction g_realGlobalFree;
static void* __stdcall ObserveGlobalFree(void* block) {
 CHECK(block==g_overlap && GetCurrentThreadId()==g_workerThread);
 CHECK(++g_overlapFrees==1 && !g_requestDeletes);
 return g_realGlobalFree(block);
}
void __cdecl operator delete(void* block) {
 if(block==g_request) {
  CHECK(GetCurrentThreadId()==g_workerThread && g_overlapFrees==1);
  CHECK(++g_requestDeletes==1);
 }
 free(block);
}

// Observe the newly created worker through an owned synchronization handle.
struct ThreadEntry {DWORD size,usage,id,process;long basePriority,deltaPriority;DWORD flags;};
extern "C" {
__declspec(dllimport) HANDLE __stdcall CreateToolhelp32Snapshot(DWORD,DWORD);
__declspec(dllimport) BOOL __stdcall Thread32First(HANDLE,ThreadEntry*);
__declspec(dllimport) BOOL __stdcall Thread32Next(HANDLE,ThreadEntry*);
__declspec(dllimport) DWORD __stdcall GetCurrentProcessId();
__declspec(dllimport) HANDLE __stdcall OpenThread(DWORD,BOOL,DWORD);
}
static unsigned Threads(DWORD* ids) {
 HANDLE snapshot=CreateToolhelp32Snapshot(4,0);CHECK(snapshot!=INVALID_HANDLE_VALUE);
 ThreadEntry entry;entry.size=sizeof(entry);unsigned count=0;
 BOOL found=Thread32First(snapshot,&entry);CHECK(found);
 do {if(entry.process==GetCurrentProcessId()) {CHECK(count<128);ids[count++]=entry.id;}}while(Thread32Next(snapshot,&entry));
 CHECK(CloseHandle(snapshot));return count;
}
static void CaptureWorker(DWORD* before,unsigned count) {
 DWORD after[128];unsigned amount=Threads(after);g_workerThread=0;g_ownedWorker=0;
 for(unsigned i=0;i<amount;++i) {unsigned n=0;while(n<count && before[n]!=after[i])++n;if(n==count) {CHECK(!g_workerThread);g_workerThread=after[i];}}
 CHECK(g_workerThread && g_workerThread!=g_mainThread);g_ownedWorker=OpenThread(0x00100040,0,g_workerThread);CHECK(g_ownedWorker);
}

typedef _STL::map<unsigned long,CThreadedFileManager::CFileObject*>::_Rep_type FixtureUidTree;
static unsigned g_completionCalls;
static void __fastcall Complete(void* self,void*,bool success) {
 CHECK(self && success && GetCurrentThreadId()==g_workerThread);
 CHECK(g_request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_READ_COMPLETED);
 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId() && g_installer[(4+8)/4]==0 && g_installer[0x7c/4]==g_afterCycle);++g_completionCalls;
 CHECK(!g_overlapFrees && !g_requestDeletes);
 CHECK(g_request->Desc.Priority==g_expectedPriority);
 OVERLAPPED* live=reinterpret_cast<OVERLAPPED*>(g_request->Overlapped);
 CHECK(live==g_overlap && live->Offset==g_expectedOffset && !live->OffsetHigh && live->hEvent==g_request);
}
static bool __fastcall Begin(void* self,void*,void** dst,EThreadedFilePriority priority) {
 CHECK(self && dst==&g_request->Desc.PDst && priority==g_request->Desc.Priority);
 CHECK(g_request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED);
 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId());
 ++g_begun;
 if(g_begun==1) {
  unsigned device=g_request->PFile->deviceId_,oldPriority=g_request->Desc.Priority;
  g_request->PManager->SetObjectPriority(g_request->UID,static_cast<EThreadedFilePriority>(g_finalPriority));
  CHECK(g_request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED && g_request->Desc.Priority==g_finalPriority);
  unsigned oldQueue=1+(device*7+oldPriority)*4,newQueue=1+(device*7+g_finalPriority)*4;
  unsigned long* oldHeader=reinterpret_cast<unsigned long*>(g_manager[oldQueue]);
  CHECK(oldPriority!=g_finalPriority && !g_manager[oldQueue+1] && !oldHeader[1] && g_manager[oldQueue+3]==reinterpret_cast<DWORD>(oldHeader));
  CHECK(g_manager[newQueue+1]==1 && g_manager[(0x14c+12)/4]!=GetCurrentThreadId());
  return false;
 }
 CHECK(g_begun==2 && g_request->Desc.Priority==g_finalPriority);return g_allow;
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
 DWORD previousThreads[128];unsigned previousCount=Threads(previousThreads);g_mainThread=GetCurrentThreadId();
 CThreadedFileManager* owner=new CThreadedFileManager;CaptureWorker(previousThreads,previousCount);unsigned long* manager=reinterpret_cast<unsigned long*>(owner);
 EnterCriticalSection(manager+0x14c/4);
 for(unsigned i=0;i<15;++i) {
  unsigned at=i==14 ? 0x13c/4 : 1+i*4;unsigned long* header=reinterpret_cast<unsigned long*>(manager[at]);
  CHECK(header && !manager[at+1] && !header[1] && header[2]==reinterpret_cast<DWORD>(header) && header[3]==reinterpret_cast<DWORD>(header));
  if(i<14)CHECK(manager[at+3]==reinterpret_cast<DWORD>(header));
  for(unsigned j=0;j<i;++j)CHECK(manager[j==14 ? 0x13c/4 : 1+j*4]!=manager[at]);
 }
 CHECK(!manager[0x138/4] && manager[0x168/4]==1 && !manager[0x16c/4]);
 unsigned long fileStorage[0x1c/4];memset(fileStorage,0,sizeof(fileStorage));fileStorage[1]=device;fileStorage[2]=reinterpret_cast<DWORD>(file);
 unsigned char buffer[8192+32];memset(buffer,0xcd,sizeof(buffer));
 reinterpret_cast<unsigned char*>(fileStorage)[0x18]=write;
 if(write)for(unsigned i=0;i<length;++i)buffer[16+i]=Payload(offset+i)^0x5a;
 CThreadedFileObjectDesc desc;memset(&desc,0,sizeof(desc));
 desc.PDst=buffer+16;desc.FilePos=offset;desc.Size=length;desc.Priority=static_cast<EThreadedFilePriority>(callback && !write?(priority+1)%7:priority);g_finalPriority=priority;desc.QueueWhenFinished=false;
 void* vtable[3]={0,reinterpret_cast<void*>(Begin),reinterpret_cast<void*>(Complete)};void* cb=vtable;desc.PCallback=callback?reinterpret_cast<CThreadedFileCallback*>(&cb):0;
 unsigned uid=owner->AddObject(desc,*reinterpret_cast<CThreadedFile*>(fileStorage));CHECK(uid==1);
 unsigned long* uidHeader=reinterpret_cast<unsigned long*>(manager[0x13c/4]);unsigned long* uidNode=reinterpret_cast<unsigned long*>(uidHeader[1]);
 CHECK(uidNode && uidNode[4]==uid && manager[0x140/4]==1);
 CThreadedFileManager::CFileObject& request=*reinterpret_cast<CThreadedFileManager::CFileObject*>(uidNode[5]);OVERLAPPED& ov=*reinterpret_cast<OVERLAPPED*>(request.Overlapped);
 CHECK(request.UID==uid && request.PManager==owner && request.PFile==reinterpret_cast<CThreadedFile*>(fileStorage));
 for(unsigned b=0;b<20;++b)CHECK(reinterpret_cast<unsigned char*>(&ov)[b]==0);
 g_request=&request;g_overlap=&ov;g_overlapFrees=g_requestDeletes=0;g_expectedOffset=offset;g_expectedPriority=priority;g_manager=manager;g_allow=true;g_begun=g_removed=g_finished=g_completionCalls=0;
 for(unsigned p=0;p<7;++p)manager[0xe4/4+p]=1-device;
 reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(manager)+0x100)[priority]=9.0;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[0]=0.25f;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[1]=0.5f;
 CHECK(manager[0x164/4] && reinterpret_cast<unsigned char*>(manager)[0x148]);
 g_finishEvent=CreateEventA(0,1,0,0);CHECK(g_finishEvent);CHECK(g_mainThread==GetCurrentThreadId() && g_workerThread && g_ownedWorker);
 const CThreadedFileManager& view=*owner;
 CHECK(view.ObjectExists(request.UID) && !view.IsObjectFinished(request.UID));
 LeaveCriticalSection(manager+0x14c/4);owner->KeepUpdating();
 DWORD started=GetTickCount();
 while(view.ObjectExists(uid)) {
  CHECK(GetTickCount()-started<5000);Sleep(1);
 }
 CHECK(!view.ObjectExists(uid) && view.IsObjectFinished(uid));
 CHECK(view.IsObjectFinished(0x7fffffffUL) && !view.ObjectExists(0x7fffffffUL));
 g_finished=1;
 DWORD hash=2166136261UL;
 {
  CHECK(g_overlapFrees==1 && g_requestDeletes==1 && manager[0x138/4]==0);
  unsigned q=1+(device*7+priority)*4;unsigned long* header=reinterpret_cast<unsigned long*>(manager[q]);
  CHECK(manager[q+1]==0 && !header[1] && header[2]==reinterpret_cast<DWORD>(header) && header[3]==reinterpret_cast<DWORD>(header) && manager[q+3]==reinterpret_cast<DWORD>(header));
  CHECK(GetThreadPriority(g_ownedWorker)==1);
  CHECK(!manager[0x140/4] && !uidHeader[1] && uidHeader[2]==reinterpret_cast<DWORD>(uidHeader) && uidHeader[3]==reinterpret_cast<DWORD>(uidHeader));
  unsigned transferred=offset>=0x9000 ? 0 : ((0x9000-offset)<length ? (0x9000-offset):length);
  for(unsigned i=0;i<transferred;++i) {CHECK(buffer[16+i]==(write ? (Payload(offset+i)^0x5a):Payload(offset+i)));hash=(hash^buffer[16+i])*16777619UL;}
  for(unsigned i=0;i<16;++i)CHECK(buffer[i]==0xcd);
  for(unsigned i=16+transferred;i<sizeof(buffer);++i)CHECK(buffer[i]==0xcd);
 }
 reinterpret_cast<CThreadedFileManager*>(manager)->Uninitialise();
 HANDLE thread=g_ownedWorker;CHECK(thread && WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);
 CHECK(g_workerThread && g_workerThread!=g_mainThread);
 CHECK(g_finished==1 && g_begun==2*unsigned(callback && !write));
 CHECK(g_completionCalls==unsigned(callback));
 CHECK(!reinterpret_cast<unsigned char*>(manager)[0x148] && reinterpret_cast<unsigned char*>(manager)[0x149] && reinterpret_cast<unsigned char*>(manager)[0x14a]);
 CHECK(manager[0x164/4]==0);
 CHECK(!manager[0x140/4] && !uidHeader[1] && g_overlapFrees==1 && g_requestDeletes==1);
 CHECK(installerObject->Paused==g_afterCycle && installerObject->Running==running && FixtureInstallerDirty==dirty);
 bool pauseSignal=running && afterPause<=1 && !dirty,unpauseSignal=running && afterPause==1;
 CHECK((WaitForSingleObject(installerEvent,0)==WAIT_OBJECT_0)==(pauseSignal || unpauseSignal));
 CHECK((WaitForSingleObject(pauseFinish,0)==WAIT_OBJECT_0)==unpauseSignal);
 CHECK(g_installer[(4+8)/4]==0);
 DeleteCriticalSection(installerObject->CriticalSection);
 CHECK(CloseHandle(installerEvent) && CloseHandle(pauseStart) && CloseHandle(pauseFinish));
 g_CFileInstallerSingleton_013CA818.object=0;g_installer=0;
 delete owner;CHECK(g_overlapFrees==1 && g_requestDeletes==1);
 CHECK(CloseHandle(thread) && CloseHandle(g_finishEvent));g_ownedWorker=0;g_finishEvent=0;g_request=0;g_manager=0;return hash;
}
int main(int argc,char** argv) {
 CHECK(argc==3 && !FixtureHardError);
 DWORD importProtection,restoredProtection;CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),4,&importProtection));
 g_realGlobalFree=FixtureGlobalFreeImport;FixtureGlobalFreeImport=ObserveGlobalFree;
 CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),importProtection,&restoredProtection));
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
 printf("NATIVE_WORKER_IMMEDIATE_RETIREMENT_PASS cases=%u reads=336 writes=336 joins=672 shutdowns=672 callbacks=336 immediate_deletions=672 overlap_frees=672\n",count);return 0;
}
