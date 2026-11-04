// Typed bank priority moves after refused begin, worker retry and owner teardown.
// Actual bank begin/read/APC/completion progress after two native teardown retries.
// Two event-controlled native retries; nonzero UID, caller buffer and cached metadata.
#include "fable_threaded_file_object.h"
#include "fable_file_installer.h"
#include "fable_bank_async_entry.h"
#include "NativeThreadedReadAPC_win32.h"
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C" {
__declspec(dllimport) BOOL __stdcall ResetEvent(HANDLE);
void* __fastcall FixtureBankBeginRead(CBankFileAsyncEntry*,void*,void*,unsigned long,void*,unsigned long,unsigned long,EThreadedFilePriority);
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
#pragma comment(linker,"/alternatename:@FixtureBankBeginRead@32=?BeginReadData@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KPAXKKW4EThreadedFilePriority@@@Z")
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
static void* AllocateBankStorage(size_t);
static bool DeleteBankStorage(void*);
void* __cdecl operator new(size_t size) {
 if(size==sizeof(CThreadedFileManager)) {
  CHECK(!g_managerAllocated);g_managerAllocated=true;
  memset(FixtureGlobalManagerStorage,0xcd,sizeof(FixtureGlobalManagerStorage));return FixtureGlobalManagerStorage;
 }
 if(size==72 || size==12)return AllocateBankStorage(size);
 void* block=malloc(size);CHECK(block);return block;
}
struct BankContext {
 bool initialized;
 unsigned long bankWords[(sizeof(CBankFileAsync)+32)/4],entryWords[12],bankHeader[5],restartHeader[5],runtimeEntry[3],file[7];
 unsigned constructed,reads,completed,released,enters,leaves,tries;
};
static BankContext g_banks[2];
static unsigned g_sharedBatches,g_sharedContexts,g_sharedTreeChecks,g_sharedPartialDeletes,g_sharedTwoChildDeletes,g_releaseOrders[4];
struct Slot {
 void** vtable;
 BankContext* bankCtx;unsigned long ownerWords[2];
 void* bankDataBlock;void* bankInfoBlock;void* bankNodeBlock;bool bankDataDeleted,bankInfoDeleted,bankNodeDeleted;
 unsigned char bankExpected[72];
 CThreadedFileManager::CFileObject* request;
 void* overlap;
 unsigned uid,offset,length,priority,device,begun,completed,frees,deletes,apcCompletions;
 bool write,callback,retained,removing,userRemoved,cancelling,cancelled,probing,child;
 unsigned long file[0x1c/4];
 unsigned char buffer[128];
};
static Slot g_slots[56];static unsigned g_count,g_protectedCalls;
static Slot* g_constructingBank;static Slot* g_releasingBank;
static HANDLE g_readIssued,g_continueRead,g_publishedEvent,g_requestRetired;
static HANDLE g_refusalReady,g_continueRetry;static unsigned g_beginRefusals;
static volatile bool g_published;static unsigned g_retries;static DWORD g_releaseStarted;
typedef void (__stdcall* PlainSleep)(DWORD);
extern "C" PlainSleep FixturePlainSleep;
#pragma comment(linker,"/alternatename:_FixturePlainSleep=__imp__Sleep@4")
static PlainSleep g_realPlainSleep;
static void __stdcall ObservePlainSleep(DWORD delay) {
 if(g_releasingBank && GetCurrentThreadId()==g_mainThread) {
  CHECK(delay==0 && GetTickCount()-g_releaseStarted<5000 && ++g_retries<=2);
  CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(g_releasingBank->bankCtx->bankWords+4);
  CHECK(!*reinterpret_cast<DWORD*>(bank->CriticalSection+8) && !*reinterpret_cast<DWORD*>(bank->CriticalSection+12));
  CHECK(g_releasingBank->bankNodeDeleted && !g_releasingBank->bankDataDeleted && !g_releasingBank->bankInfoDeleted);
  CHECK(g_releasingBank->request->LoadingStatus==1 && !g_releasingBank->frees && !g_releasingBank->deletes);
  if(g_retries==2) {CHECK(SetEvent(g_continueRead));CHECK(WaitForSingleObject(g_publishedEvent,5000)==WAIT_OBJECT_0);CHECK(g_published);}
 }
 g_realPlainSleep(delay);
}
static unsigned g_nativeBankCompletions,g_nativeBankDataDeletes,g_nativeBankInfoDeletes,g_nativeBankNodeDeletes;
static unsigned g_startUID,g_childSubmissions,g_requestReuses,g_overlapReuses,g_successAPCs,g_successBytes;
static unsigned g_zeroSubmissions,g_zeroPriorityMoves,g_zeroImmediateDeletes,g_zeroCallerDeletes,g_zeroExitDeletes;
typedef void* (__stdcall* GlobalFreeFunction)(void*);
extern "C" GlobalFreeFunction FixtureGlobalFreeImport;
#pragma comment(linker,"/alternatename:_FixtureGlobalFreeImport=__imp__GlobalFree@4")
static GlobalFreeFunction g_realGlobalFree;
static void* __stdcall ObserveGlobalFree(void* block) {
 unsigned i=0;while(i<g_count && (g_slots[i].overlap!=block || g_slots[i].frees))++i;CHECK(i<g_count);
 Slot& slot=g_slots[i];CHECK(GetCurrentThreadId()==(slot.removing?g_mainThread:g_workerThread));
 CHECK(!slot.probing && ++slot.frees==1 && !slot.deletes && (slot.removing?(!g_shutdown && (slot.retained || slot.cancelling)):g_shutdown==slot.retained));
 void* result=g_realGlobalFree(block);CHECK(result==0);return result;
}
void __cdecl operator delete(void* block) {
 if(DeleteBankStorage(block))return;
 if(block==FixtureGlobalManagerStorage) {
  CHECK(g_managerAllocated && g_shutdown);g_managerAllocated=false;
  for(unsigned j=sizeof(CThreadedFileManager);j<sizeof(FixtureGlobalManagerStorage);++j)CHECK(FixtureGlobalManagerStorage[j]==0xcd);
  return;
 }
 for(unsigned i=0;i<g_count;++i)if(g_slots[i].request==block && !g_slots[i].deletes) {
  Slot& slot=g_slots[i];CHECK(GetCurrentThreadId()==(slot.removing?g_mainThread:g_workerThread) && slot.frees==1);
  CHECK(!slot.probing && ++slot.deletes==1 && (slot.removing?(!g_shutdown && (slot.retained || slot.cancelling)):g_shutdown==slot.retained));
  if(slot.child) {
   CHECK(g_published && slot.apcCompletions==1);++slot.bankCtx->completed;CHECK(SetEvent(g_requestRetired));
   ++g_nativeBankCompletions;
  }
  if(!slot.uid) {if(slot.removing)++g_zeroCallerDeletes;else if(g_shutdown)++g_zeroExitDeletes;else ++g_zeroImmediateDeletes;}break;
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
 if(GetCurrentThreadId()==g_workerThread && g_beginRefusals && !g_releasingBank) {
  Slot& slot=g_slots[28];CHECK(g_beginRefusals==1 && delay==1 && alertable);
  CHECK(slot.request->LoadingStatus==0 && !slot.request->Desc.PDst && !slot.apcCompletions);
  CHECK(!slot.frees && !slot.deletes && !slot.bankNodeDeleted && !slot.bankDataDeleted && !slot.bankInfoDeleted);
  CHECK(!g_manager[0x138/4] && g_manager[(0x14c+12)/4]!=g_workerThread);
  CHECK(SetEvent(g_refusalReady));CHECK(WaitForSingleObject(g_continueRetry,5000)==WAIT_OBJECT_0);
 }
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
static void InstallBankLockObservers();
static void InstallObservers() {
 InstallBankLockObservers();
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
extern "C" bool FixtureManagerDirty;
#pragma comment(linker,"/alternatename:_FixtureManagerDirty=?m_bDirtyDiscError@CThreadedFileManager@@0_NA")
static unsigned g_errorCallbacks,g_totalAPCErrors,g_totalHardErrors,g_readingRefusals,g_completedRefusals,g_pendingChecks;
typedef BOOL (__stdcall* ReadFunction)(HANDLE,void*,DWORD,OVERLAPPED*,IOCompletion);
extern "C" ReadFunction FixtureReadImport;
#pragma comment(linker,"/alternatename:_FixtureReadImport=__imp__ReadFileEx@20")
static ReadFunction g_realRead;static IOCompletion g_realCompletion;
static void __fastcall ObserveHardError(const char* text) {
 CHECK(text && !*text && GetCurrentThreadId()==g_workerThread);
 CThreadedFileManager::CFileObject* object=reinterpret_cast<CThreadedFileManager::CFileObject*>(g_manager[0x138/4]);
 CHECK(object && object->LoadingStatus==2 && object->PFile->deviceId_==0);++g_errorCallbacks;++g_totalHardErrors;
}
static void __stdcall ObserveAPC(DWORD error,DWORD transferred,OVERLAPPED* overlap) {
 CHECK(GetCurrentThreadId()==g_workerThread);
 unsigned i=0;while(i<g_count && (g_slots[i].overlap!=overlap || g_slots[i].frees))++i;CHECK(i<g_count);
 Slot& slot=g_slots[i];CHECK(error==(slot.child?0:38) && transferred==(slot.child?slot.length:0));CHECK(++slot.apcCompletions==1 && overlap->hEvent==slot.request && slot.request->LoadingStatus==2);
 unsigned char preserved[64];memcpy(preserved,slot.request,sizeof(preserved));slot.probing=true;
 CHECK(!reinterpret_cast<CThreadedFile*>(slot.file)->RemoveObject(slot.uid));
 CHECK(!slot.frees && !slot.deletes && memcmp(preserved,slot.request,sizeof(preserved))==0);slot.probing=false;++g_readingRefusals;
 bool dirty=FixtureManagerDirty;unsigned before=g_errorCallbacks;if(slot.child) {++g_successAPCs;g_successBytes+=transferred;}else ++g_totalAPCErrors;
 g_realCompletion(error,transferred,overlap);
 CHECK(slot.request->LoadingStatus==3 && !reinterpret_cast<CFileInstaller*>(g_installer)->Paused);
 CHECK(g_errorCallbacks==before+unsigned(!slot.child && slot.device==0));
 CHECK(FixtureManagerDirty==(!slot.child && slot.device==1?true:dirty));
}
static BOOL __stdcall ObserveRead(HANDLE file,void* buffer,DWORD bytes,OVERLAPPED* overlap,IOCompletion completion) {
 CHECK(completion && (!g_realCompletion || completion==g_realCompletion));g_realCompletion=completion;
 unsigned i=0;while(i<g_count && (g_slots[i].overlap!=overlap || g_slots[i].frees))++i;CHECK(i<g_count);
 if(g_slots[i].child) {BankContext& ctx=*g_slots[i].bankCtx;CHECK(g_releasingBank==&g_slots[i] && g_retries==2 && ctx.enters==4 && ctx.leaves==6 && ctx.tries==2 && !ctx.reads);++ctx.reads;}
 CHECK(buffer==g_slots[i].buffer+16 && bytes==g_slots[i].length && file==reinterpret_cast<HANDLE>(g_slots[i].file[2]));
 BOOL result=g_realRead(file,buffer,bytes,overlap,ObserveAPC);CHECK(result);
 return result;
}
static void InstallReadObserver() {
 DWORD protection,restored;
 CHECK(VirtualProtect(&FixtureReadImport,sizeof(FixtureReadImport),4,&protection));
 g_realRead=FixtureReadImport;FixtureReadImport=ObserveRead;
 CHECK(VirtualProtect(&FixtureReadImport,sizeof(FixtureReadImport),protection,&restored));
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

static void ReleaseBankOwner(Slot&);
static void PrepareSharedBank(Slot&);
static void ValidateBankTree(BankContext&);
static void ReleaseSharedOwners(unsigned);
static void FinishSharedBanks();
static void SubmitChild(Slot& parent);
static void __fastcall Complete(void* self,void*,bool success) {
 Slot& slot=*static_cast<Slot*>(self);
 CHECK(success && slot.callback && GetCurrentThreadId()==g_workerThread);
 CHECK(slot.request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_READ_COMPLETED);
 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId() && g_installer[(4+12)/4]!=GetCurrentThreadId() && g_installer[0x7c/4]==g_afterCycle);
 CHECK(++slot.completed==1 && !slot.frees && !slot.deletes && slot.apcCompletions==1);
 unsigned char preserved[64];memcpy(preserved,slot.request,sizeof(preserved));slot.probing=true;
 CHECK(!reinterpret_cast<CThreadedFile*>(slot.file)->RemoveObject(slot.uid));
 CHECK(!slot.frees && !slot.deletes && memcmp(preserved,slot.request,sizeof(preserved))==0);slot.probing=false;++g_completedRefusals;

 CHECK(slot.request->Desc.Priority==slot.priority || slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING);
 OVERLAPPED* live=reinterpret_cast<OVERLAPPED*>(slot.request->Overlapped);
 CHECK(live==slot.overlap && live->Offset==slot.offset && !live->OffsetHigh && live->hEvent==slot.request);
 unsigned char parentBytes[64];memcpy(parentBytes,slot.request,sizeof(parentBytes));SubmitChild(slot);
 CHECK(memcmp(parentBytes,slot.request,sizeof(parentBytes))==0);
}
static bool __fastcall Begin(void* self,void*,void** dst,EThreadedFilePriority priority) {
 Slot& slot=*static_cast<Slot*>(self);
 CHECK(slot.callback && !slot.write && dst==&slot.request->Desc.PDst && (priority==slot.request->Desc.Priority || slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING));
 CHECK(slot.request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED);
 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId());
 EnterCriticalSection(g_manager+0x14c/4);++slot.begun;
 if(slot.begun==1) {
  unsigned char before[64];memcpy(before,slot.request,sizeof(before));slot.probing=true;
  CHECK(!reinterpret_cast<CThreadedFile*>(slot.file)->RemoveObject(slot.uid));
  CHECK(!slot.frees && !slot.deletes && memcmp(before,slot.request,sizeof(before))==0);slot.probing=false;++g_protectedCalls;
  slot.priority=(unsigned(slot.request->Desc.Priority)+1)%7;
  reinterpret_cast<CThreadedFile*>(slot.file)->SetObjectPriority(slot.uid,static_cast<EThreadedFilePriority>(slot.priority));
  CHECK(slot.request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED && slot.request->Desc.Priority==slot.priority);
  LeaveCriticalSection(g_manager+0x14c/4);return false;
 }
 CHECK(slot.begun==2 && (slot.request->Desc.Priority==slot.priority || slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING));
 LeaveCriticalSection(g_manager+0x14c/4);return true;
}
struct Node {unsigned char color,pad[3];Node* parent;Node* left;Node* right;unsigned key;CThreadedFileManager::CFileObject* value;};
static Node* g_seen[56];static Node* g_order[56];static unsigned g_seenCount,g_orderCount;
static unsigned Walk(Node* node,Node* parent) {
 if(!node)return 1;CHECK(g_seenCount<56);
 for(unsigned i=0;i<g_seenCount;++i)CHECK(g_seen[i]!=node);g_seen[g_seenCount++]=node;
 CHECK(node->parent==parent && node->color<=1);
 if(!node->color) {CHECK(!node->left || node->left->color==1);CHECK(!node->right || node->right->color==1);}
 unsigned left=Walk(node->left,node);g_order[g_orderCount++]=node;unsigned right=Walk(node->right,node);CHECK(left==right);return left+node->color;
}
static void ValidateUID(bool completed) {
 Node* header=reinterpret_cast<Node*>(g_manager[0x13c/4]);CHECK(header && !header->color);
 g_seenCount=g_orderCount=0;Walk(header->parent,header);unsigned expected[56],index=0;
 for(unsigned i=0;i<g_count;++i)if(!g_slots[i].cancelled && (!completed || (g_slots[i].retained && !g_slots[i].userRemoved)) && (!g_slots[i].deletes || (g_shutdown && g_slots[i].retained)))expected[index++]=i;
 for(unsigned i=1;i<index;++i) {unsigned value=expected[i],j=i;while(j && g_slots[value].uid<g_slots[expected[j-1]].uid) {expected[j]=expected[j-1];--j;}expected[j]=value;}
 CHECK(index==g_seenCount && index==g_orderCount && index==g_manager[0x140/4]);
 for(unsigned i=0;i<index;++i)CHECK(g_order[i]->key==g_slots[expected[i]].uid && g_order[i]->value==g_slots[expected[i]].request);
 if(index)CHECK(header->parent->color==1 && header->left==g_order[0] && header->right==g_order[index-1]);
 else CHECK(!header->parent && header->left==header && header->right==header);
}
static Node* FindPendingNode(Node* node,CThreadedFileManager::CFileObject* object) {
 if(!node || node->value==object)return node;Node* found=FindPendingNode(node->left,object);return found?found:FindPendingNode(node->right,object);
}
static void ValidatePendingQueues() {
 unsigned total=0;
 for(unsigned device=0;device<2;++device)for(unsigned priority=0;priority<7;++priority) {
  unsigned at=1+(device*7+priority)*4;Node* header=reinterpret_cast<Node*>(g_manager[at]);
  CHECK(header && !header->color);g_seenCount=g_orderCount=0;Walk(header->parent,header);
  unsigned expected[56],count=0;
  for(unsigned n=0;n<g_count;++n)if(!g_slots[n].cancelled && g_slots[n].device==device && (g_slots[n].callback?(g_slots[n].priority+1)%7:g_slots[n].priority)==priority)expected[count++]=n;
  for(unsigned i=1;i<count;++i) {unsigned value=expected[i],j=i;while(j && (value%3<expected[j-1]%3 || (value%3==expected[j-1]%3 && value<expected[j-1]))) {expected[j]=expected[j-1];--j;}expected[j]=value;}
  CHECK(count==g_seenCount && count==g_orderCount && count==g_manager[at+1]);total+=count;
  if(count)CHECK(header->parent->color==1 && header->left==g_order[0] && header->right==g_order[count-1]);
  else CHECK(!header->parent && header->left==header && header->right==header);
  bool cursorValid=g_manager[at+3]==reinterpret_cast<DWORD>(header);
  for(unsigned i=0;i<count;++i) {CHECK(g_order[i]->value==g_slots[expected[i]].request && g_order[i]->key==reinterpret_cast<DWORD>(g_slots[expected[i]].request));if(g_manager[at+3]==reinterpret_cast<DWORD>(g_order[i]))cursorValid=true;}
  CHECK(cursorValid);
 }
 unsigned live=0;for(unsigned n=0;n<g_count;++n)if(!g_slots[n].cancelled)++live;CHECK(total==live);++g_pendingChecks;
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
static void ValidateCallbackStatistics(Slot& current) {
 unsigned pending=0,pendingBytes=0,finished=0,finishedBytes=0;
 for(unsigned i=0;i<g_count;++i) {
  Slot& slot=g_slots[i];if(slot.cancelled || slot.userRemoved)continue;
  if(&slot==&current || !slot.apcCompletions) {++pending;pendingBytes+=slot.length;}
  else if(slot.retained) {++finished;finishedBytes+=slot.length;}
 }
 ValidateStatistics(pending,pendingBytes,finished,finishedBytes);
}
static void SubmitChild(Slot& parent) {
 CHECK(GetCurrentThreadId()==g_workerThread && !parent.child && parent.callback && parent.completed==1);
 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId());EnterCriticalSection(g_manager+0x14c/4);
 ValidateCallbackStatistics(parent);unsigned n=g_count;CHECK(n>=28 && n<40);
 Slot& child=g_slots[n];child.child=true;child.priority=(parent.priority+2)%7;child.device=parent.device;child.offset=0x1000+n*64;child.length=19+n%7;
 child.file[1]=child.device;child.file[2]=parent.file[2];child.file[5]=n%3;memset(child.buffer,0xcd,sizeof(child.buffer));
 CThreadedFileObjectDesc desc;memset(&desc,0,sizeof(desc));desc.PDst=child.buffer+16;desc.FilePos=child.offset;desc.Size=child.length;desc.Priority=static_cast<EThreadedFilePriority>(child.priority);
 LeaveCriticalSection(g_manager+0x14c/4);CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId());

 PrepareSharedBank(child);BankContext& ctx=*child.bankCtx;
 CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);
 CBankFileAsyncEntry* entry=reinterpret_cast<CBankFileAsyncEntry*>(ctx.entryWords+4);
 g_constructingBank=&child;
 CHECK(FixtureBankBeginRead(entry,0,child.ownerWords,child.offset,child.buffer+16,child.length,sizeof(child.buffer)-32,static_cast<EThreadedFilePriority>(child.priority))==child.ownerWords);
 g_constructingBank=0;
 CCountedPointer<CBankFileAsyncData>* owner=reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(child.ownerWords);
 CBankFileAsyncData* bankData=owner->Data;CHECK(bankData==child.bankDataBlock && owner->Info==child.bankInfoBlock);
 CHECK(owner->Info && owner->Info->RefCount==1 && owner->Info->Data==bankData && owner->Info->DeleteFunc);
 CHECK(bankData->Length==child.length && !bankData->SkipBytes && !bankData->Aborted && !bankData->BufferLocked && !bankData->AllowRestart);
 CHECK(!bankData->Finished && bankData->Buffer==child.buffer+16 && bankData->Data==child.buffer+16 && !bankData->MemoryPool && bankData->MemoryHandle==-1 && !bankData->InternalAllocatedBufferSize);
 child.uid=bankData->ThreadedFileUID;desc.PCallback=bankData;
 // Native descriptor construction preserves its three alignment bytes.
 for(unsigned pad=17;pad<20;++pad)reinterpret_cast<unsigned char*>(&desc)[pad]=0xcd;
 CHECK(memcmp(&desc,&bankData->LoadDesc,sizeof(desc))==0);
 ++ctx.constructed;
 CHECK(*reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(bank)+0x12c)==ctx.constructed);
 memcpy(child.bankExpected,bankData,72);

 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId());EnterCriticalSection(g_manager+0x14c/4);
 CHECK(child.uid==unsigned(g_startUID+n));
 Node* header=reinterpret_cast<Node*>(g_manager[0x13c/4]);Node* node=header->parent;
 while(node && node->key!=child.uid)node=child.uid<node->key?node->left:node->right;CHECK(node);
 child.request=node->value;child.overlap=child.request->Overlapped;
 CHECK(child.request->UID==child.uid && child.request->PManager==reinterpret_cast<CThreadedFileManager*>(g_manager) && child.request->PFile==reinterpret_cast<CThreadedFile*>(ctx.file));
 CHECK(memcmp(&child.request->Desc,&desc,sizeof(desc))==0 && child.request->LoadingStatus==0);
 child.request->Desc.PDst=0; // Require native OnFileReadBegin to publish Buffer.
 unsigned queue=1+(child.device*7+child.priority)*4;Node* pendingHeader=reinterpret_cast<Node*>(g_manager[queue]);
 Node* pendingNode=FindPendingNode(pendingHeader->parent,child.request);
 CHECK(pendingNode && pendingNode->key==reinterpret_cast<DWORD>(child.request) && g_manager[queue+1]>0);
 bool requestReuse=false,overlapReuse=false;
 for(unsigned i=0;i<n;++i) {
  if(g_slots[i].request==child.request) {CHECK(g_slots[i].deletes==1);requestReuse=true;}
  if(g_slots[i].overlap==child.overlap) {CHECK(g_slots[i].frees==1);overlapReuse=true;}
 }
 if(requestReuse)++g_requestReuses;if(overlapReuse)++g_overlapReuses;
 for(unsigned i=0;i<20;++i)CHECK(reinterpret_cast<unsigned char*>(child.overlap)[i]==0);
 ++g_count;++g_childSubmissions;ValidateBankTree(ctx);ValidateUID(false);ValidateCallbackStatistics(parent);
 CHECK(g_manager[0x168/4]==unsigned(g_startUID+g_count));LeaveCriticalSection(g_manager+0x14c/4);
 CHECK(g_manager[(0x14c+12)/4]!=GetCurrentThreadId());
}
static unsigned __stdcall Launch(void* context) {FixtureBeginThread(context);return 0;}
static unsigned char Payload(unsigned i) {return static_cast<unsigned char>(i*37+23);}
static void RunActive(HANDLE input);
int main(int argc,char** argv) {
 CHECK(argc==3);g_mainThread=GetCurrentThreadId();InstallObservers();InstallReadObserver();
 DWORD protection,restored;
 CHECK(VirtualProtect(&FixturePlainSleep,sizeof(FixturePlainSleep),4,&protection));g_realPlainSleep=FixturePlainSleep;FixturePlainSleep=ObservePlainSleep;CHECK(VirtualProtect(&FixturePlainSleep,sizeof(FixturePlainSleep),protection,&restored));
 CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),4,&protection));g_realGlobalFree=FixtureGlobalFreeImport;FixtureGlobalFreeImport=ObserveGlobalFree;CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),protection,&restored));
 HANDLE input=CreateFileA(argv[1],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);CHECK(input!=INVALID_HANDLE_VALUE);
 RunActive(input);CHECK(CloseHandle(input));return 0;
}

typedef void (__stdcall* BankLockCall)(void*);
typedef int (__stdcall* BankTryCall)(void*);
extern "C" BankLockCall FixtureBankEnterImport,FixtureBankLeaveImport;
extern "C" BankTryCall FixtureBankTryImport;
#pragma comment(linker,"/alternatename:_FixtureBankEnterImport=__imp__EnterCriticalSection@4")
#pragma comment(linker,"/alternatename:_FixtureBankLeaveImport=__imp__LeaveCriticalSection@4")
#pragma comment(linker,"/alternatename:_FixtureBankTryImport=__imp__TryEnterCriticalSection@4")
static BankLockCall g_realBankEnter,g_realBankLeave;static BankTryCall g_realBankTry;
static BankContext* BankForSection(void* section) {
 for(unsigned i=0;i<2;++i)if(g_banks[i].initialized && section==reinterpret_cast<CBankFileAsync*>(g_banks[i].bankWords+4)->CriticalSection)return &g_banks[i];
 return 0;
}
static void __stdcall ObserveBankEnter(void* section) {
 BankContext* ctx=BankForSection(section);
 if(ctx) {
  CHECK(GetCurrentThreadId()==g_mainThread || GetCurrentThreadId()==g_workerThread);CHECK(++ctx->enters<=6);
  if(GetCurrentThreadId()==g_workerThread)CHECK(g_releasingBank && g_retries==2 && g_manager[(0x14c+12)/4]!=g_workerThread);
 }
 g_realBankEnter(section);
}
static void __stdcall ObserveBankLeave(void* section) {
 BankContext* ctx=BankForSection(section);bool publish=false;
 if(ctx) {
  CHECK(++ctx->leaves<=8);
  if(GetCurrentThreadId()==g_workerThread && g_releasingBank && !g_published && g_releasingBank->request->LoadingStatus==3) {
   Slot& slot=*g_releasingBank;CBankFileAsyncData* data=static_cast<CBankFileAsyncData*>(slot.bankDataBlock);
   CHECK(slot.request->LoadingStatus==3 && slot.apcCompletions==1 && !slot.bankDataDeleted && slot.bankNodeDeleted);
   CHECK(*reinterpret_cast<DWORD*>(static_cast<char*>(section)+8)==1 && *reinterpret_cast<DWORD*>(static_cast<char*>(section)+12)==g_workerThread);
   unsigned char expected[72];memcpy(expected,slot.bankExpected,72);memset(expected+0x18,0,4);expected[0x44]=1;
   CHECK(memcmp(expected,data,72)==0 && data->Finished && !data->ThreadedFileUID);
   g_published=true;publish=true;
  }
 }
 g_realBankLeave(section);
 if(publish)CHECK(SetEvent(g_publishedEvent));
}
static int __stdcall ObserveBankTry(void* section) {
 BankContext* ctx=BankForSection(section);
 if(ctx) {

  CHECK(GetCurrentThreadId()==g_workerThread && ctx->tries<2*(ctx->reads+1));
  if(!ctx->tries) {
   Slot& slot=g_slots[28];CHECK(slot.bankCtx==ctx && slot.request->LoadingStatus==1 && !slot.apcCompletions);
   CHECK(!g_manager[0x138/4] && g_manager[(0x14c+12)/4]!=g_workerThread && !g_retries && !g_published);
   CHECK(slot.request->Desc.PCallback==static_cast<CBankFileAsyncData*>(slot.bankDataBlock) && !slot.request->Desc.PDst);
   CHECK(SetEvent(g_readIssued));CHECK(WaitForSingleObject(g_continueRead,5000)==WAIT_OBJECT_0);
   if(!g_beginRefusals) {
    CHECK(!g_releasingBank && !slot.bankNodeDeleted && !slot.bankDataDeleted && !slot.bankInfoDeleted);
    CHECK(*reinterpret_cast<DWORD*>(static_cast<char*>(section)+8)==1 && *reinterpret_cast<DWORD*>(static_cast<char*>(section)+12)==g_mainThread);
    int refused=g_realBankTry(section);CHECK(!refused);++g_beginRefusals;return refused;
   }
   CHECK(g_beginRefusals==1 && g_retries==2 && slot.bankNodeDeleted && !slot.bankDataDeleted && !slot.bankInfoDeleted);
  }
  CHECK(*reinterpret_cast<unsigned long*>(static_cast<char*>(section)+8)==ctx->tries%2);++ctx->tries;
 }
 int result=g_realBankTry(section);if(ctx)CHECK(result);return result;
}
static void InstallBankLockObservers() {
 DWORD protection,restored;
 CHECK(VirtualProtect(&FixtureBankEnterImport,sizeof(FixtureBankEnterImport),4,&protection));
 g_realBankEnter=FixtureBankEnterImport;FixtureBankEnterImport=ObserveBankEnter;
 CHECK(VirtualProtect(&FixtureBankEnterImport,sizeof(FixtureBankEnterImport),protection,&restored));
 CHECK(VirtualProtect(&FixtureBankLeaveImport,sizeof(FixtureBankLeaveImport),4,&protection));
 g_realBankLeave=FixtureBankLeaveImport;FixtureBankLeaveImport=ObserveBankLeave;
 CHECK(VirtualProtect(&FixtureBankLeaveImport,sizeof(FixtureBankLeaveImport),protection,&restored));
 CHECK(VirtualProtect(&FixtureBankTryImport,sizeof(FixtureBankTryImport),4,&protection));
 g_realBankTry=FixtureBankTryImport;FixtureBankTryImport=ObserveBankTry;
 CHECK(VirtualProtect(&FixtureBankTryImport,sizeof(FixtureBankTryImport),protection,&restored));
}
static void CheckBankAllocation(void* block,unsigned size) {
 unsigned char* bytes=static_cast<unsigned char*>(block);
 for(unsigned i=0;i<16;++i)CHECK(bytes[int(i)-16]==0xcd && bytes[size+i]==0xcd);
}
static void* AllocateBankStorage(size_t size) {
 CHECK(g_constructingBank && GetCurrentThreadId()==g_mainThread && (size==72 || size==12));
 void*& output=size==72?g_constructingBank->bankDataBlock:g_constructingBank->bankInfoBlock;CHECK(!output);
 unsigned char* allocation=static_cast<unsigned char*>(malloc(size+32));CHECK(allocation);memset(allocation,0xcd,size+32);
 output=allocation+16;return output;
}
static bool DeleteBankStorage(void* block) {
 for(unsigned i=0;i<g_count;++i) {
  Slot& slot=g_slots[i];bool data=block==slot.bankDataBlock,info=block==slot.bankInfoBlock;
  if(!data && !info)continue;
  CHECK(g_releasingBank==&slot && GetCurrentThreadId()==g_mainThread && !g_shutdown && g_published && g_retries==2);
  bool& deleted=data?slot.bankDataDeleted:slot.bankInfoDeleted;CHECK(!deleted);deleted=true;
  CHECK(slot.bankNodeDeleted);
  CheckBankAllocation(block,data?72:12);if(data)++g_nativeBankDataDeletes;else ++g_nativeBankInfoDeletes;
  free(static_cast<unsigned char*>(block)-16);return true;
 }
 return false;
}
static void ReleaseBankOwner(Slot& slot) {
 BankContext& ctx=*slot.bankCtx;CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);
 CCountedPointer<CBankFileAsyncData>* owner=reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(slot.ownerWords);
 CHECK(slot.child && slot.deletes==1 && slot.frees==1 && owner->Data==slot.bankDataBlock && owner->Info==slot.bankInfoBlock);
 unsigned char expected[72];memcpy(expected,slot.bankExpected,72);memset(expected+0x18,0,4);expected[0x44]=1;
 CHECK(*reinterpret_cast<unsigned long*>(bank->CriticalSection+8)==0 && *reinterpret_cast<unsigned long*>(bank->CriticalSection+12)==0);
 CHECK(memcmp(expected,owner->Data,72)==0 && owner->Info->RefCount==1 && owner->Data->Finished && !owner->Data->ThreadedFileUID);
 CheckBankAllocation(slot.bankDataBlock,72);CheckBankAllocation(slot.bankInfoBlock,12);CheckBankAllocation(slot.bankNodeBlock,20);
 unsigned long* node=static_cast<unsigned long*>(slot.bankNodeBlock);
 if(ctx.constructed-ctx.released>1)++g_sharedPartialDeletes;if(node[2] && node[3])++g_sharedTwoChildDeletes;
 g_releasingBank=&slot;owner->~CCountedPointer<CBankFileAsyncData>();g_releasingBank=0;
 CHECK(slot.bankDataDeleted && slot.bankInfoDeleted && slot.bankNodeDeleted && !owner->Data && !owner->Info);++ctx.released;
 ValidateBankTree(ctx);
}
extern "C" void* __cdecl FixtureBankTreeMalloc(size_t size) {
 CHECK(g_constructingBank && GetCurrentThreadId()==g_mainThread && size==20 && !g_constructingBank->bankNodeBlock);
 unsigned char* allocation=static_cast<unsigned char*>(malloc(size+32));CHECK(allocation);memset(allocation,0xcd,size+32);
 g_constructingBank->bankNodeBlock=allocation+16;return allocation+16;
}
extern "C" void __cdecl FixtureBankTreeFree(void* block) {
 CHECK(g_releasingBank && block==g_releasingBank->bankNodeBlock && !g_releasingBank->bankNodeDeleted && GetCurrentThreadId()==g_mainThread);
 CheckBankAllocation(block,20);g_releasingBank->bankNodeDeleted=true;++g_nativeBankNodeDeletes;
 free(static_cast<unsigned char*>(block)-16);
}
extern "C" void __cdecl UnusedBankBoundary() {Fail("cold bank dependency",__LINE__);}
#pragma comment(linker,"/alternatename:__purecall=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?CanRelocateData@CMovableResource@@UBE_NJ@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:??ACPackedUIntArray@@QBEKK@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?GetEntryDataSize@CBankFile@@QBEKK@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?LeaveSynchronisation@CMemoryAllocator@@KIXXZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?EnterSynchronisation@CMemoryAllocator@@KIXXZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?Create@CMemoryAllocator@@KIXXZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?AllocateResource@CMovableResourceMemoryPool@@QAE_NPAJPAVCMovableResource@@KK@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:??3CMemoryAllocator@@KIXPAX@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:??1CMemoryAllocator@@IAE@XZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?GetNoAllocations@CMemoryAllocator@@QAEKXZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z=_UnusedBankBoundary")
class CMemoryAllocator; CMemoryAllocator* g_FableMemoryAllocator_013CAA90=0;
bool g_FableAllocatorShutdownRequested_013CAA94=false;
fable_u32 g_CBankFileAsyncFailureHandlingMode_013CA868=0;

static void PrepareSharedBank(Slot& slot) {
 CHECK(slot.device<2);BankContext& ctx=g_banks[slot.device];slot.bankCtx=&ctx;
 if(!ctx.initialized) {
  CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);
  CBankFileAsyncEntry* entry=reinterpret_cast<CBankFileAsyncEntry*>(ctx.entryWords+4);
  for(unsigned guard=0;guard<4;++guard) {ctx.bankWords[guard]=0xcdcdcdcd;ctx.bankWords[guard+4+sizeof(CBankFileAsync)/4]=0xcdcdcdcd;}
  memcpy(ctx.file,slot.file,sizeof(ctx.file));InitializeCriticalSection(bank->CriticalSection);
  ctx.bankHeader[2]=ctx.bankHeader[3]=reinterpret_cast<DWORD>(ctx.bankHeader);
  ctx.restartHeader[2]=ctx.restartHeader[3]=reinterpret_cast<DWORD>(ctx.restartHeader);
  *reinterpret_cast<void**>(reinterpret_cast<char*>(bank)+0x128)=ctx.bankHeader;
  *reinterpret_cast<void**>(reinterpret_cast<char*>(bank)+0x134)=ctx.restartHeader;
  *reinterpret_cast<CThreadedFile**>(reinterpret_cast<char*>(bank)+0x110)=reinterpret_cast<CThreadedFile*>(ctx.file);
  *reinterpret_cast<void**>(reinterpret_cast<char*>(bank)+0x20)=ctx.runtimeEntry;
  bank->Alignment=1;bank->PackedDataOffset.Size=0;ctx.runtimeEntry[0]=0;
  *reinterpret_cast<CBankFileAsync**>(reinterpret_cast<char*>(entry)+4)=bank;entry->Index=0;
  ctx.initialized=true;++g_sharedContexts;
 }
 CHECK(ctx.file[1]==slot.device && ctx.file[2]==slot.file[2]);slot.file[5]=ctx.file[5];
}
struct BankNode {unsigned char color,pad[3];BankNode* parent;BankNode* left;BankNode* right;CBankFileAsyncData* value;};
static BankNode* g_bankVisited[12];static unsigned g_bankVisitedCount;
static unsigned g_bankSorted[12],g_bankSortedCount;
static unsigned WalkBankTree(BankContext& ctx,BankNode* node,BankNode* parent) {
 if(!node)return 1;
 unsigned index=28;while(index<g_count && (!g_slots[index].child || g_slots[index].bankCtx!=&ctx || g_slots[index].bankNodeDeleted || g_slots[index].bankNodeBlock!=node))++index;
 CHECK(index<g_count);CheckBankAllocation(node,20);
 for(unsigned i=0;i<g_bankVisitedCount;++i)CHECK(g_bankVisited[i]!=node);CHECK(g_bankVisitedCount<12);g_bankVisited[g_bankVisitedCount++]=node;
 CHECK(node->parent==parent && node->color<=1 && node->value==g_slots[index].bankDataBlock);
 unsigned left=WalkBankTree(ctx,node->left,node);g_bankSorted[g_bankSortedCount++]=reinterpret_cast<unsigned>(node->value);
 unsigned right=WalkBankTree(ctx,node->right,node);CHECK(left==right);
 if(!node->color) {CHECK(!node->left || node->left->color==1);CHECK(!node->right || node->right->color==1);}
 return left+node->color;
}
static void ValidateBankTree(BankContext& ctx) {
 CHECK(ctx.initialized);++g_sharedTreeChecks;unsigned expected[12],count=0;
 for(unsigned i=28;i<g_count;++i)if(g_slots[i].child && g_slots[i].bankCtx==&ctx && !g_slots[i].bankDataDeleted)expected[count++]=reinterpret_cast<unsigned>(g_slots[i].bankDataBlock);
 CHECK(count==ctx.constructed-ctx.released);
 for(unsigned i=1;i<count;++i) {unsigned value=expected[i],j=i;while(j && value<expected[j-1]) {expected[j]=expected[j-1];--j;}expected[j]=value;}
 CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);CHECK(*reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(bank)+0x12c)==count);
 BankNode* header=reinterpret_cast<BankNode*>(ctx.bankHeader);CHECK(!header->color);g_bankVisitedCount=g_bankSortedCount=0;
 WalkBankTree(ctx,header->parent,header);CHECK(g_bankVisitedCount==count && g_bankSortedCount==count);
 for(unsigned i=0;i<count;++i)CHECK(g_bankSorted[i]==expected[i]);
 if(count) {
  bool leftKnown=false,rightKnown=false;
  for(unsigned i=0;i<g_bankVisitedCount;++i) {if(g_bankVisited[i]==header->left)leftKnown=true;if(g_bankVisited[i]==header->right)rightKnown=true;}
  CHECK(leftKnown && rightKnown);
  CHECK(header->parent->color==1 && reinterpret_cast<unsigned>(header->left->value)==expected[0] && reinterpret_cast<unsigned>(header->right->value)==expected[count-1]);
 }
 else CHECK(!header->parent && header->left==header && header->right==header);
 CHECK(!*reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(bank)+0x138) && !ctx.restartHeader[1]);
}
static void ReleaseSharedOwners(unsigned policy) {
 for(unsigned left=12;left;--left) {
  unsigned candidates[12],count=0;
  for(unsigned i=28;i<g_count;++i)if(!g_slots[i].bankDataDeleted)candidates[count++]=i;CHECK(count==left);
  unsigned chosen=candidates[0];
  if(policy==1)chosen=candidates[count-1];
  if(policy==2) {
   for(unsigned i=1;i<count;++i) {unsigned value=candidates[i],j=i;while(j && reinterpret_cast<unsigned>(g_slots[value].bankDataBlock)<reinterpret_cast<unsigned>(g_slots[candidates[j-1]].bankDataBlock)) {candidates[j]=candidates[j-1];--j;}candidates[j]=value;}
   chosen=candidates[count/2];
  }
  if(policy==3) {
   BankContext& ctx=*g_slots[chosen].bankCtx;BankNode* root=reinterpret_cast<BankNode*>(ctx.bankHeader[1]);CHECK(root);
   for(unsigned i=0;i<count;++i)if(g_slots[candidates[i]].bankDataBlock==root->value) {chosen=candidates[i];break;}
  }
  ReleaseBankOwner(g_slots[chosen]);
 }
}
static void FinishSharedBanks() {
 for(unsigned i=0;i<2;++i) {
  BankContext& ctx=g_banks[i];CHECK(ctx.initialized && ctx.constructed>=2 && ctx.reads==ctx.constructed && ctx.completed==ctx.constructed && ctx.released==ctx.constructed);
  CHECK(ctx.enters==3*ctx.constructed && ctx.leaves==5*ctx.constructed && ctx.tries==2*ctx.constructed);ValidateBankTree(ctx);
  CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);DeleteCriticalSection(bank->CriticalSection);
  for(unsigned guard=0;guard<4;++guard)CHECK(ctx.bankWords[guard]==0xcdcdcdcd && ctx.bankWords[guard+4+sizeof(CBankFileAsync)/4]==0xcdcdcdcd);
  ctx.initialized=false;
 }
}

static void CheckPendingPriority(Slot& slot,unsigned priority) {
 EnterCriticalSection(g_manager+0x14c/4);
 CHECK(slot.request->LoadingStatus==0 && unsigned(slot.request->Desc.Priority)==priority);
 CHECK(slot.request->Desc.PCallback==static_cast<CBankFileAsyncData*>(slot.bankDataBlock) && !slot.request->Desc.PDst);
 CHECK(!slot.frees && !slot.deletes && !slot.apcCompletions && !g_manager[0x138/4] && g_manager[0x140/4]==1);
 Node* uidHead=reinterpret_cast<Node*>(g_manager[0x13c/4]);
 CHECK(uidHead->parent && uidHead->parent->key==slot.uid && uidHead->parent->value==slot.request);
 for(unsigned q=0;q<14;++q) {
  unsigned at=1+q*4;Node* header=reinterpret_cast<Node*>(g_manager[at]);CHECK(header);
  if(q==slot.device*7+priority) {
   Node* node=header->parent;CHECK(g_manager[at+1]==1 && node && node->parent==header);
   CHECK(!node->left && !node->right && node->color==1 && header->left==node && header->right==node);
   CHECK(node->key==reinterpret_cast<DWORD>(slot.request) && node->value==slot.request);
   CHECK(g_manager[at+3]==reinterpret_cast<DWORD>(header) || g_manager[at+3]==reinterpret_cast<DWORD>(node));
  } else CHECK(!g_manager[at+1] && !header->parent && header->left==header && header->right==header && g_manager[at+3]==reinterpret_cast<DWORD>(header));
 }
 LeaveCriticalSection(g_manager+0x14c/4);
}

static void RunActive(HANDLE input) {
 g_count=28;g_shutdown=false;memset(g_slots,0,sizeof(g_slots));memset(g_banks,0,sizeof(g_banks));
 g_refusalReady=CreateEventA(0,1,0,0);g_continueRetry=CreateEventA(0,1,0,0);CHECK(g_refusalReady && g_continueRetry);
 g_readIssued=CreateEventA(0,1,0,0);g_continueRead=CreateEventA(0,1,0,0);g_publishedEvent=CreateEventA(0,1,0,0);g_requestRetired=CreateEventA(0,1,0,0);g_waitRelease=CreateEventA(0,1,1,0);CHECK(g_readIssued && g_continueRead && g_publishedEvent && g_requestRetired && g_waitRelease);
 unsigned long installer[0xa0/4];memset(installer,0,sizeof(installer));g_installer=installer;
 CFileInstaller* installerObject=reinterpret_cast<CFileInstaller*>(installer);
 HANDLE installerEvent=CreateEventA(0,0,0,0),pauseStart=CreateEventA(0,0,0,0),pauseFinish=CreateEventA(0,0,0,0);CHECK(installerEvent && pauseStart && pauseFinish);
 installerObject->Event=installerEvent;installerObject->PauseStartEvent=pauseStart;installerObject->PauseFinishEvent=pauseFinish;
 InitializeCriticalSection(installerObject->CriticalSection);g_CFileInstallerSingleton_013CA818.object=installerObject;
 void* startHead[2];void* startNode[2];void* idleHead[2];startHead[0]=startHead[1]=startNode;startNode[0]=startNode[1]=startHead;idleHead[0]=idleHead[1]=idleHead;
 installer[0x38/4]=reinterpret_cast<DWORD>(startHead);installer[0x3c/4]=reinterpret_cast<DWORD>(idleHead);installer[0x68/4]=1;
 unsigned installerID;g_installerHandle=reinterpret_cast<HANDLE>(_beginthreadex(0,0,InstallerLaunch,installer,0,&installerID));CHECK(g_installerHandle);
 DWORD started=GetTickCount();for(;;) {EnterCriticalSection(installerObject->CriticalSection);bool ready=installerObject->Running;LeaveCriticalSection(installerObject->CriticalSection);if(ready)break;CHECK(GetTickCount()-started<5000);Sleep(1);}
 CHECK(GetThreadPriority(g_installerHandle)==1);
 DWORD previous[128];unsigned previousCount=Threads(previous);CThreadedFileManager* manager=new CThreadedFileManager;CaptureWorker(previous,previousCount);g_manager=reinterpret_cast<unsigned long*>(manager);
 EnterCriticalSection(g_manager+0x14c/4);
 Slot& slot=g_slots[28];slot.child=true;slot.device=0;slot.priority=3;slot.offset=0x1000;slot.length=37;
 slot.file[1]=0;slot.file[2]=reinterpret_cast<DWORD>(input);memset(slot.buffer,0xcd,sizeof(slot.buffer));PrepareSharedBank(slot);
 BankContext& ctx=*slot.bankCtx;CBankFileAsyncEntry* entry=reinterpret_cast<CBankFileAsyncEntry*>(ctx.entryWords+4);
 g_constructingBank=&slot;CHECK(FixtureBankBeginRead(entry,0,slot.ownerWords,slot.offset,slot.buffer+16,slot.length,96,THREADED_FILE_PRIORITY_NORMAL)==slot.ownerWords);g_constructingBank=0;
 CCountedPointer<CBankFileAsyncData>* owner=reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(slot.ownerWords);
 CHECK(owner->Data && owner->Info && owner->Data==slot.bankDataBlock && owner->Info==slot.bankInfoBlock && owner->Info->RefCount==1 && owner->Data->ThreadedFileUID==1 && !owner->Data->Finished);
 CHECK(owner->Info->Data==owner->Data && owner->Info->DeleteFunc && owner->Data->Length==slot.length && owner->Data->Buffer==slot.buffer+16 && owner->Data->Data==slot.buffer+16 && !owner->Data->Aborted && !owner->Data->AllowRestart && owner->Data->MemoryHandle==-1 && !owner->Data->InternalAllocatedBufferSize);
 memcpy(slot.bankExpected,owner->Data,72);slot.uid=1;Node* head=reinterpret_cast<Node*>(g_manager[0x13c/4]);CHECK(head->parent && head->parent->key==1);slot.request=head->parent->value;slot.overlap=slot.request->Overlapped;
 CHECK(slot.request->LoadingStatus==0 && slot.request->Desc.PCallback==owner->Data);slot.request->Desc.PDst=0;++ctx.constructed;++g_count;ValidateBankTree(ctx);LeaveCriticalSection(g_manager+0x14c/4);
 CHECK(WaitForSingleObject(g_readIssued,5000)==WAIT_OBJECT_0);
 CBankFileAsync* contentionBank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);
 EnterCriticalSection(contentionBank->CriticalSection);
 CHECK(SetEvent(g_continueRead));CHECK(WaitForSingleObject(g_refusalReady,5000)==WAIT_OBJECT_0);
 CHECK(g_beginRefusals==1 && slot.request->LoadingStatus==0 && !slot.request->Desc.PDst);
 CHECK(memcmp(slot.bankExpected,owner->Data,72)==0 && owner->Info->RefCount==1);
 CheckPendingPriority(slot,3);
 owner->Data->ChangeLoadingPriority(THREADED_FILE_PRIORITY_HIGHEST);CheckPendingPriority(slot,5);
 CHECK(memcmp(slot.bankExpected,owner->Data,72)==0 && owner->Info->RefCount==1);
 owner->Data->ChangeLoadingPriority(THREADED_FILE_PRIORITY_LOW);CheckPendingPriority(slot,1);
 CHECK(memcmp(slot.bankExpected,owner->Data,72)==0 && owner->Info->RefCount==1);
 slot.priority=1;
 CHECK(ResetEvent(g_readIssued) && ResetEvent(g_continueRead));
 LeaveCriticalSection(contentionBank->CriticalSection);CHECK(SetEvent(g_continueRetry));
 CHECK(WaitForSingleObject(g_readIssued,5000)==WAIT_OBJECT_0);
 CHECK(slot.request->LoadingStatus==1 && !slot.apcCompletions && !slot.frees && !slot.deletes && !slot.bankNodeDeleted && !slot.bankDataDeleted && !slot.bankInfoDeleted);
 CHECK(g_manager[(0x14c+12)/4]!=g_mainThread);g_releaseStarted=GetTickCount();g_releasingBank=&slot;owner->~CCountedPointer<CBankFileAsyncData>();g_releasingBank=0;++ctx.released;
 CHECK(g_published && g_retries==2 && slot.bankNodeDeleted && slot.bankDataDeleted && slot.bankInfoDeleted && !owner->Data && !owner->Info);
 // Reclamation can precede manager request retirement. Join before reading its ledgers.
 CHECK(WaitForSingleObject(g_requestRetired,5000)==WAIT_OBJECT_0);EnterCriticalSection(g_manager+0x14c/4);CHECK(!g_manager[0x140/4] && !g_manager[0x138/4]);LeaveCriticalSection(g_manager+0x14c/4);
 g_shutdown=true;manager->Uninitialise();CHECK(WaitForSingleObject(g_ownedWorker,5000)==WAIT_OBJECT_0);
 CHECK(slot.frees==1 && slot.deletes==1 && ctx.completed==1 && g_nativeBankCompletions==1 && g_successAPCs==1 && g_successBytes==37);
 CHECK(ctx.enters==6 && ctx.leaves==8 && ctx.tries==2 && ctx.reads==1 && g_beginRefusals==1);ValidateBankTree(ctx);
 for(unsigned i=0;i<sizeof(slot.buffer);++i)CHECK(slot.buffer[i]==(i>=16 && i<53?Payload(slot.offset+i-16):0xcd));
 CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);DeleteCriticalSection(bank->CriticalSection);ctx.initialized=false;
 CHECK(g_ackSignals==1 && g_resumeSignals==1 && g_pauseWaits==1 && g_pauseReturns==1 && !installerObject->Paused);
 EnterCriticalSection(installerObject->CriticalSection);reinterpret_cast<unsigned char*>(installer)[0x7a]=1;LeaveCriticalSection(installerObject->CriticalSection);CHECK(SetEvent(installerEvent) && WaitForSingleObject(g_installerHandle,5000)==WAIT_OBJECT_0);
 CHECK(CloseHandle(g_installerHandle));DeleteCriticalSection(installerObject->CriticalSection);CHECK(CloseHandle(installerEvent) && CloseHandle(pauseStart) && CloseHandle(pauseFinish));g_CFileInstallerSingleton_013CA818.object=0;g_installer=0;
 CHECK(CloseHandle(g_refusalReady) && CloseHandle(g_continueRetry));
 delete manager;CHECK(!g_managerAllocated && CloseHandle(g_ownedWorker) && CloseHandle(g_waitRelease) && CloseHandle(g_readIssued) && CloseHandle(g_continueRead) && CloseHandle(g_publishedEvent) && CloseHandle(g_requestRetired));
 CHECK(g_nativeBankDataDeletes==1 && g_nativeBankInfoDeletes==1 && g_nativeBankNodeDeletes==1);
 printf("PRIORITY_OWNER_PASS bank_priority_moves=2 begin_refusals=1 worker_retry=1 real_read=1 bytes=37 retries=2 publication_before_free=1 request_retired=1 data_deleted=1 info_deleted=1 node_deleted=1 worker_joined=1 installer_joined=1\n");
}
