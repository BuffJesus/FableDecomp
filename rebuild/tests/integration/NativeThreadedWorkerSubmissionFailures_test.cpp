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
#pragma section(".livemgr",read,write)
extern "C" __declspec(allocate(".livemgr")) __declspec(align(4096)) unsigned char FixtureGlobalManagerStorage[4096]={0};
#pragma comment(linker,"/alternatename:?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A=_FixtureGlobalManagerStorage")
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
static bool g_shutdown,g_managerAllocated;
void* __cdecl operator new(size_t size) {
 if(size==sizeof(CThreadedFileManager)) {
  CHECK(!g_managerAllocated);g_managerAllocated=true;
  memset(FixtureGlobalManagerStorage,0xcd,sizeof(FixtureGlobalManagerStorage));return FixtureGlobalManagerStorage;
 }
 void* block=malloc(size);CHECK(block);return block;
}
struct Slot {
 void** vtable;
 CThreadedFileManager::CFileObject* request;
 void* overlap;
 unsigned uid,offset,length,priority,device,begun,completed,frees,deletes;
 bool write,callback,retained,removing,userRemoved;
 unsigned long file[0x1c/4];
 unsigned char buffer[128];
};
static Slot g_slots[28];static unsigned g_count,g_protectedCalls;
typedef void* (__stdcall* GlobalFreeFunction)(void*);
extern "C" GlobalFreeFunction FixtureGlobalFreeImport;
#pragma comment(linker,"/alternatename:_FixtureGlobalFreeImport=__imp__GlobalFree@4")
static GlobalFreeFunction g_realGlobalFree;
static void* __stdcall ObserveGlobalFree(void* block) {
 unsigned i=0;while(i<g_count && g_slots[i].overlap!=block)++i;CHECK(i<g_count);
 Slot& slot=g_slots[i];CHECK(GetCurrentThreadId()==g_workerThread);
 CHECK(++slot.frees==1 && !slot.deletes && g_shutdown);
 void* result=g_realGlobalFree(block);CHECK(result==0);return result;
}
void __cdecl operator delete(void* block) {
 if(block==FixtureGlobalManagerStorage) {
  CHECK(g_managerAllocated && g_shutdown);g_managerAllocated=false;
  for(unsigned j=sizeof(CThreadedFileManager);j<sizeof(FixtureGlobalManagerStorage);++j)CHECK(FixtureGlobalManagerStorage[j]==0xcd);
  return;
 }
 for(unsigned i=0;i<g_count;++i)if(g_slots[i].request==block) {
  Slot& slot=g_slots[i];CHECK(GetCurrentThreadId()==g_workerThread && slot.frees==1);
  CHECK(++slot.deletes==1 && g_shutdown);break;
 }
 free(block);
}
static DWORD g_installerThread;
static HANDLE g_installerHandle;
static HANDLE g_policyEvent;
static unsigned g_failedSlot,g_failedError,g_hardErrors,g_policyObserved;
static bool g_failedSubmission,g_failedWrite,g_fixtureRecovery;
static unsigned g_totalFailedReads,g_totalFailedWrites;
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
extern "C" SignalFunction FixtureSetEventImport;
extern "C" WaitFunction FixtureWaitImport;
#pragma comment(linker,"/alternatename:_FixtureSetEventImport=__imp__SetEvent@4")
#pragma comment(linker,"/alternatename:_FixtureWaitImport=__imp__WaitForSingleObjectEx@12")
static SignalFunction g_realSignal;static WaitFunction g_realWait;
static BOOL __stdcall ObserveSignal(HANDLE event) {
 if(g_installer) {
  CFileInstaller* installer=reinterpret_cast<CFileInstaller*>(g_installer);
  if(event==installer->PauseStartEvent) {CHECK(GetCurrentThreadId()==g_installerThread);++g_ackSignals;}
  if(event==installer->PauseFinishEvent) {CHECK(GetCurrentThreadId()==g_workerThread || (g_fixtureRecovery && GetCurrentThreadId()==g_mainThread));++g_resumeSignals;}
 }
 return g_realSignal(event);
}
static DWORD __stdcall ObserveWait(HANDLE event,DWORD timeout,BOOL alertable) {
 bool pause=g_installer && event==reinterpret_cast<CFileInstaller*>(g_installer)->PauseStartEvent;
 if(pause) {CHECK(GetCurrentThreadId()==g_workerThread && timeout==0xffffffffUL && alertable);++g_pauseWaits;}
 if(g_manager && event==reinterpret_cast<HANDLE>(g_manager[0x164/4]) && GetCurrentThreadId()==g_workerThread && g_failedSubmission && !g_policyObserved) {
  CHECK(timeout==0xffffffffUL && alertable);++g_policyObserved;CHECK(SetEvent(g_policyEvent));
 }
 DWORD result=g_realWait(event,timeout,alertable);
 if(pause) {CHECK(result==WAIT_OBJECT_0);++g_pauseReturns;}
 return result;
}
typedef BOOL (__stdcall* ReadFunction)(HANDLE,void*,DWORD,OVERLAPPED*,IOCompletion);
typedef BOOL (__stdcall* WriteFunction)(HANDLE,const void*,DWORD,OVERLAPPED*,IOCompletion);
extern "C" ReadFunction FixtureReadImport;
extern "C" WriteFunction FixtureWriteImport;
#pragma comment(linker,"/alternatename:_FixtureReadImport=__imp__ReadFileEx@20")
#pragma comment(linker,"/alternatename:_FixtureWriteImport=__imp__WriteFileEx@20")
static ReadFunction g_realRead;static WriteFunction g_realWrite;
static void RecordFailure(HANDLE file,OVERLAPPED* overlap,bool write) {
 CHECK(file==INVALID_HANDLE_VALUE && GetCurrentThreadId()==g_workerThread && !g_failedSubmission);
 unsigned error=GetLastError();CHECK(error==6);
 unsigned i=0;while(i<g_count && g_slots[i].overlap!=overlap)++i;CHECK(i<g_count);
 CHECK(overlap->hEvent==g_slots[i].request && g_slots[i].request->LoadingStatus==2 && g_slots[i].write==write);
 g_failedSlot=i;g_failedWrite=write;g_failedError=error;g_failedSubmission=true;
 if(write)++g_totalFailedWrites;else ++g_totalFailedReads;SetLastError(error);
}
static BOOL __stdcall ObserveRead(HANDLE file,void* buffer,DWORD bytes,OVERLAPPED* overlap,IOCompletion callback) {
 BOOL result=g_realRead(file,buffer,bytes,overlap,callback);CHECK(!result);RecordFailure(file,overlap,false);return result;
}
static BOOL __stdcall ObserveWrite(HANDLE file,const void* buffer,DWORD bytes,OVERLAPPED* overlap,IOCompletion callback) {
 BOOL result=g_realWrite(file,buffer,bytes,overlap,callback);CHECK(!result);RecordFailure(file,overlap,true);return result;
}
static void __fastcall ObserveHardError(const char* text) {
 CHECK(GetCurrentThreadId()==g_workerThread && g_failedSubmission && g_failedWrite && text && !*text && ++g_hardErrors==1);
}
static void InstallObservers() {
 DWORD protection,restored;
 CHECK(VirtualProtect(&FixtureSetEventImport,sizeof(FixtureSetEventImport),4,&protection));
 g_realSignal=FixtureSetEventImport;FixtureSetEventImport=ObserveSignal;
 CHECK(VirtualProtect(&FixtureSetEventImport,sizeof(FixtureSetEventImport),protection,&restored));
 CHECK(VirtualProtect(&FixtureWaitImport,sizeof(FixtureWaitImport),4,&protection));
 g_realWait=FixtureWaitImport;FixtureWaitImport=ObserveWait;
 CHECK(VirtualProtect(&FixtureWaitImport,sizeof(FixtureWaitImport),protection,&restored));
 CHECK(VirtualProtect(&FixtureReadImport,sizeof(FixtureReadImport),4,&protection));
 g_realRead=FixtureReadImport;FixtureReadImport=ObserveRead;
 CHECK(VirtualProtect(&FixtureReadImport,sizeof(FixtureReadImport),protection,&restored));
 CHECK(VirtualProtect(&FixtureWriteImport,sizeof(FixtureWriteImport),4,&protection));
 g_realWrite=FixtureWriteImport;FixtureWriteImport=ObserveWrite;
 CHECK(VirtualProtect(&FixtureWriteImport,sizeof(FixtureWriteImport),protection,&restored));
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

static void __fastcall Complete(void*,void*,bool) {Fail("unexpected completion after failed submission",__LINE__);}
static bool __fastcall Begin(void*,void*,void**,EThreadedFilePriority) {Fail("unexpected selected callback",__LINE__);return false;}
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
 for(unsigned i=0;i<g_count;++i)if(!completed || (g_slots[i].retained && !g_slots[i].userRemoved)) {
  CHECK(index<g_orderCount && g_order[index]->key==g_slots[i].uid && g_order[index]->value==g_slots[i].request);++index;
 }
 CHECK(index==g_seenCount && index==g_orderCount && index==g_manager[0x140/4]);
 if(index)CHECK(header->parent->color==1 && header->left==g_order[0] && header->right==g_order[index-1]);
 else CHECK(!header->parent && header->left==header && header->right==header);
}
static unsigned g_statisticsChecks;
static void ValidateStatistics(unsigned pending,unsigned pendingBytes,unsigned finished,unsigned finishedBytes) {
 const CThreadedFileManager& manager=*reinterpret_cast<CThreadedFileManager*>(g_manager);
 CHECK(manager.GetTotalObjectsPendingCount()==long(pending));
 CHECK(manager.GetTotalBytesPendingCount()==long(pendingBytes));
 CHECK(manager.GetTotalObjectsFinishedCount()==long(finished));
 CHECK(manager.GetTotalBytesFinishedCount()==long(finishedBytes));++g_statisticsChecks;
}
static void ValidateFinishedStatistics() {
 unsigned count=0,bytes=0;
 for(unsigned i=0;i<g_count;++i)if(g_slots[i].retained && !g_slots[i].userRemoved) {++count;bytes+=g_slots[i].length;}
 ValidateStatistics(0,0,count,bytes);
}
static unsigned __stdcall Launch(void* context) {FixtureBeginThread(context);return 0;}
static unsigned char Payload(unsigned i) {return static_cast<unsigned char>(i*37+23);}
static void Run(HANDLE input,HANDLE output,unsigned variant,DWORD paused,bool running,bool dirty) {
 g_count=0;g_protectedCalls=0;g_shutdown=false;g_installerThread=0;
 g_failedSubmission=g_failedWrite=g_fixtureRecovery=false;g_failedError=g_hardErrors=g_policyObserved=0;
 g_policyEvent=CreateEventA(0,1,0,0);CHECK(g_policyEvent);
 g_ackSignals=g_resumeSignals=g_pauseWaits=g_pauseReturns=g_idleReads=g_idleAlways=0;memset(g_slots,0,sizeof(g_slots));
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
 CThreadedFileManager* owner=new CThreadedFileManager;CHECK(owner==reinterpret_cast<CThreadedFileManager*>(FixtureGlobalManagerStorage) && owner==&g_FableThreadedFileManager_013BC870);CaptureWorker(previousThreads,previousCount);unsigned long* manager=reinterpret_cast<unsigned long*>(owner);
 EnterCriticalSection(manager+0x14c/4);
 for(unsigned i=0;i<15;++i) {
  unsigned at=i==14 ? 0x13c/4 : 1+i*4;unsigned long* header=reinterpret_cast<unsigned long*>(manager[at]);
  CHECK(header && !manager[at+1] && !header[1] && header[2]==reinterpret_cast<DWORD>(header) && header[3]==reinterpret_cast<DWORD>(header));
  if(i<14)CHECK(manager[at+3]==reinterpret_cast<DWORD>(header));
  for(unsigned j=0;j<i;++j)CHECK(manager[j==14 ? 0x13c/4 : 1+j*4]!=manager[at]);
 }
 CHECK(!manager[0x138/4] && manager[0x168/4]==1 && !manager[0x16c/4]);
 g_manager=manager;ValidateStatistics(0,0,0,0);
 void* vtable[3]={0,reinterpret_cast<void*>(Begin),reinterpret_cast<void*>(Complete)};
 const CThreadedFileManager& view=*owner;unsigned submittedBytes=0;
 for(unsigned n=0;n<28;++n) {
  Slot& slot=g_slots[n];slot.vtable=vtable;slot.priority=n%7;slot.device=(n/7)%2;slot.offset=n*64;slot.length=17+n%7;
  slot.write=((n+variant)%2)!=0;slot.callback=((n/2+variant)%2)!=0;slot.retained=((n/4+variant)%2)!=0;
  slot.file[1]=slot.device;slot.file[2]=reinterpret_cast<DWORD>(INVALID_HANDLE_VALUE);slot.file[5]=n%3;
  reinterpret_cast<unsigned char*>(slot.file)[0x18]=slot.write;
  memset(slot.buffer,0xcd,sizeof(slot.buffer));
  if(slot.write)for(unsigned j=0;j<slot.length;++j)slot.buffer[16+j]=Payload(slot.offset+j)^0x5a;
  CThreadedFileObjectDesc desc;memset(&desc,0,sizeof(desc));
  desc.PDst=slot.buffer+16;desc.FilePos=slot.offset;desc.Size=slot.length;
  desc.Priority=static_cast<EThreadedFilePriority>(slot.callback && !slot.write?(slot.priority+1)%7:slot.priority);
  desc.QueueWhenFinished=slot.retained;desc.PCallback=slot.callback?reinterpret_cast<CThreadedFileCallback*>(&slot):0;
  unsigned uid=reinterpret_cast<CThreadedFile*>(slot.file)->AddObject(desc);CHECK(uid==n+1);slot.uid=uid;
  Node* header=reinterpret_cast<Node*>(manager[0x13c/4]);Node* node=header->parent;
  while(node && node->key!=uid)node=uid<node->key?node->left:node->right;CHECK(node);
  slot.request=node->value;slot.overlap=slot.request->Overlapped;++g_count;
  CHECK(slot.request->UID==uid && slot.request->PManager==owner && slot.request->PFile==reinterpret_cast<CThreadedFile*>(slot.file));
  for(unsigned j=0;j<20;++j)CHECK(reinterpret_cast<unsigned char*>(slot.overlap)[j]==0);
  CHECK(reinterpret_cast<CThreadedFile*>(slot.file)->ObjectExists(uid) && !reinterpret_cast<CThreadedFile*>(slot.file)->IsObjectFinished(uid));ValidateUID(false);submittedBytes+=slot.length;ValidateStatistics(n+1,submittedBytes,0,0);
 }
 for(unsigned p=0;p<7;++p)manager[0xe4/4+p]=0;
 double deadline=GFGetTime()+3600.0;
 for(unsigned p=0;p<7;++p)reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(manager)+0x100)[p]=deadline;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[0]=0.25f;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[1]=0.5f;
 LeaveCriticalSection(manager+0x14c/4);
 CHECK(WaitForSingleObject(g_policyEvent,5000)==WAIT_OBJECT_0);
 EnterCriticalSection(manager+0x14c/4);ValidateUID(false);ValidateStatistics(28,submittedBytes,0,0);
 CHECK(g_failedSubmission && g_failedError==6 && g_policyObserved==1);
 CHECK(g_failedSlot==(variant?6:20) && g_failedWrite==(variant!=0));
 CHECK(g_hardErrors==unsigned(g_failedWrite));
 CHECK(g_ackSignals==1 && g_pauseWaits==1 && g_pauseReturns==1 && g_resumeSignals==unsigned(g_failedWrite));
 CHECK(installerObject->Paused==unsigned(!g_failedWrite));
 CHECK(manager[0x138/4]==reinterpret_cast<DWORD>(g_slots[g_failedSlot].request));
 unsigned pending=0;for(unsigned q=0;q<14;++q)pending+=manager[1+q*4+1];CHECK(pending==27);
 for(unsigned n=0;n<28;++n) {
  Slot& slot=g_slots[n];CHECK(!slot.frees && !slot.deletes && !slot.begun && !slot.completed);
  CHECK(slot.request->LoadingStatus==(n==g_failedSlot?2:0));
  CHECK(reinterpret_cast<CThreadedFile*>(slot.file)->ObjectExists(slot.uid) && !reinterpret_cast<CThreadedFile*>(slot.file)->IsObjectFinished(slot.uid));
  for(unsigned j=0;j<sizeof(slot.buffer);++j) {
   unsigned char wanted=slot.write && j>=16 && j<16+slot.length?static_cast<unsigned char>(Payload(slot.offset+j-16)^0x5a):0xcd;
   CHECK(slot.buffer[j]==wanted);
  }
 }
 g_shutdown=true;LeaveCriticalSection(manager+0x14c/4);owner->Uninitialise();
 HANDLE thread=g_ownedWorker;CHECK(WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);
 for(unsigned n=0;n<28;++n)CHECK(g_slots[n].frees==1 && g_slots[n].deletes==1);
 ValidateUID(false);CHECK(manager[0x140/4]==28 && !manager[0x164/4]);
 // Only fixture cleanup repairs the unmatched pause after read submission failure.
 g_fixtureRecovery=true;if(!g_failedWrite)installerObject->Unpause();g_fixtureRecovery=false;
 CHECK(installerObject->Paused==0 && g_resumeSignals==1);
 EnterCriticalSection(installerObject->CriticalSection);reinterpret_cast<unsigned char*>(installer)[0x7a]=1;LeaveCriticalSection(installerObject->CriticalSection);
 CHECK(SetEvent(installerEvent) && WaitForSingleObject(g_installerHandle,5000)==WAIT_OBJECT_0);
 CHECK(reinterpret_cast<unsigned char*>(installer)[0x78]==1 && !installerObject->Running);
 CHECK(CloseHandle(g_installerHandle));g_installerHandle=0;
 DeleteCriticalSection(installerObject->CriticalSection);
 CHECK(CloseHandle(installerEvent) && CloseHandle(pauseStart) && CloseHandle(pauseFinish));
 g_CFileInstallerSingleton_013CA818.object=0;g_installer=0;
 delete owner;CHECK(!g_managerAllocated);
 for(unsigned n=0;n<28;++n)CHECK(g_slots[n].frees==1 && g_slots[n].deletes==1);
 CHECK(CloseHandle(thread) && CloseHandle(g_policyEvent));g_policyEvent=0;g_ownedWorker=0;g_manager=0;g_count=0;
}
int main(int argc,char** argv) {
 CHECK(argc==3 && !FixtureHardError);InstallObservers();FixtureHardError=ObserveHardError;
 DWORD importProtection,restoredProtection;CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),4,&importProtection));
 g_realGlobalFree=FixtureGlobalFreeImport;FixtureGlobalFreeImport=ObserveGlobalFree;
 CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),importProtection,&restoredProtection));
 // Actual manager allocation uses the global storage addressed by file wrappers.
 CHECK(!g_managerAllocated);
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
 CHECK(g_statisticsChecks==720 && g_totalFailedReads==12 && g_totalFailedWrites==12);
 printf("NATIVE_WORKER_SUBMISSION_FAILURES_PASS batches=%u requests=672 failed_reads=12 failed_writes=12 no_completions=24 shutdowns=24 exit_objects=672 overlap_frees=672 installer_joins=24 acknowledgments=24 write_error_callbacks=12 read_cleanup_unpauses=12 statistics_checks=720\n",count);return 0;
}
