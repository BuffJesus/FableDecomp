// Native bank consumer waits during PENDING_LOCKED and promotes the actual pending queue.
// Actual bank/file/manager wait; scoped owner release after completion.
#include "fable_bank_entry_pointer_outofline.h"
#include "fable_threaded_file_object.h"
#include "fable_file_installer.h"
#include "fable_bank_async_entry.h"
#include "NativeThreadedReadAPC_win32.h"
#include "NativeBankLifetimeColdAliases.h"
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "fable_filesystem.h"
#include "fable_disk_file.h"
#include <string.h>
extern "C" {
void* __fastcall FixtureFileCtor(void*,void*);
void __fastcall FixtureFileDtor(void*,void*);
void* __fastcall FixtureAsyncBankCtor(void*,void*);
void __fastcall FixtureAsyncBankDtor(void*,void*);
void* __fastcall FixtureBankGetEntry(CBankFileAsync*,void*,void*,unsigned long);
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
#pragma comment(linker,"/alternatename:@FixtureBankGetEntry@16=?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z")
#pragma comment(linker,"/alternatename:@FixtureAsyncBankCtor@8=??0CBankFileAsync@@QAE@XZ")
#pragma comment(linker,"/alternatename:@FixtureAsyncBankDtor@8=??1CBankFileAsync@@UAE@XZ")
#pragma comment(linker,"/alternatename:@FixtureFileCtor@8=??0CThreadedFile@@QAE@XZ")
#pragma comment(linker,"/alternatename:@FixtureFileDtor@8=??1CThreadedFile@@UAE@XZ")
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
static void* g_metadataBlocks[3];static unsigned g_metadataSizes[3],g_metadataFrees;
static bool g_destroyingWholeBank,g_bankSectionDeleted;static unsigned g_initializationFrees;
static void* AllocateMetadata(unsigned,unsigned);
static bool DeleteMetadata(void*);
static bool g_constructingEntry;
static void* g_entryBlock;static void* g_entryInfoBlock;static unsigned g_entryDeletes,g_entryInfoDeletes;
static void* AllocateEntryStorage(size_t);
static bool DeleteEntryStorage(void*);
static void* AllocateBankStorage(size_t);
static bool DeleteBankStorage(void*);
extern "C" void ManagerCreate(const CWideString&,unsigned long);
extern "C" void ManagerDestroy();
static unsigned g_headerCalls;static unsigned g_parsedSize=37,g_parsedOffset=0x313,g_payloadOffset=0x1000;
static bool g_openingFile;static CBankFileAsync* g_openBank;static unsigned g_diskCtorChecks,g_fileCtorChecks,g_notifyCalls;
extern "C" void __fastcall ObserveDiskDelete(void*);
extern "C" void __fastcall ActualBankNotify(CBankFile*,void*);
#pragma comment(linker,"/alternatename:@ObserveDiskDelete@4=?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@CIXPAX@Z")
#pragma comment(linker,"/alternatename:@ActualBankNotify@8=?NotifyBankOpened@CBankFile@@MAEXXZ")
static unsigned g_parsedEntries,g_parsedPost;
static unsigned long __fastcall ObserveFormat(CBankFile* bank,void*) {CHECK(bank==g_openBank && g_openingFile && !g_headerCalls && bank->OpenFlags==0x40);++g_headerCalls;return 19;}
static void __fastcall ObserveSize(CBankFile* bank,void*,unsigned long size,const void*) {CHECK(bank==g_openBank && size==4 && !g_parsedEntries);}
static void __fastcall ObserveEntry(CBankFile* bank,void*,unsigned long type,unsigned long index,const void*) {CHECK(bank==g_openBank && type==7 && index==3 && !g_parsedEntries);++g_parsedEntries;}
static void __fastcall ObservePost(CBankFile* bank,void*) {CHECK(bank==g_openBank && g_parsedEntries==1 && !g_parsedPost);++g_parsedPost;}
static void __fastcall ObserveBankNotify(CBankFile* bank,void*) {CHECK(bank==g_openBank && g_headerCalls==1 && !g_notifyCalls);++g_notifyCalls;ActualBankNotify(bank,0);}
static void* g_stringBlocks[64];static unsigned g_stringAllocations,g_stringFrees;
static wchar_t g_openPath[1024];static HANDLE g_openHandle,g_diskOpenHandle;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static HANDLE g_firstReaderHandle;static CThreadedFile* g_firstReader;static unsigned g_firstReaderCloses;
static void* AllocateString(size_t size) {
 if(size==sizeof(CThreadedFile) && g_openBank && g_fileCtorChecks==1 && g_openBank->ThreadedFile.control_) {
  CHECK(!g_firstReader && g_openBank->ThreadedFile.control_->RefCount==1);g_firstReader=g_openBank->ThreadedFile.data_;
  CHECK(g_firstReader->openFlag_ && g_firstReader->physicalSortKey_==1 && g_firstReader->length_==0x9000);g_firstReaderHandle=g_firstReader->fileHandle_;
 }

 if(size==12 && g_openBank) {
  CThreadedFile* file=g_openBank->ThreadedFile.data_;
  if(file && !g_openBank->ThreadedFile.control_) {CHECK(g_fileCtorChecks<2 && file->fileHandle_==INVALID_HANDLE_VALUE && !file->openFlag_ && !file->openedForWrite_ && !reinterpret_cast<unsigned long*>(file)[3]);++g_fileCtorChecks;}
  else {CHECK(!g_diskCtorChecks && g_openBank->BankFile.Data && !g_openBank->BankFile.Info);CHECK(!reinterpret_cast<unsigned char*>(g_openBank->BankFile.Data)[20] && !reinterpret_cast<unsigned long*>(g_openBank->BankFile.Data)[6]);++g_diskCtorChecks;}
 }
 void* p=malloc(size);CHECK(p);memset(p,0xcd,size);unsigned i=0;while(i<64 && g_stringBlocks[i])++i;CHECK(i<64);g_stringBlocks[i]=p;++g_stringAllocations;return p;
}
static bool DeleteString(void* p) {
 if(!p)return false;for(unsigned i=0;i<64;++i)if(g_stringBlocks[i]==p) {g_stringBlocks[i]=0;++g_stringFrees;free(p);return true;}return false;
}
void* operator new[](size_t size) {CHECK(g_openingFile);return AllocateString(size);}
CWideString CWideString::GetSubString(unsigned long,unsigned long) const {Fail("cold dot-path substring",__LINE__);}
CWideString __fastcall CAFile::GetAbsoluteFilePath(const CWideString&,const CWideString&) {Fail("cold parent-path normalization",__LINE__);}
void* __cdecl operator new(size_t size) {
 if(g_openingFile)return AllocateString(size);
 if(size==sizeof(CThreadedFileManager)) {
  CHECK(!g_managerAllocated);g_managerAllocated=true;
  memset(FixtureGlobalManagerStorage,0xcd,sizeof(FixtureGlobalManagerStorage));return FixtureGlobalManagerStorage;
 }
 if(g_constructingEntry && (size==16 || size==12))return AllocateEntryStorage(size);
 if(size==72 || size==12)return AllocateBankStorage(size);
 void* block=malloc(size);CHECK(block);return block;
}
struct BankContext {
 bool initialized;
 unsigned long bankWords[(sizeof(CBankFileAsync)+32)/4],entryWords[12],runtimeEntry[9];unsigned long* file;
 unsigned long* bankHeader;unsigned long* restartHeader;
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
static void CheckPendingPriority(Slot&,unsigned);
static Slot* g_constructingBank;static Slot* g_releasingBank;
static HANDLE g_readIssued,g_continueRead,g_publishedEvent,g_requestRetired;
static volatile bool g_published;static unsigned g_retries;static DWORD g_releaseStarted;
typedef void (__stdcall* PlainSleep)(DWORD);
extern "C" PlainSleep FixturePlainSleep;
#pragma comment(linker,"/alternatename:_FixturePlainSleep=__imp__Sleep@4")
static PlainSleep g_realPlainSleep;
static void __stdcall ObservePlainSleep(DWORD delay) {
 CHECK(!g_releasingBank);
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
 if(DeleteString(block))return;
 if(DeleteMetadata(block))return;
 if(DeleteEntryStorage(block))return;
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
 if(GetCurrentThreadId()==g_mainThread) {
  CHECK(g_releasingBank && delay==0 && alertable && ++g_waitSleeps==1);
  Slot& slot=*g_releasingBank;
  CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(slot.bankCtx->bankWords+4);
  CHECK(!*reinterpret_cast<DWORD*>(bank->CriticalSection+8) && !*reinterpret_cast<DWORD*>(bank->CriticalSection+12));
  CHECK(g_manager[(0x14c+12)/4]!=g_mainThread && slot.request->LoadingStatus==1 && slot.request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING);
  CHECK(!slot.bankNodeDeleted && !slot.bankDataDeleted && !slot.bankInfoDeleted && !slot.frees && !slot.deletes);
  CHECK(slot.bankCtx->enters==5 && slot.bankCtx->leaves==5);CheckPendingPriority(slot,6);
  CHECK(SetEvent(g_continueRead));CHECK(WaitForSingleObject(g_requestRetired,5000)==WAIT_OBJECT_0);CHECK(g_published);
 }
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
typedef BOOL (__stdcall* CloseFunction)(HANDLE);
extern "C" CloseFunction FixtureCloseImport;
#pragma comment(linker,"/alternatename:_FixtureCloseImport=__imp__CloseHandle@4")
static CloseFunction g_realClose;
static BOOL __stdcall ObserveClose(HANDLE handle) {
 bool replacing=g_openingFile && g_firstReader && !g_firstReaderCloses && handle==g_firstReaderHandle;
 if(replacing)CHECK(g_openBank && g_fileCtorChecks==1 && g_firstReader->openFlag_ && g_firstReader->fileHandle_==handle);
 BOOL result=g_realClose(handle);if(replacing){CHECK(result);++g_firstReaderCloses;}return result;
}
static void InstallObservers() {
 InstallBankLockObservers();
 DWORD protection,restored;
 CHECK(VirtualProtect(&FixtureCloseImport,sizeof(FixtureCloseImport),4,&protection));g_realClose=FixtureCloseImport;FixtureCloseImport=ObserveClose;CHECK(VirtualProtect(&FixtureCloseImport,sizeof(FixtureCloseImport),protection,&restored));
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
 if(g_slots[i].child) {BankContext& ctx=*g_slots[i].bankCtx;CHECK(ctx.enters==5 && ctx.leaves==7 && ctx.tries==2 && !ctx.reads);++ctx.reads;}
 CHECK(buffer==g_slots[i].buffer+16 && bytes==g_slots[i].length && file==reinterpret_cast<HANDLE>(g_slots[i].file[2]));
 BOOL result=g_realRead(file,buffer,bytes,overlap,ObserveAPC);CHECK(result);
 CHECK(g_slots[i].child && !g_published && !g_retries);CHECK(SetEvent(g_readIssued));
 CHECK(WaitForSingleObject(g_continueRead,5000)==WAIT_OBJECT_0);return result;
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
static unsigned long g_factoryIndex=3;
static void RunActive(HANDLE input);
int main(int argc,char** argv) {

 CHECK(argc==3 || argc==4);unsigned variant=argc==4?strtoul(argv[3],0,10):0;CHECK(variant<3);g_parsedSize=37+8*variant;g_parsedOffset=0x313+0x100*variant;g_payloadOffset=0x1000+0x80*variant;g_mainThread=GetCurrentThreadId();InstallObservers();InstallReadObserver();
 DWORD protection,restored;
 CHECK(VirtualProtect(&FixturePlainSleep,sizeof(FixturePlainSleep),4,&protection));g_realPlainSleep=FixturePlainSleep;FixturePlainSleep=ObservePlainSleep;CHECK(VirtualProtect(&FixturePlainSleep,sizeof(FixturePlainSleep),protection,&restored));
 CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),4,&protection));g_realGlobalFree=FixtureGlobalFreeImport;FixtureGlobalFreeImport=ObserveGlobalFree;CHECK(VirtualProtect(&FixtureGlobalFreeImport,sizeof(FixtureGlobalFreeImport),protection,&restored));
 CHECK(mbstowcs(g_openPath,argv[1],1023)!=size_t(-1));HANDLE input=INVALID_HANDLE_VALUE;
 RunActive(input);CHECK(!CloseHandle(g_openHandle) && GetLastError()==6);CHECK(!CloseHandle(g_diskOpenHandle) && GetLastError()==6);{CWideString empty;g_CAFileCurrentPath_013BCA10=empty;}CHECK(g_stringAllocations==g_stringFrees && g_CWideStringInstanceCount_013BCA20==1);return 0;
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
static bool g_closingNativeFile;static unsigned g_fileCloseEnters,g_fileCloseLeaves;
static void __stdcall ObserveBankEnter(void* section) {
 if(g_closingNativeFile && GetCurrentThreadId()==g_mainThread && section==g_manager+0x14c/4) {CHECK(reinterpret_cast<unsigned char*>(g_manager)[0x148] && !g_manager[0x16c/4]);++g_fileCloseEnters;}
 BankContext* ctx=BankForSection(section);
 if(ctx) {
  CHECK(GetCurrentThreadId()==g_mainThread || GetCurrentThreadId()==g_workerThread);CHECK(++ctx->enters<=11);
  if(GetCurrentThreadId()==g_workerThread)CHECK(g_releasingBank && g_waitSleeps==1 && g_manager[(0x14c+12)/4]!=g_workerThread);
 }
 g_realBankEnter(section);
}
static void __stdcall ObserveBankLeave(void* section) {
 if(g_closingNativeFile && GetCurrentThreadId()==g_mainThread && section==g_manager+0x14c/4) {CHECK(!g_manager[0x16c/4]);++g_fileCloseLeaves;}
 BankContext* ctx=BankForSection(section);bool publish=false;
 if(ctx) {
  CHECK(++ctx->leaves<=13);
  if(GetCurrentThreadId()==g_workerThread && g_releasingBank && !g_published && g_releasingBank->request->LoadingStatus==3) {
   Slot& slot=*g_releasingBank;CBankFileAsyncData* data=static_cast<CBankFileAsyncData*>(slot.bankDataBlock);
   CHECK(slot.request->LoadingStatus==3 && slot.apcCompletions==1 && !slot.bankDataDeleted && !slot.bankNodeDeleted);
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
   CHECK(g_slots[28].request->LoadingStatus==1 && !g_waitSleeps);
   CHECK(SetEvent(g_readIssued));CHECK(WaitForSingleObject(g_continueRead,5000)==WAIT_OBJECT_0);
   CHECK(g_waitSleeps==1 && g_slots[28].request->Desc.Priority==THREADED_FILE_PRIORITY_BLOCKING);
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
static void* AllocateMetadata(unsigned index,unsigned size) {
 CHECK(index<3 && !g_metadataBlocks[index] && !g_shutdown && GetCurrentThreadId()==g_mainThread);
 unsigned char* raw=static_cast<unsigned char*>(malloc(size+32));CHECK(raw);memset(raw,0xcd,size+32);memset(raw+16,0,size);
 g_metadataBlocks[index]=raw+16;g_metadataSizes[index]=size;return raw+16;
}
static bool DeleteMetadata(void* block) {
 if(!block)return false;
 unsigned index=0;while(index<3 && g_metadataBlocks[index]!=block)++index;if(index==3)return false;
 const unsigned order[]={0};
 CHECK(g_destroyingWholeBank && g_bankSectionDeleted && g_shutdown && g_metadataFrees<1 && index==order[g_metadataFrees] && g_initializationFrees==3);
 CheckBankAllocation(block,g_metadataSizes[index]);++g_metadataFrees;free(static_cast<unsigned char*>(block)-16);return true;
}
extern "C" void* __cdecl FixtureParseMalloc(size_t size) {
 if(g_openingFile) {if(size==48 && !g_metadataBlocks[0])return AllocateMetadata(0,size);return AllocateString(size);}
 return malloc(size);
}
extern "C" void __cdecl FixtureParseFree(void* block) {
 if(!block || DeleteString(block) || DeleteMetadata(block))return;
 free(block);
}
void operator delete[](void* block) {if(!block || DeleteString(block))return;CHECK(DeleteMetadata(block));}
static void* AllocateEntryStorage(size_t size) {
 CHECK(GetCurrentThreadId()==g_mainThread && g_constructingEntry);
 void*& block=size==16?g_entryBlock:g_entryInfoBlock;CHECK(!block);
 unsigned char* raw=static_cast<unsigned char*>(malloc(size+32));CHECK(raw);memset(raw,0xcd,size+32);block=raw+16;return block;
}
static bool DeleteEntryStorage(void* block) {
 bool data=block==g_entryBlock,info=block==g_entryInfoBlock;if(!data && !info)return false;
 CHECK(GetCurrentThreadId()==g_mainThread && !g_shutdown && !g_constructingEntry && g_published);
 CHECK(g_slots[28].bankDataDeleted && g_slots[28].bankInfoDeleted && g_slots[28].bankNodeDeleted);
 if(data)CHECK(++g_entryDeletes==1 && !g_entryInfoDeletes);else CHECK(++g_entryInfoDeletes==1 && g_entryDeletes==1);
 CheckBankAllocation(block,data?16:12);free(static_cast<unsigned char*>(block)-16);return true;
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
  CHECK(g_releasingBank==&slot && GetCurrentThreadId()==g_mainThread && !g_shutdown && g_published && !g_retries && g_waitSleeps==1 && slot.frees==1 && slot.deletes==1);
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

int g_CWideStringInstanceCount_013BCA20;
int g_CCharStringInstanceCount_013BD800;
static CBankFileAsync* g_initializingBank;
static void* g_initializationBlocks[7];static unsigned g_initializationSizes[7],g_initializationCount;
extern "C" void* __cdecl FixtureBankInitMalloc(size_t size) {
 const unsigned sizes[]={24,24,24,52,20,20,16};
 CHECK(g_initializingBank && g_initializationCount<7 && size==sizes[g_initializationCount] && GetCurrentThreadId()==g_mainThread);
 CHECK(reinterpret_cast<unsigned long*>(g_initializingBank)[0x140/4]==0xcdcdcdcd && reinterpret_cast<unsigned char*>(g_initializingBank)[0x148]==0xcd && reinterpret_cast<unsigned char*>(g_initializingBank)[0x149]==0xcd);
 unsigned char* raw=static_cast<unsigned char*>(malloc(size+32));CHECK(raw);memset(raw,0xcd,size+32);
 g_initializationBlocks[g_initializationCount]=raw+16;g_initializationSizes[g_initializationCount]=size;++g_initializationCount;return raw+16;
}
static void __stdcall ObserveBankSectionDelete(void* section) {
 CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(g_banks[1].bankWords+4);
 CHECK(g_destroyingWholeBank && g_shutdown && !g_bankSectionDeleted && !g_initializationFrees && section==bank->CriticalSection);
 CHECK(!reinterpret_cast<unsigned long*>(section)[2] && !reinterpret_cast<unsigned long*>(section)[3]);
 DeleteCriticalSection(section);g_bankSectionDeleted=true;
}
extern "C" {void (__stdcall* FixtureDeleteSectionPointer)(void*)=ObserveBankSectionDelete;}
#pragma comment(linker,"/alternatename:__imp__FixtureDeleteCriticalSection@4=_FixtureDeleteSectionPointer")
extern "C" void __cdecl FixtureBankInitFree(void* block) {
 if(!block)return;
 if(DeleteString(block))return;
 if(DeleteMetadata(block))return;
 CHECK(g_destroyingWholeBank && g_bankSectionDeleted && g_initializationFrees<7 && GetCurrentThreadId()==g_mainThread);
 unsigned index=6-g_initializationFrees;CHECK(block==g_initializationBlocks[index]);
 if(index<=3)CHECK(g_metadataFrees==1);
 CheckBankAllocation(block,g_initializationSizes[index]);++g_initializationFrees;free(static_cast<unsigned char*>(block)-16);
}
static void PrepareSharedBank(Slot& slot) {
 CHECK(slot.device<2);BankContext& ctx=g_banks[slot.device];slot.bankCtx=&ctx;
 if(!ctx.initialized) {
  CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);
  CBankFileAsyncEntry* entry=reinterpret_cast<CBankFileAsyncEntry*>(ctx.entryWords+4);
  for(unsigned guard=0;guard<4;++guard) {ctx.bankWords[guard]=0xcdcdcdcd;ctx.bankWords[guard+4+sizeof(CBankFileAsync)/4]=0xcdcdcdcd;}
  reinterpret_cast<unsigned long*>(bank)[0x140/4]=0xcdcdcdcd;reinterpret_cast<unsigned char*>(bank)[0x148]=reinterpret_cast<unsigned char*>(bank)[0x149]=0xcd;
  g_initializingBank=bank;CHECK(FixtureAsyncBankCtor(bank,0)==bank);g_initializingBank=0;
  CHECK(g_initializationCount==7 && !bank->LastRestartingData && !bank->NonCached && !bank->DisableAsync && bank->Alignment==1 && bank->BankFormatVersion==0x73);
  CHECK(reinterpret_cast<unsigned long*>(bank)[0x108/4]==0x10000);
  for(unsigned packed=0x38;packed<=0x68;packed+=0x10)CHECK(!reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(bank)+packed)[0] && !reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(bank)+packed)[1] && !reinterpret_cast<unsigned char*>(bank)[packed+8] && !reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(bank)+packed)[3]);
  ctx.bankHeader=*reinterpret_cast<unsigned long**>(reinterpret_cast<char*>(bank)+0x128);ctx.restartHeader=*reinterpret_cast<unsigned long**>(reinterpret_cast<char*>(bank)+0x134);
  CHECK(ctx.bankHeader==g_initializationBlocks[4] && ctx.restartHeader==g_initializationBlocks[5]);
  g_openingFile=true;g_openBank=bank;
  unsigned long* originalVtable=*reinterpret_cast<unsigned long**>(bank);unsigned long observedVtable[20];memcpy(observedVtable,originalVtable,sizeof(observedVtable));observedVtable[8]=reinterpret_cast<unsigned long>(ObserveFormat);observedVtable[10]=reinterpret_cast<unsigned long>(ObserveSize);observedVtable[12]=reinterpret_cast<unsigned long>(ObserveEntry);observedVtable[15]=reinterpret_cast<unsigned long>(ObservePost);observedVtable[11]=reinterpret_cast<unsigned long>(ObserveBankNotify);*reinterpret_cast<unsigned long**>(bank)=observedVtable;
  {CWideString name(g_openPath);ManagerCreate(name,0x40);CCharString handle("owned-native-bank");CHECK(bank->CBankFileAsync::OpenReadOnly(handle,0x40));CHECK(bank->BankHandle==handle);ManagerDestroy();}
  *reinterpret_cast<unsigned long**>(bank)=originalVtable;
  CHECK(g_diskCtorChecks==1 && g_fileCtorChecks==2 && g_firstReaderCloses==1 && g_notifyCalls==1);g_openingFile=false;g_openBank=0;
  CHECK(g_headerCalls==1 && g_parsedEntries==1 && g_parsedPost==1 && bank->ReadOnly && bank->OpenFlags==0x40 && !bank->DisableAsync && bank->BankFile.Data && bank->BankFile.Info && bank->BankFile.Info->RefCount==1);
  ctx.file=reinterpret_cast<unsigned long*>(bank->ThreadedFile.data_);CHECK(ctx.file && bank->ThreadedFile.control_ && bank->ThreadedFile.control_->RefCount==1 && bank->ThreadedFile.control_->Data==bank->ThreadedFile.data_ && bank->ThreadedFile.control_->DeleteFunc==CCountedPointer<CThreadedFile>::DeleteData);
  CThreadedFile* nativeFile=bank->ThreadedFile.data_;
  CHECK(wcscmp(static_cast<const wchar_t*>(nativeFile->filenameStorage_),g_openPath)==0);
  g_openHandle=nativeFile->fileHandle_;g_diskOpenHandle=reinterpret_cast<HANDLE*>(bank->BankFile.Data)[4];slot.file[2]=reinterpret_cast<DWORD>(g_openHandle);
  CHECK(nativeFile->deviceId_==1 && nativeFile->length_==0x9000 && nativeFile->physicalSortKey_==2 && nativeFile->openFlag_ && !nativeFile->openedForWrite_);
  CHECK(bank->Size==4 && bank->Alignment==8 && bank->FileValid);
  unsigned long* runtime=*reinterpret_cast<unsigned long**>(reinterpret_cast<char*>(bank)+0x20);
  CHECK(runtime==g_metadataBlocks[0] && runtime[9]==g_parsedOffset && runtime[10]==g_parsedSize && (runtime[11]&0xffff)==0x107);
  CHECK(!bank->PackedDataOffset.Size);ctx.runtimeEntry[0]=0;
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
 CHECK(slot.request->LoadingStatus==1 && unsigned(slot.request->Desc.Priority)==priority);
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
 Slot& slot=g_slots[28];slot.child=true;slot.device=1;slot.priority=3;slot.offset=g_payloadOffset;slot.length=g_parsedSize;
 slot.file[1]=1;slot.file[2]=reinterpret_cast<DWORD>(input);memset(slot.buffer,0xcd,sizeof(slot.buffer));PrepareSharedBank(slot);
 BankContext& ctx=*slot.bankCtx;
 CBankFileAsync* factoryBank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);
 unsigned long entryOffset=g_parsedOffset;
 CHECK(factoryBank->Size==4 && factoryBank->Alignment==8 && factoryBank->GetEntryDataSize(3)==g_parsedSize && factoryBank->GetEntryDataOffset(3)==entryOffset);
 unsigned long entryOwnerWords[2]={0,0};
 CCountedPointer<CBankFileAsyncEntry>* entryOwner=reinterpret_cast<CCountedPointer<CBankFileAsyncEntry>*>(entryOwnerWords);
 g_constructingEntry=true;CHECK(FixtureBankGetEntry(factoryBank,0,entryOwnerWords,g_factoryIndex)==entryOwnerWords);g_constructingEntry=false;
 CBankFileAsyncEntry* entry=entryOwner->Data;
 CHECK(entry && entry==g_entryBlock && entryOwner->Info && entryOwner->Info==g_entryInfoBlock);
 CHECK(entryOwner->Info->RefCount==1 && entryOwner->Info->Data==entry && entryOwner->Info->DeleteFunc);
 CHECK(&entry->BankFile==factoryBank && entry->Index==g_factoryIndex && entry->PaddedSize==((g_parsedSize+7)&~7U));
 unsigned char entryBefore[16];memcpy(entryBefore,entry,16);
 CheckBankAllocation(entry,16);CheckBankAllocation(entryOwner->Info,12);
 g_constructingBank=&slot;CHECK(FixtureBankBeginRead(entry,0,slot.ownerWords,slot.offset-entryOffset,slot.buffer+16,slot.length,96,THREADED_FILE_PRIORITY_NORMAL)==slot.ownerWords);g_constructingBank=0;
 CCountedPointer<CBankFileAsyncData>* owner=reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(slot.ownerWords);
 CHECK(owner->Data && owner->Info && owner->Data==slot.bankDataBlock && owner->Info==slot.bankInfoBlock && owner->Info->RefCount==1 && owner->Data->ThreadedFileUID==1 && !owner->Data->Finished);
 CHECK(owner->Info->Data==owner->Data && owner->Info->DeleteFunc && owner->Data->Length==slot.length && owner->Data->Buffer==slot.buffer+16 && owner->Data->Data==slot.buffer+16 && !owner->Data->Aborted && !owner->Data->AllowRestart && owner->Data->MemoryHandle==-1 && !owner->Data->InternalAllocatedBufferSize);
 memcpy(slot.bankExpected,owner->Data,72);slot.uid=1;Node* head=reinterpret_cast<Node*>(g_manager[0x13c/4]);CHECK(head->parent && head->parent->key==1);slot.request=head->parent->value;slot.overlap=slot.request->Overlapped;
 CHECK(slot.request->LoadingStatus==0 && slot.request->Desc.PCallback==owner->Data);
 CHECK(slot.request->Desc.FilePos==slot.offset && slot.request->Desc.Size==slot.length);slot.request->Desc.PDst=0;++ctx.constructed;++g_count;ValidateBankTree(ctx);LeaveCriticalSection(g_manager+0x14c/4);
 CHECK(WaitForSingleObject(g_readIssued,5000)==WAIT_OBJECT_0);
 CHECK(slot.request->LoadingStatus==1 && !slot.frees && !slot.deletes && !slot.bankNodeDeleted && !slot.bankDataDeleted && !slot.bankInfoDeleted);
 CHECK(!owner->Data->IsFinished());CHECK(memcmp(slot.bankExpected,owner->Data,72)==0);
 CHECK(g_manager[(0x14c+12)/4]!=g_mainThread);g_releasingBank=&slot;g_waitStarted=GetTickCount();
 CHECK(owner->Data->WaitUntilFinished());CHECK(g_waitSleeps==1 && g_published && slot.frees==1 && slot.deletes==1);
 CHECK(owner->Data->IsFinished());CHECK(owner->Data->GetData()==slot.buffer+16);
 unsigned char completed[72];memcpy(completed,slot.bankExpected,72);memset(completed+0x18,0,4);completed[0x44]=1;
 CHECK(memcmp(completed,owner->Data,72)==0 && owner->Info->RefCount==1);
 CHECK(owner->Data->WaitUntilFinished());CHECK(g_waitSleeps==1 && memcmp(completed,owner->Data,72)==0);
 EnterCriticalSection(g_manager+0x14c/4);CHECK(!g_manager[0x140/4] && !g_manager[0x138/4]);LeaveCriticalSection(g_manager+0x14c/4);
 CHECK(!slot.bankNodeDeleted && !slot.bankDataDeleted && !slot.bankInfoDeleted);
 owner->~CCountedPointer<CBankFileAsyncData>();g_releasingBank=0;++ctx.released;
 CHECK(!g_retries && slot.bankNodeDeleted && slot.bankDataDeleted && slot.bankInfoDeleted && !owner->Data && !owner->Info);

 CHECK(memcmp(entryBefore,entry,16)==0 && entryOwner->Info->RefCount==1);
 entryOwner->~CCountedPointer<CBankFileAsyncEntry>();
 CHECK(!entryOwner->Data && !entryOwner->Info && g_entryDeletes==1 && g_entryInfoDeletes==1);
 int fileWideBefore=g_CWideStringInstanceCount_013BCA20;
 CHECK(!g_manager[0x140/4] && !g_manager[0x16c/4]);
 g_closingNativeFile=true;
 reinterpret_cast<CThreadedFile*>(ctx.file)->Close();
 CHECK(ctx.file[2]==0xffffffff && !reinterpret_cast<CThreadedFile*>(ctx.file)->openFlag_);
 reinterpret_cast<CThreadedFile*>(ctx.file)->Close(); // Already-closed fast path.
 factoryBank->ThreadedFile.Reset(0);ctx.file=0;g_closingNativeFile=false;CHECK(g_fileCloseEnters==1 && g_fileCloseLeaves==1);
 CHECK(g_CWideStringInstanceCount_013BCA20==fileWideBefore-1 && !factoryBank->ThreadedFile.data_ && !factoryBank->ThreadedFile.control_ && !g_manager[0x16c/4]);
 g_shutdown=true;manager->Uninitialise();CHECK(WaitForSingleObject(g_ownedWorker,5000)==WAIT_OBJECT_0);
 CHECK(slot.frees==1 && slot.deletes==1 && ctx.completed==1 && g_nativeBankCompletions==1 && g_successAPCs==1 && g_successBytes==g_parsedSize);
 CHECK(ctx.enters==10 && ctx.leaves==12 && ctx.tries==2 && ctx.reads==1);ValidateBankTree(ctx);
 for(unsigned i=0;i<sizeof(slot.buffer);++i)CHECK(slot.buffer[i]==(i>=16 && i<16+g_parsedSize?Payload(slot.offset+i-16):0xcd));
 CBankFileAsync* bank=reinterpret_cast<CBankFileAsync*>(ctx.bankWords+4);
 g_destroyingWholeBank=true;FixtureAsyncBankDtor(bank,0);g_destroyingWholeBank=false;
 CHECK(g_bankSectionDeleted && g_initializationFrees==7 && g_metadataFrees==1 && g_CWideStringInstanceCount_013BCA20==1 && !g_CCharStringInstanceCount_013BD800 && !bank->Size);
 CHECK(ctx.enters==11 && ctx.leaves==13 && ctx.tries==2);
 for(unsigned guard=0;guard<4;++guard)CHECK(ctx.bankWords[guard]==0xcdcdcdcd && ctx.bankWords[guard+4+sizeof(CBankFileAsync)/4]==0xcdcdcdcd);
 ctx.initialized=false;
 CHECK(g_ackSignals==1 && g_resumeSignals==1 && g_pauseWaits==1 && g_pauseReturns==1 && !installerObject->Paused);
 EnterCriticalSection(installerObject->CriticalSection);reinterpret_cast<unsigned char*>(installer)[0x7a]=1;LeaveCriticalSection(installerObject->CriticalSection);CHECK(SetEvent(installerEvent) && WaitForSingleObject(g_installerHandle,5000)==WAIT_OBJECT_0);
 CHECK(CloseHandle(g_installerHandle));DeleteCriticalSection(installerObject->CriticalSection);CHECK(CloseHandle(installerEvent) && CloseHandle(pauseStart) && CloseHandle(pauseFinish));g_CFileInstallerSingleton_013CA818.object=0;g_installer=0;
 delete manager;CHECK(!g_managerAllocated && CloseHandle(g_ownedWorker) && CloseHandle(g_waitRelease) && CloseHandle(g_readIssued) && CloseHandle(g_continueRead) && CloseHandle(g_publishedEvent) && CloseHandle(g_requestRetired));
 CHECK(g_nativeBankDataDeletes==1 && g_nativeBankInfoDeletes==1 && g_nativeBankNodeDeletes==1);
 printf("FACTORY_METADATA index=%lu\n",g_factoryIndex);
 printf("NATIVE_BANK_ASYNC_HANDLE_HOST_PASS native_files=2 native_file_destructions=2 native_banks=1 native_bank_destructions=1 metadata_frees=1 native_entries=1 entry_releases=1 blocking_queue_moves=1 bank_waits=2 finished_queries=2 data_queries=1 real_read=1 bytes=%u retries=0 publication_before_free=1 request_retired=1 data_deleted=1 info_deleted=1 node_deleted=1 worker_joined=1 installer_joined=1\n",g_parsedSize);
}

// Inactive disk virtual slots and write/error-only helpers fail if reached.
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@ABV0@0@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@ABV0@PBD@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@PBDABV0@@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?Clone@CDiskFileWin32@@UBEPAVCAFile@@XZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?GFGetLastWindowsError@@YI?AVCCharString@@XZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?GFIntToCharString@@YI?AVCCharString@@J@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?GetPosition@CDiskFileWin32@@UBEJXZ=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?MovePosition@CDiskFileWin32@@UAEXJ@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?SetLength@CDiskFileWin32@@UAEXJ@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?SetPosition@CDiskFileWin32@@UAEXJ@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?ValidatePathnameIsOnWriteableDrive@CAFile@@SIXABVCWideString@@@Z=_UnusedBankBoundary")
#pragma comment(linker,"/alternatename:?Write@CDiskFileWin32@@UAEXPBXJPAVCASuspendableProcess@@@Z=_UnusedBankBoundary")

#pragma comment(linker,"/alternatename:_ColdBoundary=_UnusedBankBoundary")
#include "NativeFileInputStreamColdAliases.h"
#pragma comment(linker,"/alternatename:?ReadRecoveryFile@CBankFile@@AAE_NK@Z=_UnusedBankBoundary")
const CCharString& CCharString::operator=(const char*) {UnusedBankBoundary();return *this;}
CCharString CCharString::GetSubString(unsigned long,unsigned long) const {UnusedBankBoundary();return *this;}
long CCharString::FindCharPos(char) const {UnusedBankBoundary();return 0;}
