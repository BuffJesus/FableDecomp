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
static DWORD g_mainThread,g_workerThread;
static HANDLE g_ownedWorker;
static unsigned long* g_manager;
static bool g_shutdown;
struct Slot {
 void** vtable;
 CThreadedFileManager::CFileObject* request;
 void* overlap;
 unsigned uid,offset,length,priority,device,begun,completed,frees,deletes;
 bool write,callback,retained;
 unsigned long file[0x1c/4];
 unsigned char buffer[128];
};
static Slot g_slots[28];static unsigned g_count;
typedef void* (__stdcall* GlobalFreeFunction)(void*);
extern "C" GlobalFreeFunction FixtureGlobalFreeImport;
#pragma comment(linker,"/alternatename:_FixtureGlobalFreeImport=__imp__GlobalFree@4")
static GlobalFreeFunction g_realGlobalFree;
static void* __stdcall ObserveGlobalFree(void* block) {
 unsigned i=0;while(i<g_count && g_slots[i].overlap!=block)++i;CHECK(i<g_count);
 Slot& slot=g_slots[i];CHECK(GetCurrentThreadId()==g_workerThread);
 CHECK(++slot.frees==1 && !slot.deletes && g_shutdown==slot.retained);
 void* result=g_realGlobalFree(block);CHECK(result==0);return result;
}
void __cdecl operator delete(void* block) {
 for(unsigned i=0;i<g_count;++i)if(g_slots[i].request==block) {
  Slot& slot=g_slots[i];CHECK(GetCurrentThreadId()==g_workerThread && slot.frees==1);
  CHECK(++slot.deletes==1 && g_shutdown==slot.retained);break;
 }
 free(block);
}
static DWORD g_installerThread;
static HANDLE g_installerHandle;
static HANDLE g_waitRelease;static unsigned g_waitSleeps;static volatile DWORD g_waitStarted;static volatile unsigned g_waitUID;
static unsigned g_ackSignals,g_resumeSignals,g_pauseWaits,g_pauseReturns,g_idleReads,g_idleAlways;
void __cdecl FableInstallerWorker_009C5870(void*);
void CFileInstaller::DecompressionThreadMain() {Fail("cold decompression worker",__LINE__);}
void CFileInstaller::PostFileReadCompletion(CActiveRead*) {Fail("cold installer completion",__LINE__);}
void CFileInstaller::ProcessActiveFileForWrite(CActiveFile&) {Fail("cold installer write",__LINE__);}
void CFileInstaller::ProcessReads() {
 CHECK(GetCurrentThreadId()==g_installerThread && g_installer[(4+12)/4]==GetCurrentThreadId());++g_idleReads;
}
void CFileInstaller::ProcessAlways() {
 CHECK(GetCurrentThreadId()==g_installerThread && g_installer[(4+12)/4]==GetCurrentThreadId());++g_idleAlways;
}
static unsigned __stdcall InstallerLaunch(void* object) {
 g_installerThread=GetCurrentThreadId();FableInstallerWorker_009C5870(object);return 0;
}
typedef BOOL (__stdcall* SignalFunction)(HANDLE);
typedef DWORD (__stdcall* WaitFunction)(HANDLE,DWORD,BOOL);
typedef DWORD (__stdcall* SleepFunction)(DWORD,BOOL);
extern "C" SleepFunction FixtureSleepImport;
#pragma comment(linker,"/alternatename:_FixtureSleepImport=__imp__SleepEx@8")
static SleepFunction g_realSleep;
static DWORD __stdcall ObserveSleep(DWORD delay,BOOL alertable) {
 if(GetCurrentThreadId()==g_mainThread) {
 if(GetTickCount()-g_waitStarted>=5000) {
  fprintf(stderr,"WAIT_DIAGNOSTIC uid=%u ack=%u resume=%u waits=%u returns=%u paused=%lu manager_count=%lu current=%08lx sleep=%u\n",g_waitUID,g_ackSignals,g_resumeSignals,g_pauseWaits,g_pauseReturns,g_installer[0x7c/4],g_manager[0x140/4],g_manager[0x138/4],g_waitSleeps);
  for(unsigned i=0;i<g_count;++i)fprintf(stderr,"SLOT %u free=%u delete=%u begun=%u completed=%u status=%u priority=%u\n",i,g_slots[i].frees,g_slots[i].deletes,g_slots[i].begun,g_slots[i].completed,g_slots[i].deletes?99:unsigned(g_slots[i].request->LoadingStatus),g_slots[i].deletes?99:unsigned(g_slots[i].request->Desc.Priority));
 }
 CHECK(delay==0 && alertable && GetTickCount()-g_waitStarted<5000);++g_waitSleeps;CHECK(SetEvent(g_waitRelease));}
 return g_realSleep(delay,alertable);
}
extern "C" SignalFunction FixtureSetEventImport;
extern "C" WaitFunction FixtureWaitImport;
#pragma comment(linker,"/alternatename:_FixtureSetEventImport=__imp__SetEvent@4")
#pragma comment(linker,"/alternatename:_FixtureWaitImport=__imp__WaitForSingleObjectEx@12")
static SignalFunction g_realSignal;static WaitFunction g_realWait;
static BOOL __stdcall ObserveSignal(HANDLE event) {
 if(g_installer) {
  CFileInstaller* installer=reinterpret_cast<CFileInstaller*>(g_installer);
  if(event==installer->PauseStartEvent) {CHECK(GetCurrentThreadId()==g_installerThread);if(!g_ackSignals)CHECK(WaitForSingleObject(g_waitRelease,5000)==WAIT_OBJECT_0);++g_ackSignals;}
  if(event==installer->PauseFinishEvent) {CHECK(GetCurrentThreadId()==g_workerThread);++g_resumeSignals;}
 }
 return g_realSignal(event);
}
static DWORD __stdcall ObserveWait(HANDLE event,DWORD timeout,BOOL alertable) {
 bool pause=g_installer && event==reinterpret_cast<CFileInstaller*>(g_installer)->PauseStartEvent;
 if(pause) {CHECK(GetCurrentThreadId()==g_workerThread && timeout==0xffffffffUL && alertable);++g_pauseWaits;}
 DWORD result=g_realWait(event,timeout,alertable);
 if(pause) {CHECK(result==WAIT_OBJECT_0);++g_pauseReturns;}
 return result;
}
static void InstallObservers() {
 DWORD protection,restored;
 CHECK(VirtualProtect(&FixtureSleepImport,sizeof(FixtureSleepImport),4,&protection));
 g_realSleep=FixtureSleepImport;FixtureSleepImport=ObserveSleep;
 CHECK(VirtualProtect(&FixtureSleepImport,sizeof(FixtureSleepImport),protection,&restored));
 CHECK(VirtualProtect(&FixtureSetEventImport,sizeof(FixtureSetEventImport),4,&protection));
 g_realSignal=FixtureSetEventImport;FixtureSetEventImport=ObserveSignal;
 CHECK(VirtualProtect(&FixtureSetEventImport,sizeof(FixtureSetEventImport),protection,&restored));
 CHECK(VirtualProtect(&FixtureWaitImport,sizeof(FixtureWaitImport),4,&protection));
 g_realWait=FixtureWaitImport;FixtureWaitImport=ObserveWait;
 CHECK(VirtualProtect(&FixtureWaitImport,sizeof(FixtureWaitImport),protection,&restored));
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

static void __fastcall Complete(void* self,void*,bool success) {
 Slot& slot=*static_cast<Slot*>(self);
 CHECK(success && slot.callback && GetCurrentThreadId()==g_workerThread);
 CHECK(slot.request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_READ_COMPLETED);
 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId() && g_installer[(4+12)/4]!=GetCurrentThreadId() && g_installer[0x7c/4]==g_afterCycle);
 CHECK(++slot.completed==1 && !slot.frees && !slot.deletes);
 CHECK(slot.request->Desc.Priority==slot.priority || slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING);
 OVERLAPPED* live=reinterpret_cast<OVERLAPPED*>(slot.request->Overlapped);
 CHECK(live==slot.overlap && live->Offset==slot.offset && !live->OffsetHigh && live->hEvent==slot.request);
}
static bool __fastcall Begin(void* self,void*,void** dst,EThreadedFilePriority priority) {
 Slot& slot=*static_cast<Slot*>(self);
 CHECK(slot.callback && !slot.write && dst==&slot.request->Desc.PDst && (priority==slot.request->Desc.Priority || slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING));
 CHECK(slot.request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED);
 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId());
 EnterCriticalSection(g_manager+0x14c/4);++slot.begun;
 if(slot.begun==1) {
  slot.priority=(unsigned(slot.request->Desc.Priority)+1)%7;
  slot.request->PManager->SetObjectPriority(slot.uid,static_cast<EThreadedFilePriority>(slot.priority));
  CHECK(slot.request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED && slot.request->Desc.Priority==slot.priority);
  LeaveCriticalSection(g_manager+0x14c/4);return false;
 }
 CHECK(slot.begun==2 && (slot.request->Desc.Priority==slot.priority || slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING));
 LeaveCriticalSection(g_manager+0x14c/4);return true;
}
struct Node {unsigned char color,pad[3];Node* parent;Node* left;Node* right;unsigned key;CThreadedFileManager::CFileObject* value;};
static Node* g_seen[28];static Node* g_order[28];static unsigned g_seenCount,g_orderCount;
static unsigned Walk(Node* node,Node* parent) {
 if(!node)return 1;CHECK(g_seenCount<28);
 for(unsigned i=0;i<g_seenCount;++i)CHECK(g_seen[i]!=node);g_seen[g_seenCount++]=node;
 CHECK(node->parent==parent && node->color<=1);
 if(!node->color) {CHECK(!node->left || node->left->color==1);CHECK(!node->right || node->right->color==1);}
 unsigned left=Walk(node->left,node);g_order[g_orderCount++]=node;unsigned right=Walk(node->right,node);CHECK(left==right);return left+node->color;
}
static void ValidateUID(bool completed) {
 Node* header=reinterpret_cast<Node*>(g_manager[0x13c/4]);CHECK(header && !header->color);
 g_seenCount=g_orderCount=0;Walk(header->parent,header);unsigned index=0;
 for(unsigned i=0;i<g_count;++i)if(!completed || g_slots[i].retained) {
  CHECK(index<g_orderCount && g_order[index]->key==g_slots[i].uid && g_order[index]->value==g_slots[i].request);++index;
 }
 CHECK(index==g_seenCount && index==g_orderCount && index==g_manager[0x140/4]);
 if(index)CHECK(header->parent->color==1 && header->left==g_order[0] && header->right==g_order[index-1]);
 else CHECK(!header->parent && header->left==header && header->right==header);
}
static unsigned __stdcall Launch(void* context) {FixtureBeginThread(context);return 0;}
static unsigned char Payload(unsigned i) {return static_cast<unsigned char>(i*37+23);}
static void Run(HANDLE input,HANDLE output,unsigned variant,DWORD paused,bool running,bool dirty) {
 g_count=0;g_shutdown=false;g_installerThread=0;g_waitSleeps=0;g_waitRelease=CreateEventA(0,1,0,0);CHECK(g_waitRelease);g_ackSignals=g_resumeSignals=g_pauseWaits=g_pauseReturns=g_idleReads=g_idleAlways=0;memset(g_slots,0,sizeof(g_slots));
 unsigned long installer[0xa0/4];memset(installer,0,sizeof(installer));g_installer=installer;
 CFileInstaller* installerObject=reinterpret_cast<CFileInstaller*>(installer);
 CHECK(!paused && running && !dirty);installerObject->Running=false;installerObject->Paused=0;FixtureInstallerDirty=false;
 HANDLE installerEvent=CreateEventA(0,0,0,0),pauseStart=CreateEventA(0,0,0,0),pauseFinish=CreateEventA(0,0,0,0);
 CHECK(installerEvent && pauseStart && pauseFinish);
 installerObject->Event=installerEvent;installerObject->PauseStartEvent=pauseStart;installerObject->PauseFinishEvent=pauseFinish;
 InitializeCriticalSection(installerObject->CriticalSection);
 g_CFileInstallerSingleton_013CA818.object=installerObject;
 void* startHead[2];void* startNode[2];void* idleHead[2];
 startHead[0]=startHead[1]=startNode;startNode[0]=startNode[1]=startHead;idleHead[0]=idleHead[1]=idleHead;
 installer[0x38/4]=reinterpret_cast<DWORD>(startHead);installer[0x3c/4]=reinterpret_cast<DWORD>(idleHead);installer[0x68/4]=1;
 unsigned installerID;g_installerHandle=reinterpret_cast<HANDLE>(_beginthreadex(0,0,InstallerLaunch,installer,0,&installerID));CHECK(g_installerHandle);
 DWORD installerStarted=GetTickCount();
 for(;;) {
  EnterCriticalSection(installerObject->CriticalSection);bool ready=installerObject->Running;LeaveCriticalSection(installerObject->CriticalSection);
  if(ready)break;CHECK(GetTickCount()-installerStarted<5000);Sleep(1);
 }
 CHECK(g_installerThread==installerID && GetThreadPriority(g_installerHandle)==1);
 CHECK(WaitForSingleObject(pauseStart,0)==WAIT_TIMEOUT && WaitForSingleObject(pauseFinish,0)==WAIT_TIMEOUT);

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
 g_manager=manager;
 void* vtable[3]={0,reinterpret_cast<void*>(Begin),reinterpret_cast<void*>(Complete)};
 const CThreadedFileManager& view=*owner;
 for(unsigned n=0;n<28;++n) {
  Slot& slot=g_slots[n];slot.vtable=vtable;slot.priority=n%7;slot.device=(n/7)%2;slot.offset=n*64;slot.length=17+n%7;
  slot.write=((n+variant)%2)!=0;slot.callback=((n/2+variant)%2)!=0;slot.retained=((n/4+variant)%2)!=0;
  slot.file[1]=slot.device;slot.file[2]=reinterpret_cast<DWORD>(slot.write?output:input);slot.file[5]=n%3;
  reinterpret_cast<unsigned char*>(slot.file)[0x18]=slot.write;
  memset(slot.buffer,0xcd,sizeof(slot.buffer));
  if(slot.write)for(unsigned j=0;j<slot.length;++j)slot.buffer[16+j]=Payload(slot.offset+j)^0x5a;
  CThreadedFileObjectDesc desc;memset(&desc,0,sizeof(desc));
  desc.PDst=slot.buffer+16;desc.FilePos=slot.offset;desc.Size=slot.length;
  desc.Priority=static_cast<EThreadedFilePriority>(slot.callback && !slot.write?(slot.priority+1)%7:slot.priority);
  desc.QueueWhenFinished=slot.retained;desc.PCallback=slot.callback?reinterpret_cast<CThreadedFileCallback*>(&slot):0;
  unsigned uid=owner->AddObject(desc,*reinterpret_cast<CThreadedFile*>(slot.file));CHECK(uid==n+1);slot.uid=uid;
  Node* header=reinterpret_cast<Node*>(manager[0x13c/4]);Node* node=header->parent;
  while(node && node->key!=uid)node=uid<node->key?node->left:node->right;CHECK(node);
  slot.request=node->value;slot.overlap=slot.request->Overlapped;++g_count;
  CHECK(slot.request->UID==uid && slot.request->PManager==owner && slot.request->PFile==reinterpret_cast<CThreadedFile*>(slot.file));
  for(unsigned j=0;j<20;++j)CHECK(reinterpret_cast<unsigned char*>(slot.overlap)[j]==0);
  CHECK(view.ObjectExists(uid) && !view.IsObjectFinished(uid));ValidateUID(false);
 }
 for(unsigned p=0;p<7;++p)manager[0xe4/4+p]=0;
 for(unsigned p=0;p<7;++p)reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(manager)+0x100)[p]=9.0;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[0]=0.25f;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[1]=0.5f;
 CHECK(manager[0x164/4] && reinterpret_cast<unsigned char*>(manager)[0x148]);
 CHECK(g_mainThread==GetCurrentThreadId() && g_workerThread && g_ownedWorker);
 LeaveCriticalSection(manager+0x14c/4);
 g_waitStarted=GetTickCount();
 for(unsigned n=0;n<28;++n) {g_waitUID=g_slots[n].uid;owner->WaitForObjectToFinish(g_waitUID);}
 CHECK(g_waitSleeps>0);
 // Repeat every completed/missing UID and an unrelated UID; all must return.
 for(unsigned n=0;n<28;++n) {g_waitUID=g_slots[n].uid;owner->WaitForObjectToFinish(g_waitUID);}
 owner->WaitForObjectToFinish(0x7fffffffUL);
 EnterCriticalSection(manager+0x14c/4);ValidateUID(true);CHECK(!manager[0x138/4]);
 for(unsigned n=0;n<28;++n) {
  Slot& slot=g_slots[n];CHECK(view.ObjectExists(slot.uid)==slot.retained && view.IsObjectFinished(slot.uid));
  CHECK(slot.frees==unsigned(!slot.retained) && slot.deletes==unsigned(!slot.retained));
  CHECK(slot.completed==unsigned(slot.callback) && slot.begun==2*unsigned(slot.callback && !slot.write));
  if(slot.retained)CHECK(slot.request->LoadingStatus==4 && (slot.request->Desc.Priority==slot.priority || slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING));
  for(unsigned j=0;j<slot.length;++j)CHECK(slot.buffer[16+j]==(slot.write?(Payload(slot.offset+j)^0x5a):Payload(slot.offset+j)));
  for(unsigned j=0;j<16;++j)CHECK(slot.buffer[j]==0xcd);
  for(unsigned j=16+slot.length;j<sizeof(slot.buffer);++j)CHECK(slot.buffer[j]==0xcd);
 }
 for(unsigned q=0;q<14;++q) {
  unsigned at=1+q*4;Node* header=reinterpret_cast<Node*>(manager[at]);
  CHECK(!manager[at+1] && !header->parent && header->left==header && header->right==header && manager[at+3]==reinterpret_cast<DWORD>(header));
 }
 CHECK(view.IsObjectFinished(0x7fffffffUL) && !view.ObjectExists(0x7fffffffUL));
 CHECK(GetThreadPriority(g_ownedWorker)==1);g_shutdown=true;
 LeaveCriticalSection(manager+0x14c/4);
 reinterpret_cast<CThreadedFileManager*>(manager)->Uninitialise();
 HANDLE thread=g_ownedWorker;CHECK(thread && WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);
 CHECK(g_workerThread && g_workerThread!=g_mainThread);
 for(unsigned n=0;n<28;++n)CHECK(g_slots[n].frees==1 && g_slots[n].deletes==1);
 CHECK(!reinterpret_cast<unsigned char*>(manager)[0x148] && reinterpret_cast<unsigned char*>(manager)[0x149] && reinterpret_cast<unsigned char*>(manager)[0x14a]);
 CHECK(manager[0x164/4]==0);
 ValidateUID(true);
 CHECK(installerObject->Paused==0 && installerObject->Running && !FixtureInstallerDirty);
 CHECK(g_ackSignals==28 && g_resumeSignals==28 && g_pauseWaits==28 && g_pauseReturns==28);
 CHECK(g_idleReads && g_idleAlways);
 EnterCriticalSection(installerObject->CriticalSection);reinterpret_cast<unsigned char*>(installer)[0x7a]=1;LeaveCriticalSection(installerObject->CriticalSection);
 CHECK(SetEvent(installerEvent) && WaitForSingleObject(g_installerHandle,5000)==WAIT_OBJECT_0);
 CHECK(reinterpret_cast<unsigned char*>(installer)[0x78]==1 && !installerObject->Running && installerObject->Paused==0);
 CHECK(CloseHandle(g_installerHandle));g_installerHandle=0;
 DeleteCriticalSection(installerObject->CriticalSection);
 CHECK(CloseHandle(installerEvent) && CloseHandle(pauseStart) && CloseHandle(pauseFinish));
 g_CFileInstallerSingleton_013CA818.object=0;g_installer=0;
 delete owner;for(unsigned n=0;n<28;++n)CHECK(g_slots[n].frees==1 && g_slots[n].deletes==1);
 CHECK(CloseHandle(thread) && CloseHandle(g_waitRelease));g_waitRelease=0;g_ownedWorker=0;g_manager=0;g_count=0;
}
int main(int argc,char** argv) {
 CHECK(argc==3 && !FixtureHardError);InstallObservers();
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
 for(unsigned repetition=0;repetition<12;++repetition)for(unsigned variant=0;variant<2;++variant) {
  Run(input,output,variant,0,true,false);++count;
 }
 CHECK(CloseHandle(input) && CloseHandle(output));
 CHECK(g_FableTimerInitialised_013CAA10 && g_FableHighResolutionTimer_013CAA00 && g_FableTimerFrequency_013CAA08>0);
 CHECK(GFGetTime()>0.0);
 printf("NATIVE_WORKER_BLOCKING_WAIT_PASS batches=%u requests=672 reads=336 writes=336 joins=24 shutdowns=24 callbacks=336 immediate_deletions=336 exit_objects=336 overlap_frees=672 installer_joins=24 acknowledgments=672 pause_waits=672 resumes=672 wait_calls=1368 blocking_batches=24\n",count);return 0;
}
