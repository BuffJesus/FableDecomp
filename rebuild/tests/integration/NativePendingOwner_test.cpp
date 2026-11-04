// Owned Windows worker submission probe; no installed-game process is started.
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
static bool g_pendingMode=true;static HANDLE g_pendingBlockedEvent;static volatile unsigned g_pendingBlockProofs;
static unsigned g_pendingCancelled,g_pendingManagerChecks,g_pendingJoins;
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
 unsigned char bankExpected[72],requestExpected[64];
 CThreadedFileManager::CFileObject* request;
 void* overlap;
 unsigned uid,offset,length,priority,device,begun,completed,frees,deletes,apcCompletions;
 bool write,callback,retained,removing,userRemoved,cancelling,cancelled,probing,child;
 unsigned long file[0x1c/4];
 unsigned char buffer[128];
};
static Slot g_slots[56];static unsigned g_count,g_protectedCalls;
static Slot* g_constructingBank;static Slot* g_releasingBank;
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
   CBankFileAsyncData* data=static_cast<CBankFileAsyncData*>(slot.bankDataBlock);
   BankContext& ctx=*slot.bankCtx;
   CHECK(g_pendingMode && slot.cancelling && slot.removing && slot.bankNodeDeleted && !slot.bankDataDeleted);
   CHECK(data && !data->Finished && data->ThreadedFileUID==slot.uid && data->Buffer==slot.buffer+16);
   CHECK(ctx.enters==ctx.constructed+ctx.released+1 && ctx.leaves==ctx.constructed+ctx.released && !ctx.tries && !ctx.reads && !ctx.completed);
   CHECK(*reinterpret_cast<unsigned long*>(reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4)->CriticalSection+8)==1);
   CCountedPointer<CBankFileAsyncData>* owner=reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(slot.ownerWords);CHECK(owner->Info->RefCount==0);
   CHECK(!slot.apcCompletions && !slot.begun && !slot.completed);++g_pendingCancelled;
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
 if(g_pendingMode && g_manager && event==reinterpret_cast<HANDLE>(g_manager[0x164/4])) {
  CHECK(GetCurrentThreadId()==g_workerThread && timeout==0xffffffffUL && alertable);
  EnterCriticalSection(g_manager+0x14c/4);
  if(g_manager[0x140/4] && g_manager[0x16c/4]==1 && !g_manager[0x138/4]) {if(++g_pendingBlockProofs>=2)CHECK(SetEvent(g_pendingBlockedEvent));}
  LeaveCriticalSection(g_manager+0x14c/4);
 }

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
 CHECK(!g_pendingMode);
 CHECK(completion && (!g_realCompletion || completion==g_realCompletion));g_realCompletion=completion;
 unsigned i=0;while(i<g_count && (g_slots[i].overlap!=overlap || g_slots[i].frees))++i;CHECK(i<g_count);
 if(g_slots[i].child) {BankContext& ctx=*g_slots[i].bankCtx;CHECK(ctx.enters==ctx.constructed+ctx.completed && ctx.leaves==ctx.constructed+ctx.completed+2*(ctx.reads+1) && ctx.tries==2*(ctx.reads+1));++ctx.reads;}
 CHECK(buffer==g_slots[i].buffer+16 && bytes==g_slots[i].length && file==reinterpret_cast<HANDLE>(g_slots[i].file[2]));
 BOOL result=g_realRead(file,buffer,bytes,overlap,ObserveAPC);CHECK(result);return result;
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
static void ValidatePendingManager();
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
static void Run(HANDLE input,HANDLE output,unsigned variant,DWORD paused,bool running,bool dirty) {
 g_count=0;g_protectedCalls=0;g_errorCallbacks=g_readingRefusals=g_completedRefusals=0;FixtureManagerDirty=false;g_shutdown=false;g_installerThread=0;g_waitSleeps=0;g_waitRelease=CreateEventA(0,1,0,0);CHECK(g_waitRelease);g_ackSignals=g_resumeSignals=g_pauseWaits=g_pauseReturns=g_idleReads=g_idleAlways=0;memset(g_slots,0,sizeof(g_slots));memset(g_banks,0,sizeof(g_banks));
 unsigned bankReleasePolicy=g_sharedBatches++%4;++g_releaseOrders[bankReleasePolicy];
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
 unsigned startUID=variant?0xfffffffeUL:0xfffffff0UL;manager[0x168/4]=startUID;g_startUID=startUID;
 void* vtable[3]={0,reinterpret_cast<void*>(Begin),reinterpret_cast<void*>(Complete)};
 const CThreadedFileManager& view=*owner;unsigned submittedBytes=0;
 for(unsigned n=0;n<28;++n) {
  Slot& slot=g_slots[n];slot.vtable=vtable;slot.priority=n%7;slot.device=(n/7)%2;slot.offset=0x9000+n*64;slot.length=17+n%7;
  slot.write=false;slot.callback=((n/2+variant)%2)!=0;slot.retained=((n/4+variant)%2)!=0;
  slot.file[1]=slot.device;slot.file[2]=reinterpret_cast<DWORD>(slot.write?output:input);slot.file[5]=n%3;
  reinterpret_cast<unsigned char*>(slot.file)[0x18]=slot.write;
  memset(slot.buffer,0xcd,sizeof(slot.buffer));
  if(slot.write)for(unsigned j=0;j<slot.length;++j)slot.buffer[16+j]=Payload(slot.offset+j)^0x5a;
  CThreadedFileObjectDesc desc;memset(&desc,0,sizeof(desc));
  desc.PDst=slot.buffer+16;desc.FilePos=slot.offset;desc.Size=slot.length;
  desc.Priority=static_cast<EThreadedFilePriority>(slot.callback && !slot.write?(slot.priority+1)%7:slot.priority);
  desc.QueueWhenFinished=slot.retained;desc.PCallback=slot.callback?reinterpret_cast<CThreadedFileCallback*>(&slot):0;
  unsigned uid=reinterpret_cast<CThreadedFile*>(slot.file)->AddObject(desc);CHECK(uid==unsigned(startUID+n));slot.uid=uid;if(!uid)++g_zeroSubmissions;
  Node* header=reinterpret_cast<Node*>(manager[0x13c/4]);Node* node=header->parent;
  while(node && node->key!=uid)node=uid<node->key?node->left:node->right;CHECK(node);
  slot.request=node->value;slot.overlap=slot.request->Overlapped;++g_count;
  CHECK(slot.request->UID==uid && slot.request->PManager==owner && slot.request->PFile==reinterpret_cast<CThreadedFile*>(slot.file));
  for(unsigned j=0;j<20;++j)CHECK(reinterpret_cast<unsigned char*>(slot.overlap)[j]==0);
  CHECK(reinterpret_cast<CThreadedFile*>(slot.file)->ObjectExists(uid) && !reinterpret_cast<CThreadedFile*>(slot.file)->IsObjectFinished(uid));ValidateUID(false);submittedBytes+=slot.length;ValidateStatistics(n+1,submittedBytes,0,0);
 }
 CHECK(manager[0x168/4]==unsigned(startUID+28));
 unsigned zeroIndex=variant?2:16;Slot& zero=g_slots[zeroIndex];CHECK(!zero.uid && !zero.callback);
 zero.priority=(zero.priority+3)%7;reinterpret_cast<CThreadedFile*>(zero.file)->SetObjectPriority(0,static_cast<EThreadedFilePriority>(zero.priority));
 CHECK(zero.request->Desc.Priority==zero.priority && zero.request->LoadingStatus==0);++g_zeroPriorityMoves;
 ValidateUID(false);
 ValidatePendingQueues();unsigned remainingBytes=submittedBytes;
 for(unsigned step=0;step<4;++step) {
  Slot& slot=g_slots[step*7];unsigned priority=slot.request->Desc.Priority;
  unsigned at=1+(slot.device*7+priority)*4;Node* header=reinterpret_cast<Node*>(manager[at]);Node* target=FindPendingNode(header->parent,slot.request);CHECK(target);
  if(step%2==0)manager[at+3]=reinterpret_cast<DWORD>(target);
  slot.removing=slot.cancelling=true;CHECK(reinterpret_cast<CThreadedFile*>(slot.file)->RemoveObject(slot.uid));
  slot.removing=slot.cancelling=false;slot.cancelled=slot.userRemoved=true;remainingBytes-=slot.length;
  CHECK(slot.frees==1 && slot.deletes==1 && manager[at+3]!=reinterpret_cast<DWORD>(target));
  CHECK(!reinterpret_cast<CThreadedFile*>(slot.file)->ObjectExists(slot.uid) && reinterpret_cast<CThreadedFile*>(slot.file)->IsObjectFinished(slot.uid));
  CHECK(reinterpret_cast<CThreadedFile*>(slot.file)->RemoveObject(slot.uid) && slot.frees==1 && slot.deletes==1);
  ValidateUID(false);ValidatePendingQueues();ValidateStatistics(27-step,remainingBytes,0,0);
 }
 for(unsigned p=0;p<7;++p)manager[0xe4/4+p]=0;
 for(unsigned p=0;p<7;++p)reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(manager)+0x100)[p]=9.0;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[0]=0.25f;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[1]=0.5f;
 CHECK(manager[0x164/4] && reinterpret_cast<unsigned char*>(manager)[0x148]);
 CHECK(g_mainThread==GetCurrentThreadId() && g_workerThread && g_ownedWorker);
 LeaveCriticalSection(manager+0x14c/4);
 g_waitStarted=GetTickCount();
 for(unsigned n=0;n<28;++n) {g_waitUID=g_slots[n].uid;reinterpret_cast<CThreadedFile*>(g_slots[n].file)->WaitForObjectToFinish(g_waitUID);}
 CHECK(g_count==40 && g_waitSleeps>0);
 for(unsigned n=28;n<g_count;++n) {g_waitUID=g_slots[n].uid;reinterpret_cast<CThreadedFile*>(g_slots[n].file)->WaitForObjectToFinish(g_waitUID);}
 // Repeat every completed/missing UID and an unrelated UID; all must return.
 for(unsigned n=0;n<28;++n) {g_waitUID=g_slots[n].uid;reinterpret_cast<CThreadedFile*>(g_slots[n].file)->WaitForObjectToFinish(g_waitUID);}
 reinterpret_cast<CThreadedFile*>(g_slots[0].file)->WaitForObjectToFinish(0x7fffffffUL);
 EnterCriticalSection(manager+0x14c/4);ValidateUID(true);ValidateFinishedStatistics();CHECK(!manager[0x138/4]);
 for(unsigned n=0;n<g_count;++n) {
  Slot& slot=g_slots[n];
  if(slot.cancelled) {
   CHECK(slot.frees==1 && slot.deletes==1 && !slot.apcCompletions && !slot.begun && !slot.completed);
   CHECK(!reinterpret_cast<CThreadedFile*>(slot.file)->ObjectExists(slot.uid) && reinterpret_cast<CThreadedFile*>(slot.file)->IsObjectFinished(slot.uid));
   for(unsigned j=0;j<sizeof(slot.buffer);++j)CHECK(slot.buffer[j]==0xcd);continue;
  }
  CHECK(reinterpret_cast<CThreadedFile*>(slot.file)->ObjectExists(slot.uid)==slot.retained && reinterpret_cast<CThreadedFile*>(slot.file)->IsObjectFinished(slot.uid));
  CHECK(slot.frees==unsigned(!slot.retained) && slot.deletes==unsigned(!slot.retained));
  CHECK(slot.completed==unsigned(slot.callback) && slot.begun==2*unsigned(slot.callback && !slot.write));
  if(slot.retained)CHECK(slot.request->LoadingStatus==4 && (slot.request->Desc.Priority==slot.priority || slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING));
  for(unsigned j=0;j<slot.length;++j)CHECK(slot.buffer[16+j]==(slot.child?Payload(slot.offset+j):0xcd));
  CHECK(slot.apcCompletions==1);
  for(unsigned j=0;j<16;++j)CHECK(slot.buffer[j]==0xcd);
  for(unsigned j=16+slot.length;j<sizeof(slot.buffer);++j)CHECK(slot.buffer[j]==0xcd);
 }
 CHECK(g_protectedCalls==12 && g_readingRefusals==36 && g_completedRefusals==12 && g_errorCallbacks==12 && FixtureManagerDirty);
 for(unsigned n=0;n<28;++n) {
  Slot& slot=g_slots[n];if(slot.cancelled || !slot.retained || n%2)continue;
  slot.removing=true;CHECK(reinterpret_cast<CThreadedFile*>(slot.file)->RemoveObject(slot.uid));slot.removing=false;slot.userRemoved=true;
  CHECK(slot.frees==1 && slot.deletes==1);
  CHECK(!reinterpret_cast<CThreadedFile*>(slot.file)->ObjectExists(slot.uid) && reinterpret_cast<CThreadedFile*>(slot.file)->IsObjectFinished(slot.uid));
  CHECK(reinterpret_cast<CThreadedFile*>(slot.file)->RemoveObject(slot.uid) && slot.frees==1 && slot.deletes==1);ValidateUID(true);ValidateFinishedStatistics();
 }
 CHECK(reinterpret_cast<CThreadedFile*>(g_slots[0].file)->RemoveObject(0x7fffffffUL));ValidateFinishedStatistics();
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
 ReleaseSharedOwners(bankReleasePolicy);FinishSharedBanks();
 for(unsigned n=0;n<g_count;++n)CHECK(g_slots[n].frees==1 && g_slots[n].deletes==1);
 CHECK(!reinterpret_cast<unsigned char*>(manager)[0x148] && reinterpret_cast<unsigned char*>(manager)[0x149] && reinterpret_cast<unsigned char*>(manager)[0x14a]);
 CHECK(manager[0x164/4]==0);
 ValidateUID(true);
 CHECK(installerObject->Paused==0 && installerObject->Running && !FixtureInstallerDirty);
 CHECK(g_ackSignals==36 && g_resumeSignals==36 && g_pauseWaits==36 && g_pauseReturns==36);
 CHECK(g_idleReads && g_idleAlways);
 EnterCriticalSection(installerObject->CriticalSection);reinterpret_cast<unsigned char*>(installer)[0x7a]=1;LeaveCriticalSection(installerObject->CriticalSection);
 CHECK(SetEvent(installerEvent) && WaitForSingleObject(g_installerHandle,5000)==WAIT_OBJECT_0);
 CHECK(reinterpret_cast<unsigned char*>(installer)[0x78]==1 && !installerObject->Running && installerObject->Paused==0);
 CHECK(CloseHandle(g_installerHandle));g_installerHandle=0;
 DeleteCriticalSection(installerObject->CriticalSection);
 CHECK(CloseHandle(installerEvent) && CloseHandle(pauseStart) && CloseHandle(pauseFinish));
 g_CFileInstallerSingleton_013CA818.object=0;g_installer=0;
 delete owner;CHECK(!g_managerAllocated);for(unsigned n=0;n<g_count;++n)CHECK(g_slots[n].frees==1 && g_slots[n].deletes==1);
 CHECK(CloseHandle(thread) && CloseHandle(g_waitRelease));g_waitRelease=0;g_ownedWorker=0;g_manager=0;g_count=0;
}
static void InstallPendingSleepObserver();
static void RunPendingOwners(HANDLE,unsigned,unsigned);
int main(int argc,char** argv) {
 CHECK(argc==3);g_mainThread=GetCurrentThreadId();InstallObservers();InstallReadObserver();
 DWORD protection,restored;CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),4,&protection));
 g_realGlobalFree=FixtureGlobalFreeImport;FixtureGlobalFreeImport=ObserveGlobalFree;CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),protection,&restored));
 InstallPendingSleepObserver();
 HANDLE input=CreateFileA(argv[1],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);CHECK(input!=INVALID_HANDLE_VALUE);
 const unsigned starts[4]={1,0x7ffffff8UL,0x80000000UL,0xffffffe0UL};
 for(unsigned batch=0;batch<24;++batch)RunPendingOwners(input,starts[batch%4],batch%4);
 CHECK(CloseHandle(input));CHECK(g_pendingCancelled==288 && g_pendingJoins==24 && !g_nativeBankCompletions && !g_successAPCs && !g_successBytes);
 CHECK(g_nativeBankDataDeletes==288 && g_nativeBankInfoDeletes==288 && g_nativeBankNodeDeletes==288 && g_sharedContexts==48 && g_sharedTreeChecks==624 && g_sharedPartialDeletes==240 && g_sharedTwoChildDeletes>=12);
 CHECK(g_pendingManagerChecks==312);for(unsigned i=0;i<4;++i)CHECK(g_releaseOrders[i]==6);
 printf("NATIVE_PENDING_OWNER_PASS batches=24 owners=288 pending_removals=288 real_reads=0 joins=24 contexts=48 tree_checks=624 partial_deletes=240 two_child_deletes=%u manager_checks=312 data_deletes=288 info_deletes=288 node_deletes=288\n",g_sharedTwoChildDeletes);return 0;
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
  CHECK(*reinterpret_cast<unsigned long*>(static_cast<char*>(section)+8)==0);
  CHECK(GetCurrentThreadId()==g_mainThread);CHECK(++ctx->enters<=36);
  if(g_constructingBank && g_constructingBank->bankCtx==ctx)CHECK(static_cast<CBankFileAsyncData*>(g_constructingBank->bankDataBlock)->InternalAllocatedBufferSize==0xcdcdcdcdUL);
 }
 g_realBankEnter(section);
}
static void __stdcall ObserveBankLeave(void* section) {
 BankContext* ctx=BankForSection(section);
 if(ctx) {CHECK(GetCurrentThreadId()==g_mainThread);CHECK(++ctx->leaves<=60);}
 g_realBankLeave(section);
}
static int __stdcall ObserveBankTry(void* section) {
 CHECK(!g_pendingMode);
 BankContext* ctx=BankForSection(section);
 if(ctx) {
  CHECK(GetCurrentThreadId()==g_workerThread && ctx->tries<2*(ctx->reads+1));
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
  CHECK(g_releasingBank==&slot && GetCurrentThreadId()==g_mainThread && !g_shutdown);
  bool& deleted=data?slot.bankDataDeleted:slot.bankInfoDeleted;CHECK(!deleted);deleted=true;
  BankContext& ctx=*slot.bankCtx;CHECK(ctx.enters==ctx.constructed+ctx.completed+ctx.released+1 && ctx.leaves==ctx.constructed+ctx.completed+ctx.released+1+2*ctx.reads && ctx.tries==2*ctx.reads);
  CheckBankAllocation(block,data?72:12);if(data)++g_nativeBankDataDeletes;else ++g_nativeBankInfoDeletes;
  free(static_cast<unsigned char*>(block)-16);return true;
 }
 return false;
}
static void ReleaseBankOwner(Slot& slot) {
 BankContext& ctx=*slot.bankCtx;CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);
 CCountedPointer<CBankFileAsyncData>* owner=reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(slot.ownerWords);
 CHECK(slot.child && !slot.deletes && !slot.frees && slot.uid && owner->Data==slot.bankDataBlock && owner->Info==slot.bankInfoBlock);
 unsigned char expected[72];memcpy(expected,slot.bankExpected,72);
 CHECK(*reinterpret_cast<unsigned long*>(bank->CriticalSection+8)==0 && *reinterpret_cast<unsigned long*>(bank->CriticalSection+12)==0);
 CHECK(memcmp(expected,owner->Data,72)==0 && owner->Info->RefCount==1 && !owner->Data->Finished && owner->Data->ThreadedFileUID==slot.uid);
 CheckBankAllocation(slot.bankDataBlock,72);CheckBankAllocation(slot.bankInfoBlock,12);CheckBankAllocation(slot.bankNodeBlock,20);
 unsigned long* node=static_cast<unsigned long*>(slot.bankNodeBlock);
 if(ctx.constructed-ctx.released>1)++g_sharedPartialDeletes;if(node[2] && node[3])++g_sharedTwoChildDeletes;
 slot.cancelling=slot.removing=true;g_releasingBank=&slot;owner->~CCountedPointer<CBankFileAsyncData>();g_releasingBank=0;slot.cancelling=slot.removing=false;
 CHECK(slot.frees==1 && slot.deletes==1);
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
 CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(g_releasingBank->bankCtx->bankWords+4);CHECK(*reinterpret_cast<unsigned long*>(bank->CriticalSection+8)==1 && *reinterpret_cast<unsigned long*>(bank->CriticalSection+12)==g_mainThread);
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
  ReleaseBankOwner(g_slots[chosen]);ValidatePendingManager();
 }
}
static void FinishSharedBanks() {
 for(unsigned i=0;i<2;++i) {
  BankContext& ctx=g_banks[i];CHECK(ctx.initialized && ctx.constructed>=2 && !ctx.reads && !ctx.completed && ctx.released==ctx.constructed);
  CHECK(ctx.enters==2*ctx.constructed && ctx.leaves==2*ctx.constructed && !ctx.tries);ValidateBankTree(ctx);
  CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);DeleteCriticalSection(bank->CriticalSection);
  for(unsigned guard=0;guard<4;++guard)CHECK(ctx.bankWords[guard]==0xcdcdcdcd && ctx.bankWords[guard+4+sizeof(CBankFileAsync)/4]==0xcdcdcdcd);
  ctx.initialized=false;
 }
}

typedef void (__stdcall* PendingSleepCall)(DWORD);
extern "C" PendingSleepCall FixturePendingSleepImport;
#pragma comment(linker,"/alternatename:_FixturePendingSleepImport=__imp__Sleep@4")
static PendingSleepCall g_realPendingSleep;
static void __stdcall ObservePendingSleep(DWORD delay) {
 CHECK(!g_releasingBank);g_realPendingSleep(delay);
}
static void InstallPendingSleepObserver() {
 DWORD protection,restored;CHECK(VirtualProtect(&FixturePendingSleepImport,sizeof(FixturePendingSleepImport),4,&protection));
 g_realPendingSleep=FixturePendingSleepImport;FixturePendingSleepImport=ObservePendingSleep;
 CHECK(VirtualProtect(&FixturePendingSleepImport,sizeof(FixturePendingSleepImport),protection,&restored));
}
static bool PendingBefore(unsigned a,unsigned b) {
 Slot& lhs=g_slots[a];Slot& rhs=g_slots[b];
 if(lhs.file[5]!=rhs.file[5])return lhs.file[5]<rhs.file[5];
 if(lhs.offset!=rhs.offset)return lhs.offset<rhs.offset;
 return reinterpret_cast<unsigned>(lhs.request)<reinterpret_cast<unsigned>(rhs.request);
}
static void ValidatePendingManager() {
 EnterCriticalSection(g_manager+0x14c/4);CHECK(g_manager[0x16c/4]==1 && !g_manager[0x138/4]);
 unsigned expected[12],count=0,bytes=0;
 for(unsigned i=28;i<g_count;++i)if(!g_slots[i].deletes) {expected[count++]=i;bytes+=g_slots[i].length;CHECK(!g_slots[i].frees && g_slots[i].request->LoadingStatus==0 && memcmp(g_slots[i].requestExpected,g_slots[i].request,64)==0);for(unsigned j=0;j<20;++j)CHECK(static_cast<unsigned char*>(g_slots[i].overlap)[j]==0);}
 CHECK(g_manager[0x140/4]==count);
 for(unsigned i=1;i<count;++i) {unsigned value=expected[i],j=i;while(j && g_slots[value].uid<g_slots[expected[j-1]].uid) {expected[j]=expected[j-1];--j;}expected[j]=value;}
 Node* header=reinterpret_cast<Node*>(g_manager[0x13c/4]);g_seenCount=g_orderCount=0;Walk(header->parent,header);CHECK(g_seenCount==count && g_orderCount==count);
 for(unsigned i=0;i<count;++i)CHECK(g_order[i]->key==g_slots[expected[i]].uid && g_order[i]->value==g_slots[expected[i]].request);
 for(unsigned q=0;q<14;++q) {
  unsigned n=0,at=1+q*4;for(unsigned i=28;i<g_count;++i)if(!g_slots[i].deletes && g_slots[i].device*7+g_slots[i].priority==q)expected[n++]=i;
  CHECK(g_manager[at+1]==n);
  for(unsigned i=1;i<n;++i) {unsigned value=expected[i],j=i;while(j && PendingBefore(value,expected[j-1])) {expected[j]=expected[j-1];--j;}expected[j]=value;}
  Node* head=reinterpret_cast<Node*>(g_manager[at]);CHECK(g_manager[at+3]==reinterpret_cast<DWORD>(head));g_seenCount=g_orderCount=0;Walk(head->parent,head);CHECK(g_orderCount==n);
  for(unsigned i=0;i<n;++i)CHECK(g_order[i]->key==reinterpret_cast<unsigned>(g_slots[expected[i]].request) && g_order[i]->value==g_slots[expected[i]].request);
 }
 CThreadedFileManager* manager=reinterpret_cast<CThreadedFileManager*>(g_manager);
 CHECK(manager->GetTotalObjectsPendingCount()==long(count) && manager->GetTotalBytesPendingCount()==long(bytes) && !manager->GetTotalObjectsFinishedCount() && !manager->GetTotalBytesFinishedCount());
 ++g_pendingManagerChecks;LeaveCriticalSection(g_manager+0x14c/4);
}
static void RunPendingOwners(HANDLE input,unsigned start,unsigned policy) {
 CHECK(!g_managerAllocated && !g_manager && !g_constructingBank && !g_releasingBank);g_shutdown=false;g_count=28;g_installer=0;
 memset(g_slots,0,sizeof(g_slots));memset(g_banks,0,sizeof(g_banks));++g_releaseOrders[policy];
 g_pendingBlockedEvent=CreateEventA(0,1,0,0);CHECK(g_pendingBlockedEvent);g_pendingBlockProofs=0;
 DWORD previous[128];unsigned previousCount=Threads(previous);
 CThreadedFileManager* owner=new CThreadedFileManager;CHECK(owner==reinterpret_cast<CThreadedFileManager*>(FixtureGlobalManagerStorage));CaptureWorker(previous,previousCount);
 unsigned long* manager=reinterpret_cast<unsigned long*>(owner);EnterCriticalSection(manager+0x14c/4);g_manager=manager;
 CHECK(!manager[0x140/4] && !manager[0x138/4]);manager[0x16c/4]=1;manager[0x168/4]=start;
 for(unsigned n=0;n<12;++n) {
  Slot& slot=g_slots[28+n];slot.child=true;slot.device=n%2;slot.priority=(n%3)*2;slot.offset=0x1000+(n%6)*64;slot.length=n?17+n%7:0;
  slot.file[1]=slot.device;slot.file[2]=reinterpret_cast<DWORD>(input);slot.file[5]=n%3;memset(slot.buffer,0xcd,sizeof(slot.buffer));PrepareSharedBank(slot);
  BankContext& ctx=*slot.bankCtx;CBankFileAsyncEntry* entry=reinterpret_cast<CBankFileAsyncEntry*>(ctx.entryWords+4);
  g_constructingBank=&slot;CHECK(FixtureBankBeginRead(entry,0,slot.ownerWords,slot.offset,slot.buffer+16,slot.length,96,static_cast<EThreadedFilePriority>(slot.priority))==slot.ownerWords);g_constructingBank=0;
  CCountedPointer<CBankFileAsyncData>* result=reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(slot.ownerWords);
  CHECK(result->Data==slot.bankDataBlock && result->Info && result->Info==slot.bankInfoBlock && result->Info->RefCount==1 && result->Info->DeleteFunc && result->Info->Data==result->Data);
  CBankFileAsyncData* data=result->Data;slot.uid=data->ThreadedFileUID;CHECK(slot.uid==start+n && slot.uid && data->Length==slot.length && !data->SkipBytes && !data->Finished && data->Buffer==slot.buffer+16 && data->Data==slot.buffer+16 && !data->MemoryPool && data->MemoryHandle==-1 && !data->InternalAllocatedBufferSize);
  CThreadedFileObjectDesc desc;memset(&desc,0,sizeof(desc));desc.PDst=slot.buffer+16;desc.FilePos=slot.offset;desc.Size=slot.length;desc.PCallback=data;desc.Priority=static_cast<EThreadedFilePriority>(slot.priority);for(unsigned j=17;j<20;++j)reinterpret_cast<unsigned char*>(&desc)[j]=0xcd;
  CHECK(memcmp(&data->LoadDesc,&desc,sizeof(desc))==0);
  memset(slot.bankExpected,0xcd,72);memcpy(slot.bankExpected,data,8);
  unsigned fields[][2]={{8,reinterpret_cast<unsigned>(slot.buffer+16)},{12,reinterpret_cast<unsigned>(slot.buffer+16)},{16,slot.length},{20,0},{24,slot.uid},{28,0},{32,0xffffffffUL},{36,0},{40,reinterpret_cast<unsigned>(ctx.bankWords+4)}};
  for(unsigned field=0;field<sizeof(fields)/sizeof(fields[0]);++field)memcpy(slot.bankExpected+fields[field][0],&fields[field][1],4);
  memcpy(slot.bankExpected+0x2c,&desc,24);memset(slot.bankExpected+0x44,0,4);
  CHECK(memcmp(slot.bankExpected,data,72)==0);++ctx.constructed;
  Node* head=reinterpret_cast<Node*>(manager[0x13c/4]);Node* node=head->parent;while(node && node->key!=slot.uid)node=slot.uid<node->key?node->left:node->right;CHECK(node);
  slot.request=node->value;slot.overlap=slot.request->Overlapped;CHECK(slot.request->PFile==reinterpret_cast<CThreadedFile*>(ctx.file) && slot.request->PManager==owner && slot.request->UID==slot.uid && slot.request->LoadingStatus==0 && memcmp(&slot.request->Desc,&desc,sizeof(desc))==0);
  memcpy(slot.requestExpected,slot.request,64);++g_count;ValidateBankTree(ctx);
 }
 CHECK(ResetEvent(g_pendingBlockedEvent));g_pendingBlockProofs=0;LeaveCriticalSection(manager+0x14c/4);
 DWORD started=GetTickCount();while(WaitForSingleObject(g_pendingBlockedEvent,10)!=WAIT_OBJECT_0) {CHECK(GetTickCount()-started<5000);CHECK(SetEvent(reinterpret_cast<HANDLE>(manager[0x164/4])));}
 CHECK(g_pendingBlockProofs>=2);ValidatePendingManager();ReleaseSharedOwners(policy);FinishSharedBanks();
 for(unsigned i=28;i<g_count;++i) {CHECK(g_slots[i].deletes==1 && g_slots[i].frees==1 && !g_slots[i].apcCompletions);for(unsigned j=0;j<sizeof(g_slots[i].buffer);++j)CHECK(g_slots[i].buffer[j]==0xcd);}
 g_shutdown=true;owner->Uninitialise();CHECK(WaitForSingleObject(g_ownedWorker,5000)==WAIT_OBJECT_0);++g_pendingJoins;
 CHECK(!reinterpret_cast<unsigned char*>(manager)[0x148] && reinterpret_cast<unsigned char*>(manager)[0x149] && reinterpret_cast<unsigned char*>(manager)[0x14a] && !manager[0x140/4]);delete owner;CHECK(!g_managerAllocated);
 CHECK(CloseHandle(g_ownedWorker) && CloseHandle(g_pendingBlockedEvent));g_ownedWorker=0;g_pendingBlockedEvent=0;g_manager=0;g_count=0;
}
