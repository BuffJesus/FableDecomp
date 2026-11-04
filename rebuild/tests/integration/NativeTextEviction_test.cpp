#include "fable_threaded_file_overlapped.h"
#include <wchar.h>
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
#include "fable_data_input_stream.h"
#include "fable_crc.h"
extern CBankFileManager g_FableBankFileManager_013CA79C;
extern CWideString g_CAFileCurrentPath_013BCA10;
// Cold random-relocation test seed; the random stress entry point is not invoked.
unsigned long g_FableHandlePoolTestSeed_013CB504=0;
unsigned long g_NativeTextReadContext_0138DE48=0x12345678;
unsigned long g_NativeTextReadContext_0138E064=0x87654321;
extern "C" void* _ReturnAddress();
#pragma intrinsic(_ReturnAddress)
extern "C" void ColdBoundary(){fprintf(stderr,"TEXT_PAYLOAD_COLD return=%p\n",_ReturnAddress());exit(80);}
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static void require(bool ok,int line){if(!ok){fprintf(stderr,"TEXT_PAYLOAD_FAILURE line=%d\n",line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
static bool observe;static unsigned observed;
class ObservedMemory:public CMemoryDataInputStream {
public:
 ObservedMemory(const void* p,unsigned long size):CMemoryDataInputStream(p,size){}
 virtual CCharString ReadNullTerminatedString(){CHECK(!observe || (g_NativeTextReadContext_0138DE48==11 && g_NativeTextReadContext_0138E064==11));++observed;return CMemoryDataInputStream::ReadNullTerminatedString();}
 virtual CWideString ReadNullTerminatedWString(){CHECK(!observe || (g_NativeTextReadContext_0138DE48==11 && g_NativeTextReadContext_0138E064==11));++observed;return CMemoryDataInputStream::ReadNullTerminatedWString();}
};
static unsigned long StringCRC(const CCharString& text){return FableCRC_Calc_004014A0(0,(const char*)text,text.GetLength());}

#include "fable_bank_async_entry.h"
#include "fable_counted_pointer_inline.h"


#include "fable_memory_allocator.h"
#include "fable_critical_section_lock.h"
#include "fable_handle_memory_pool.h"
#include "fable_loading_memory_failure_handler.h"
#include "fable_date_and_time.h"
#include "fable_file_information.h"
extern EThreadedFilePriority g_NativeTextLoadingPriority_01396314;
extern "C" void __fastcall BeginNativeGroup(const NGameText::CDataBankGroupEntry*,void*,unsigned long,NGameText::CDataBank&);
extern "C" void __fastcall BeginNativeText(const NGameText::CDataBankTextEntry*,void*,unsigned long,NGameText::CDataBank&);
extern "C" void __fastcall BuildNativeGroup(const NGameText::CDataBankGroupEntry*,void*);
extern "C" void __fastcall BuildNativeText(const NGameText::CDataBankTextEntry*,void*);
#pragma comment(linker,"/alternatename:@BeginNativeGroup@16=?BeginLoading@CDataBankGroupEntry@NGameText@@ABEXKAAVCDataBank@2@@Z")
#pragma comment(linker,"/alternatename:@BeginNativeText@16=?BeginLoading@CDataBankTextEntry@NGameText@@ABEXKAAVCDataBank@2@@Z")
#pragma comment(linker,"/alternatename:@BuildNativeGroup@8=?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ")
#pragma comment(linker,"/alternatename:@BuildNativeText@8=?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ")

extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
// The request wait may observe worker completion before its bank callback has
// published Finished/UID. Observe publication through the native accessor.
static bool WaitPublished(CBankFileAsyncData& data){
 if(!data.WaitUntilFinished())return false;unsigned long start=GetTickCount();
 while(!data.IsFinished()){if(GetTickCount()-start>=5000)return false;Sleep(0);}return true;
}

extern "C" bool __fastcall ForceNativeBuffer(CBankFileAsyncData*,void*,bool);
#pragma comment(linker,"/alternatename:@ForceNativeBuffer@12=?SetupBuffer@CBankFileAsyncData@@AAE_N_N@Z")
static bool WaitRestartable(CBankFileAsyncData& data){
 if(!data.WaitUntilFinished())return false;unsigned long start=GetTickCount();
 for(;;){CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(data.BankFile.CriticalSection),false);if(data.Finished)return data.AllowRestart;lock.Release();if(GetTickCount()-start>=5000)return false;Sleep(0);}
}

// Forward the real OS read, then hold the worker outside alertable waits so
// completion cannot publish until the main thread has checked disposal ownership.
struct _OVERLAPPED;
typedef void (__stdcall* NativeReadCompletion)(unsigned long,unsigned long,_OVERLAPPED*);
extern "C" {
 __declspec(dllimport) int __stdcall ReadFileEx(void*,void*,unsigned long,_OVERLAPPED*,NativeReadCompletion);
 __declspec(dllimport) void* __stdcall CreateEventA(void*,int,int,const char*);
 __declspec(dllimport) int __stdcall SetEvent(void*);
 __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void*,unsigned long);
 __declspec(dllimport) int __stdcall CloseHandle(void*);
}
static void* ReadIssued;static void* ReleaseRead;static volatile bool PauseRead;
// Capture the original x86 tie-break key while the request object is live.
// Keep only its integer value; never dereference it after completion.
struct ReadTraceEntry {unsigned Priority,Offset,ObjectAddress;void* Handle;};static ReadTraceEntry ReadTrace[8];static volatile unsigned ReadTraceCount;static volatile bool TraceReads;static CThreadedFileObjectDesc* ActiveReadDesc;

static void* PublishedReadIssued;static void* ReleasePublishedRead;
static bool PhaseReadMode;static NativeReadCompletion OriginalPhaseCompletion;static unsigned PublishedReadStatus;
static void __stdcall ForwardPublishedRead(unsigned long error,unsigned long count,_OVERLAPPED* overlap){
 CHECK(OriginalPhaseCompletion);OriginalPhaseCompletion(error,count,overlap);PublishedReadStatus=*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(overlap->hEvent)+0x1c);CHECK(PublishedReadStatus==3);CHECK(SetEvent(PublishedReadIssued) && WaitForSingleObject(ReleasePublishedRead,5000)==0);
}

static int __stdcall ObserveNativeRead(void* file,void* buffer,unsigned long size,_OVERLAPPED* overlap,NativeReadCompletion completion){
 NativeReadCompletion actualCompletion=completion;if(PhaseReadMode){CHECK(!OriginalPhaseCompletion);OriginalPhaseCompletion=completion;actualCompletion=ForwardPublishedRead;}int result=ReadFileEx(file,buffer,size,overlap,actualCompletion);
 if(TraceReads){CHECK(result && ReadTraceCount<8);unsigned n=ReadTraceCount;CThreadedFileObjectDesc* desc=reinterpret_cast<CThreadedFileObjectDesc*>(overlap->hEvent);ReadTrace[n].Priority=desc->Priority;ReadTrace[n].Offset=overlap->Offset;ReadTrace[n].Handle=file;ReadTrace[n].ObjectAddress=reinterpret_cast<unsigned long>(overlap->hEvent);if(!n)ActiveReadDesc=desc;ReadTraceCount=n+1;}
 if(PauseRead){CHECK(result);PauseRead=false;CHECK(SetEvent(ReadIssued));CHECK(WaitForSingleObject(ReleaseRead,5000)==0);}
 return result;
}
typedef int (__stdcall* NativeReadAPI)(void*,void*,unsigned long,_OVERLAPPED*,NativeReadCompletion);
extern "C" NativeReadAPI FixtureReadFileExSlot=ObserveNativeRead;
#pragma comment(linker,"/alternatename:__imp__FixtureReadFileEx@20=_FixtureReadFileExSlot")

// A typed policy leaf for bounded null-allocation tests. No OS exhaustion and
// no invalid read submission: routing/accounting and SetupBuffer remain native.
class NullAllocator:public CMemoryAllocatorBase {
public:
 unsigned Expected,Calls;
 NullAllocator():CMemoryAllocatorBase(16),Expected(0),Calls(0){}
 virtual void* Allocate(unsigned long size){CHECK(size==Expected && ++Calls==1);return 0;}
 virtual void Deallocate(void*){ColdBoundary();}
 virtual unsigned long GetAllAllocatedAreas(void**,unsigned long){ColdBoundary();return 0;}
 virtual unsigned long GetAllocationSize(void*){ColdBoundary();return 0;}
 virtual unsigned long GetAllocationSizeIncludingHeader(void*){ColdBoundary();return 0;}
 virtual unsigned long GetLargestFreeAreaSize(){ColdBoundary();return 0;}
 virtual unsigned long GetTotalMemoryControlledByAllocator(){ColdBoundary();return 0;}
 virtual unsigned long GetTotalMemoryAllocated(){ColdBoundary();return 0;}
 virtual unsigned long GetNoFreeAreas(){ColdBoundary();return 0;}
 virtual unsigned long GetNoAllocatedAreas(){ColdBoundary();return 0;}
 virtual float GetFragmentation(){ColdBoundary();return 0;}
 virtual void* GetAllocatedArea(long){ColdBoundary();return 0;}
 virtual unsigned long GetHeaderOverhead(){ColdBoundary();return 0;}
 virtual void Validate(){ColdBoundary();}
};
extern "C" CBankFileAsyncData* __fastcall ConstructNativeAllocatedData(CBankFileAsyncData*,void*,unsigned long,unsigned long,CMovableResourceMemoryPool*,CBankFileAsync&);
#pragma comment(linker,"/alternatename:@ConstructNativeAllocatedData@24=??0CBankFileAsyncData@@AAE@KKPAVCMovableResourceMemoryPool@@AAVCBankFileAsync@@@Z")

// Controlled queue consumer; its readiness comes from a real bank read.
// Queue traversal, removal, notification and destination insertion are native.
class QueuedRead:public CResource {
public:
 CResourceBank& Bank;CCountedPointer<CBankFileAsyncData> Data;unsigned Notifications;
 QueuedRead(CResourceBank& bank,const CCountedPointer<CBankFileAsyncData>& data):Bank(bank),Data(data),Notifications(0){ResourceSize=sizeof(*this)+data->Sizeof();}
 virtual bool IsResourceLoaded() const{return Data->IsFinished();}
 virtual void OnResourceLoaded(){CHECK(!ResourceList && !NextResource && !PrevResource);++Notifications;Bank.AddResource(*this);}
};

extern "C" CCharString* __fastcall NativeBankEnumeration(const NGameText::CDataBank*,void*,CCharString*,unsigned long);
extern "C" CDateAndTime* __fastcall NativeBankTimestamp(NGameText::CDataBank*,void*,CDateAndTime*,const CCharString&);
#pragma comment(linker,"/alternatename:@NativeBankEnumeration@16=?GetEnumerationType@CDataBank@NGameText@@MBE?AVCCharString@@K@Z")
#pragma comment(linker,"/alternatename:@NativeBankTimestamp@16=?GetLastModifiedTime@CDataBank@NGameText@@MAE?AVCDateAndTime@@ABVCCharString@@@Z")

struct FixtureSystemTime {unsigned short Year,Month,DayOfWeek,Day,Hour,Minute,Second,Millisecond;};
struct FixtureFileInfo {unsigned long Attributes;FILETIME Creation,Access,Write;unsigned long Volume,SizeHigh,SizeLow,Links,IndexHigh,IndexLow;};
extern "C" {
 __declspec(dllimport) int __stdcall GetFileInformationByHandle(void*,FixtureFileInfo*);
 __declspec(dllimport) int __stdcall FileTimeToSystemTime(const FILETIME*,FixtureSystemTime*);
 __declspec(dllimport) unsigned long __stdcall GetFileAttributesA(const char*);
 __declspec(dllimport) int __stdcall SetFileAttributesA(const char*,unsigned long);
 CFileInformation* __fastcall NativeDiskInformation(CDiskFileWin32*,void*,CFileInformation*);
}
#pragma comment(linker,"/alternatename:@NativeDiskInformation@12=?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ")
extern "C" CAFile* __fastcall NativeDiskClone(const CDiskFileWin32*,void*);
#pragma comment(linker,"/alternatename:@NativeDiskClone@8=?Clone@CDiskFileWin32@@UBEPAVCAFile@@XZ")
static void CheckArchiveWindow(const char* path,unsigned long offset,const unsigned char* actual,unsigned long count){unsigned char expected[32];CHECK(count<=sizeof(expected));FILE* file=fopen(path,"rb");CHECK(file && !fseek(file,offset,SEEK_SET) && fread(expected,1,count,file)==count);CHECK(!fclose(file) && !memcmp(expected,actual,count));}
static bool MatchFileStamp(const CDateAndTime& date,const FILETIME& raw){FixtureSystemTime system;CHECK(FileTimeToSystemTime(&raw,&system));return date.Year==system.Year && date.Month==system.Month && date.Day==system.Day && date.Hour==system.Hour && date.Minute==system.Minute && date.Second==system.Second && date.Millisecond==system.Millisecond;}
static bool ZeroFileStamp(const CDateAndTime& date){return !date.Year && !date.Month && !date.Day && !date.Hour && !date.Minute && !date.Second && !date.Millisecond;}

extern double FABLE_FASTCALL GFGetTime();
extern bool g_FableHighResolutionTimer_013CAA00;
extern __int64 g_FableTimerFrequency_013CAA08;
extern __int64 g_FableTimerStartTick_013CB2F0;
class BudgetRead:public QueuedRead {
public:
 __int64 Shift;
 BudgetRead(CResourceBank& bank,const CCountedPointer<CBankFileAsyncData>& data,__int64 shift):QueuedRead(bank,data),Shift(shift){ResourceSize=sizeof(*this)+data->Sizeof();}
 virtual void OnResourceLoaded(){QueuedRead::OnResourceLoaded();g_FableTimerStartTick_013CB2F0-=Shift;}
};

// Consumer abort policy stays controlled; actual queue removal, async discard,
// pending-read refusal, completion and disposal ownership all execute natively.
class QueuedAbortRead:public CResource {
public:
 CCountedPointer<CBankFileAsyncData> Data;unsigned Aborts,Loaded;
 QueuedAbortRead(const CCountedPointer<CBankFileAsyncData>& data):Data(data),Aborts(0),Loaded(0){ResourceSize=sizeof(*this)+data->Sizeof();}
 virtual bool CanAbortResourceLoad(long frames) const{return frames>=2 && !Data->IsFinished();}
 virtual bool IsResourceLoaded() const{return Data.Data && Data->IsFinished();}
 virtual void OnResourceLoaded(){++Loaded;ColdBoundary();}
 virtual void OnResourceLoadAborted(){CHECK(!ResourceList && !NextResource && !PrevResource);++Aborts;CBankFileAsyncData::DiscardAsyncData(Data);}
};

class PoolFailure:public CFailedAllocationHandler {
public:
 CMovableResourceMemoryPool& Pool;long Blocker;unsigned Calls;bool Recover;
 PoolFailure(CMovableResourceMemoryPool& pool,bool recover):Pool(pool),Blocker(-1),Calls(0),Recover(recover){}
 virtual bool HandleFailedAllocation(unsigned long size){CHECK(size && ++Calls==1);if(!Recover)return false;CHECK(Blocker!=-1);Pool.FreeResource(Blocker);Blocker=-1;return true;}
};

static void CheckText(const NGameText::CTextEntry& entry,unsigned round){
 char expected[32];sprintf(expected,"native_%u",round);CCharString narrow(expected);CWideString wide=narrow.ToWideString();CHECK(!wcscmp((const wchar_t*)entry.Text,(const wchar_t*)wide));CHECK(!strcmp((const char*)entry.SpeechBank,"SPEECH") && !strcmp((const char*)entry.Narrator,"NARRATOR") && !strcmp((const char*)entry.SoundName,"SOUND") && entry.Tags.size()==round%5);
 for(unsigned i=0;i<entry.Tags.size();++i){sprintf(expected,"TAG_%u",i);CHECK((unsigned long)entry.Tags[i].Pos==0x80000000+round*7+i && !strcmp((const char*)entry.Tags[i].Name,expected));}
}
static void CheckGroup(const NGameText::CGroupEntry& entry,unsigned round){CHECK(entry.Entries.size()==round%17);for(unsigned i=0;i<entry.Entries.size();++i)CHECK((unsigned long)entry.Entries[i]==0xf0000000+round*11+i*3);}

static void CheckList(const CResourceList& list){
 const CResource* node=list.Head.NextResource;unsigned count=0,total=0;while(node!=&list.Head){CHECK(node && node->NextResource && node->PrevResource && node->NextResource->PrevResource==node && node->PrevResource->NextResource==node);total+=node->ResourceSize;node=node->NextResource;CHECK(++count<=64);}CHECK(count==list.ResourceCount && total==(unsigned long)list.AllocatedMemory);
}
static void CheckOwners(NGameText::CDataBank& bank,unsigned removed,const unsigned long* addresses){
 for(unsigned index=1;index<=64;++index){const CResource* resource=index%2?(const CResource*)bank.GetTextInfo(index)->PText.data_:(const CResource*)bank.GetGroupInfo(index)->PGroup.data_;if(index<=removed)CHECK(!resource);else CHECK(resource && (unsigned long)resource==addresses[index] && resource->IVRefCount==1);}
}
static void ProbePriority(NGameText::CDataBank& bank,const char* archive,unsigned scenario){
 const bool promote=scenario==1;
 ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead);ReadTraceCount=0;TraceReads=true;PauseRead=true;
 {
  CCountedPointer<CBankFileAsyncEntry> a=bank.GetAsyncEntry(1),b=bank.GetAsyncEntry(2),c=bank.GetAsyncEntry(3),d=bank.GetAsyncEntry(4);
  CCountedPointer<CBankFileAsyncData> first=a->BeginReadDataAutoAlloc(0,128,THREADED_FILE_PRIORITY_NORMAL);
  CHECK(WaitForSingleObject(ReadIssued,5000)==0 && !PauseRead && ReadTraceCount==1 && !first->Finished);
  CCountedPointer<CBankFileAsyncData> low=(scenario==2?d.Data:b.Data)->BeginReadDataAutoAlloc(0,128,scenario>=2?THREADED_FILE_PRIORITY_HIGH:THREADED_FILE_PRIORITY_LOW);
  CCountedPointer<CBankFileAsyncData> high=(scenario==3?b.Data:c.Data)->BeginReadDataAutoAlloc(0,128,(promote || scenario>=2)?THREADED_FILE_PRIORITY_HIGH:THREADED_FILE_PRIORITY_HIGHEST);
  CCountedPointer<CBankFileAsyncData> below=(scenario>=2?b.Data:d.Data)->BeginReadDataAutoAlloc(0,128,scenario>=2?THREADED_FILE_PRIORITY_HIGH:THREADED_FILE_PRIORITY_BELOW_NORMAL);
  CHECK(ReadTraceCount==1 && !low->Finished && !high->Finished && !below->Finished && bank.AsyncDataInstances.size()==4);
  if(promote){CHECK(first->ThreadedFileUID && low->ThreadedFileUID);bank.ThreadedFile->SetObjectPriority(first->ThreadedFileUID,THREADED_FILE_PRIORITY_HIGHEST);CHECK(ActiveReadDesc && ActiveReadDesc->Priority==THREADED_FILE_PRIORITY_NORMAL);bank.ThreadedFile->SetObjectPriority(low->ThreadedFileUID,THREADED_FILE_PRIORITY_HIGHEST);}
  CHECK(SetEvent(ReleaseRead));CBankFileAsyncData* data[]={first.Data,low.Data,high.Data,below.Data};
  unsigned long started=GetTickCount();for(unsigned n=0;n<4;++n)while(!data[n]->IsFinished()){CHECK(GetTickCount()-started<5000);Sleep(0);}
  TraceReads=false;CHECK(ReadTraceCount==4);unsigned priority[]={3,5,promote?4u:2u,promote?2u:1u},index[]={1,promote?2u:3u,promote?3u:4u,promote?4u:2u},request[]={0,promote?1u:2u,promote?2u:3u,promote?3u:1u};if(scenario==2){priority[1]=priority[2]=priority[3]=4;index[1]=2;index[2]=3;index[3]=4;request[1]=3;request[2]=2;request[3]=1;}if(scenario==3){priority[1]=priority[2]=priority[3]=4;index[1]=index[2]=index[3]=2;request[1]=1;request[2]=2;request[3]=3;}
  for(unsigned n=0;n<4;++n){if(scenario==3 && n>1)CHECK(ReadTrace[n-1].ObjectAddress<ReadTrace[n].ObjectAddress);CHECK(ReadTrace[n].Priority==priority[n] && ReadTrace[n].Offset==bank.GetEntryDataOffset(index[n]));const unsigned char* bytes=static_cast<const unsigned char*>(data[request[n]]->GetData());CheckArchiveWindow(archive,ReadTrace[n].Offset,bytes,32);printf("PRIORITY_READ\t%u\t%u\t%u\t%u\t",scenario,n,ReadTrace[n].Priority,ReadTrace[n].Offset);for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");}
 }
 CHECK(bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead));ReadIssued=ReleaseRead=0;
}
class DirectRead : public CThreadedFileCallback {
public:
 void* Done;unsigned Begins,Completions;EThreadedFilePriority ExpectedPriority;unsigned char Bytes[64];
 DirectRead():Done(CreateEventA(0,0,0,0)),Begins(0),Completions(0),ExpectedPriority(THREADED_FILE_PRIORITY_HIGH){CHECK(Done);memset(Bytes,0x95,sizeof(Bytes));}
 virtual ~DirectRead(){CHECK(CloseHandle(Done));}
 virtual bool OnFileReadBegin(void** destination,EThreadedFilePriority priority){CHECK(destination && *destination==Bytes+16 && priority==ExpectedPriority && ++Begins==1);return true;}
 virtual void OnFileReadCompletion(bool success){CHECK(success && Begins==1 && ++Completions==1);CHECK(SetEvent(Done));}
};
static unsigned AddDirect(CThreadedFile& file,DirectRead& read,unsigned offset,EThreadedFilePriority priority){
 read.ExpectedPriority=priority;CThreadedFileObjectDesc desc;desc.PDst=read.Bytes+16;desc.FilePos=offset;desc.Size=32;desc.PCallback=&read;desc.QueueWhenFinished=false;desc.Priority=priority;return file.AddObject(desc);
}
static void ProbeFileKeys(const CWideString& path,const char* archive){
 long beforeChars=g_CCharStringInstanceCount_013BD800,beforeWide=g_CWideStringInstanceCount_013BCA20;
 {
  CThreadedFile a,b,c;CHECK(a.Open(path,false) && b.Open(path,false) && c.Open(path,false));CHECK(a.deviceId_==b.deviceId_ && b.deviceId_==c.deviceId_ && a.physicalSortKey_<b.physicalSortKey_ && b.physicalSortKey_<c.physicalSortKey_);
  DirectRead read[4];CThreadedFile* owner[]={&c,&c,&b,&a};unsigned offset[]={16,0,8,128},uid[4];ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead);ReadTraceCount=0;TraceReads=true;PauseRead=true;
  uid[0]=AddDirect(c,read[0],offset[0],THREADED_FILE_PRIORITY_NORMAL);CHECK(uid[0] && WaitForSingleObject(ReadIssued,5000)==0 && !PauseRead && ReadTraceCount==1);
  for(unsigned n=1;n<4;++n){uid[n]=AddDirect(*owner[n],read[n],offset[n],THREADED_FILE_PRIORITY_HIGH);CHECK(uid[n] && uid[n]!=uid[0]);}CHECK(ReadTraceCount==1);CHECK(SetEvent(ReleaseRead));
  for(unsigned n=0;n<4;++n)CHECK(WaitForSingleObject(read[n].Done,5000)==0);
  TraceReads=false;CHECK(ReadTraceCount==4);unsigned request[]={0,3,2,1},rank[]={3,1,2,3};
  for(unsigned n=0;n<4;++n){unsigned r=request[n];CHECK(ReadTrace[n].Handle==owner[r]->fileHandle_ && ReadTrace[n].Offset==offset[r] && ReadTrace[n].Priority==(n?4u:3u));CHECK(read[r].Begins==1 && read[r].Completions==1);for(unsigned k=0;k<sizeof(read[r].Bytes);++k)if(k<16 || k>=48)CHECK(read[r].Bytes[k]==0x95);CheckArchiveWindow(archive,offset[r],read[r].Bytes+16,32);printf("FILE_SORT_READ\t%u\t%u\t%u\t",n,rank[n],offset[r]);for(unsigned k=16;k<48;++k)printf("%02x",read[r].Bytes[k]);printf("\n");}
  // Submission order is already captured. Join native object removal now,
  // after every completion callback fired, so waits cannot change selection.
  for(unsigned n=0;n<4;++n)owner[n]->WaitForObjectToFinish(uid[n]);
  a.Close();b.Close();c.Close();CHECK(!a.openFlag_ && !b.openFlag_ && !c.openFlag_);a.Close();b.Close();c.Close();
  CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead));ReadIssued=ReleaseRead=0;
 }
 CHECK(g_CCharStringInstanceCount_013BD800==beforeChars && g_CWideStringInstanceCount_013BCA20==beforeWide);
}
extern "C" void ProbeNativeFileSeekExceptions(CDiskFileWin32*,const char*);
class CThreadedFileManager;
extern CThreadedFileManager g_FableThreadedFileManager_013BC870;
extern "C" void __fastcall NativeDeviceSlice(CThreadedFileManager*,void*,EThreadedFileDeviceID,float);
#pragma comment(linker,"/alternatename:@NativeDeviceSlice@16=?SetDeviceTimeSlice@CThreadedFileManager@@QAEXW4EThreadedFileDeviceID@@M@Z")
static void ProbeDevices(const CWideString& diskPath,const char* archive,const char* reference,bool grouped,EThreadedFilePriority priority){
 long beforeChars=g_CCharStringInstanceCount_013BD800,beforeWide=g_CWideStringInstanceCount_013BCA20;
 {
  CCharString referenceName(reference);CWideString referencePath=referenceName.ToWideString();CThreadedFile disk,referenceFile;CHECK(disk.Open(diskPath,false) && referenceFile.Open(referencePath,false));CHECK(disk.deviceId_==1 && referenceFile.deviceId_==0);
  DirectRead read[5];CThreadedFile* owner[]={&disk,&disk,&referenceFile,&disk,&referenceFile};unsigned offset[]={16,32,128,0,0},uid[5],device[]={1,1,0,1,0};
  ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead);ReadTraceCount=0;TraceReads=true;PauseRead=true;
  uid[0]=AddDirect(disk,read[0],16,THREADED_FILE_PRIORITY_NORMAL);CHECK(uid[0] && WaitForSingleObject(ReadIssued,5000)==0 && !PauseRead && ReadTraceCount==1);
  unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);unsigned char policy[84];float slices[2];__int64 startTick;unsigned initialDevice;double oldEnd;
  {
   CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);memcpy(policy,manager+0xe4,sizeof(policy));memcpy(slices,manager+0x170,sizeof(slices));memcpy(&initialDevice,policy+static_cast<unsigned>(priority)*4,4);memcpy(&oldEnd,policy+28+static_cast<unsigned>(priority)*8,8);CHECK(initialDevice<2);
   startTick=g_FableTimerStartTick_013CB2F0;g_FableTimerStartTick_013CB2F0-=g_FableHighResolutionTimer_013CAA00?g_FableTimerFrequency_013CAA08*60:60000;CHECK(GFGetTime()>oldEnd+1);
   float selected=grouped?100.0f:0.0f;NativeDeviceSlice(&g_FableThreadedFileManager_013BC870,0,THREADED_FILE_DEVICE_ID_HARD_DISK,selected);NativeDeviceSlice(&g_FableThreadedFileManager_013BC870,0,THREADED_FILE_DEVICE_ID_DVD,selected);float actual[2];memcpy(actual,manager+0x170,sizeof(actual));CHECK(actual[0]==selected && actual[1]==selected);
  }
  for(unsigned n=1;n<5;++n){uid[n]=AddDirect(*owner[n],read[n],offset[n],priority);CHECK(uid[n]);for(unsigned k=0;k<n;++k)CHECK(uid[n]!=uid[k]);}CHECK(ReadTraceCount==1);CHECK(SetEvent(ReleaseRead));
  for(unsigned n=0;n<5;++n)CHECK(WaitForSingleObject(read[n].Done,5000)==0);
  TraceReads=false;CHECK(ReadTraceCount==5 && ReadTrace[0].Handle==disk.fileHandle_ && ReadTrace[0].Offset==16);unsigned seen[2]={0,0};
  for(unsigned n=1;n<5;++n){unsigned first=(initialDevice+1)%2;unsigned d=grouped?(n<=2?first:1-first):(initialDevice+n)%2;unsigned expected=seen[d]++?(d?32:128):0;CHECK(ReadTrace[n].Handle==(d?disk.fileHandle_:referenceFile.fileHandle_) && ReadTrace[n].Offset==expected && ReadTrace[n].Priority==static_cast<unsigned>(priority));}CHECK(seen[0]==2 && seen[1]==2);
  for(unsigned n=0;n<5;++n){CHECK(read[n].Begins==1 && read[n].Completions==1);for(unsigned k=0;k<sizeof(read[n].Bytes);++k)if(k<16 || k>=48)CHECK(read[n].Bytes[k]==0x95);CheckArchiveWindow(device[n]?archive:reference,offset[n],read[n].Bytes+16,32);printf("PRIORITY_DEVICE_WINDOW\t%u\t%u\t%u\t%u\t%u\t",static_cast<unsigned>(priority),grouped,n,device[n],offset[n]);for(unsigned k=16;k<48;++k)printf("%02x",read[n].Bytes[k]);printf("\n");}
  for(unsigned n=0;n<5;++n)owner[n]->WaitForObjectToFinish(uid[n]);
  {
   CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);g_FableTimerStartTick_013CB2F0=startTick;NativeDeviceSlice(&g_FableThreadedFileManager_013BC870,0,THREADED_FILE_DEVICE_ID_HARD_DISK,slices[0]);NativeDeviceSlice(&g_FableThreadedFileManager_013BC870,0,THREADED_FILE_DEVICE_ID_DVD,slices[1]);memcpy(manager+0xe4,policy,sizeof(policy));CHECK(g_FableTimerStartTick_013CB2F0==startTick && !memcmp(manager+0xe4,policy,sizeof(policy)) && !memcmp(manager+0x170,slices,sizeof(slices)));
  }
  disk.Close();referenceFile.Close();CHECK(!disk.openFlag_ && !referenceFile.openFlag_);disk.Close();referenceFile.Close();CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead));ReadIssued=ReleaseRead=0;
 }
 CHECK(g_CCharStringInstanceCount_013BD800==beforeChars && g_CWideStringInstanceCount_013BCA20==beforeWide);
}

extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
class WriteCompletion : public CThreadedFileCallback {
public:
 void* Done;unsigned Completions;
 WriteCompletion():Done(CreateEventA(0,0,0,0)),Completions(0){CHECK(Done);}
 virtual ~WriteCompletion(){CHECK(CloseHandle(Done));}
 virtual bool OnFileReadBegin(void**,EThreadedFilePriority){CHECK(false);return false;}
 virtual void OnFileReadCompletion(bool success){CHECK(success && ++Completions==1);CHECK(SetEvent(Done));}
};
static void VerifyWritten(const char* path,const unsigned char* bytes,unsigned count){
 FILE* file=fopen(path,"rb");CHECK(file && !fseek(file,0,SEEK_END) && ftell(file)==static_cast<long>(count) && !fseek(file,0,SEEK_SET));
 for(unsigned n=0;n<count;++n)CHECK(fgetc(file)==bytes[n]);CHECK(fgetc(file)==EOF && !ferror(file) && !fclose(file));
}
static void ProbeWrites(const char* ownedPath){
 long beforeChars=g_CCharStringInstanceCount_013BD800,beforeWide=g_CWideStringInstanceCount_013BCA20;
 unsigned char* bytes=static_cast<unsigned char*>(VirtualAlloc(0,8192,0x3000,4));CHECK(bytes && !(reinterpret_cast<unsigned long>(bytes)&4095));for(unsigned n=0;n<8192;++n)bytes[n]=static_cast<unsigned char>((n*37+(n/4096)*19+11)%251);
 {
 CCharString filename(ownedPath);CWideString path=filename.ToWideString();CThreadedFile file;CHECK(file.OpenForWrite(path,true));CHECK(file.openFlag_ && file.IsOpenForWrite() && file.deviceId_==0 && file.length_==0);void* handle=file.fileHandle_;
 WriteCompletion completion[2];unsigned uid[2];
 for(unsigned n=0;n<2;++n){unsigned offset=n?0:4096;CThreadedFileObjectDesc desc;desc.PDst=bytes+offset;desc.FilePos=offset;desc.Size=4096;desc.PCallback=&completion[n];desc.QueueWhenFinished=false;desc.Priority=THREADED_FILE_PRIORITY_HIGH;uid[n]=file.AddObject(desc);CHECK(uid[n] && (!n || uid[n]!=uid[0]));}
 for(unsigned n=0;n<2;++n)CHECK(WaitForSingleObject(completion[n].Done,5000)==0 && completion[n].Completions==1);
 for(unsigned n=0;n<2;++n)file.WaitForObjectToFinish(uid[n]);CHECK(file.FinishAndGetFileHandle()==handle && file.openFlag_ && file.length_==0);file.Close();CHECK(!file.openFlag_);VerifyWritten(ownedPath,bytes,8192);
 CHECK(file.OpenForWrite(path,false) && file.length_==8192 && file.IsOpenForWrite());CHECK(file.ResizeFile(12288) && file.length_==12288);file.Close();FILE* extended=fopen(ownedPath,"rb");CHECK(extended && !fseek(extended,0,SEEK_END) && ftell(extended)==12288 && !fclose(extended));
 CHECK(file.OpenForWrite(path,false) && file.length_==12288);CHECK(file.ResizeFile(4096) && file.length_==4096);file.Close();VerifyWritten(ownedPath,bytes,4096);
 CHECK(file.OpenForWrite(path,true) && file.length_==0);file.Close();VerifyWritten(ownedPath,bytes,0);file.Close();
 CHECK(strlen(ownedPath)<900);char badPath[1024];sprintf(badPath,"%s\\child",ownedPath);CCharString badName(badPath);CWideString badWide=badName.ToWideString();CThreadedFile refused;CHECK(!refused.OpenForWrite(badWide,true) && !refused.openFlag_);
 }
 CHECK(VirtualFree(bytes,0,0x8000));CHECK(g_CCharStringInstanceCount_013BD800==beforeChars && g_CWideStringInstanceCount_013BCA20==beforeWide);
 printf("NATIVE_WRITE_PASS writes=2 bytes=8192 begin_skipped=2 completion=2 joined=2 expand=12288 truncate=4096 overwrite=0 failed_open=1 stale_length=1 cleanup=1\n");return;
}


extern "C" __declspec(dllimport) int __stdcall WriteFileEx(void*,const void*,unsigned long,_OVERLAPPED*,NativeReadCompletion);
extern "C" __declspec(dllimport) void* __stdcall CreateThread(void*,unsigned long,unsigned long (__stdcall*)(void*),void*,unsigned long,unsigned long*);
static void* WriteIssued;static void* ReleaseWrite;static volatile bool PauseWrite;
extern "C" int __stdcall ForwardPendingWrite(void* file,const void* bytes,unsigned long size,_OVERLAPPED* overlap,NativeReadCompletion completion){
 int result=WriteFileEx(file,bytes,size,overlap,completion);if(PauseWrite){CHECK(result);PauseWrite=false;CHECK(SetEvent(WriteIssued));CHECK(WaitForSingleObject(ReleaseWrite,5000)==0);}return result;
}
typedef int (__stdcall* WriteImportFunction)(void*,const void*,unsigned long,_OVERLAPPED*,NativeReadCompletion);
extern "C" WriteImportFunction FixtureWriteFileExSlot=ForwardPendingWrite;
#pragma comment(linker,"/alternatename:__imp__FixtureWriteFileEx@20=_FixtureWriteFileExSlot")
class CloseWriteCompletion : public CThreadedFileCallback {
public:
 void* Done;bool Expected;unsigned Completed;
 CloseWriteCompletion():Done(CreateEventA(0,0,0,0)),Expected(true),Completed(0){CHECK(Done);}
 virtual ~CloseWriteCompletion(){CHECK(CloseHandle(Done));}
 virtual bool OnFileReadBegin(void**,EThreadedFilePriority){CHECK(false);return false;}
 virtual void OnFileReadCompletion(bool success){CHECK(success==Expected && ++Completed==1);CHECK(SetEvent(Done));}
};
static unsigned long __stdcall CloseWriter(void* pointer){static_cast<CThreadedFile*>(pointer)->Close();return 0;}


extern "C" long __fastcall NativePendingObjects(const CThreadedFileManager*,void*);
extern "C" long __fastcall NativePendingBytes(const CThreadedFileManager*,void*);
extern "C" long __fastcall NativeFinishedObjects(const CThreadedFileManager*,void*);
extern "C" long __fastcall NativeFinishedBytes(const CThreadedFileManager*,void*);
#pragma comment(linker,"/alternatename:@NativePendingObjects@8=?GetTotalObjectsPendingCount@CThreadedFileManager@@QBEJXZ")
#pragma comment(linker,"/alternatename:@NativePendingBytes@8=?GetTotalBytesPendingCount@CThreadedFileManager@@QBEJXZ")
#pragma comment(linker,"/alternatename:@NativeFinishedObjects@8=?GetTotalObjectsFinishedCount@CThreadedFileManager@@QBEJXZ")
#pragma comment(linker,"/alternatename:@NativeFinishedBytes@8=?GetTotalBytesFinishedCount@CThreadedFileManager@@QBEJXZ")
static void CheckLiveStatistics(long pending,long pendingBytes,long finished,long finishedBytes){
 unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);
 CHECK(NativePendingObjects(&g_FableThreadedFileManager_013BC870,0)==pending && NativePendingBytes(&g_FableThreadedFileManager_013BC870,0)==pendingBytes && NativeFinishedObjects(&g_FableThreadedFileManager_013BC870,0)==finished && NativeFinishedBytes(&g_FableThreadedFileManager_013BC870,0)==finishedBytes);
}

struct CancelPeerLockProbe { _RTL_CRITICAL_SECTION* Section;bool Acquired; };
static unsigned long __stdcall ProbeCancelPeerLock(void* pointer){
 CancelPeerLockProbe* probe=static_cast<CancelPeerLockProbe*>(pointer);unsigned start=GetTickCount();
 while(GetTickCount()-start<2000){CCriticalSectionLock lock(*probe->Section,true);if(lock.IsLocked()){probe->Acquired=true;return 0;}Sleep(0);}return 0;
}
class CancelAndQueuePeer : public CloseWriteCompletion {
public:
 CThreadedFile* Peer;unsigned char* Bytes;CloseWriteCompletion* Next;unsigned* NewUID;
 CancelAndQueuePeer():Peer(0),Bytes(0),Next(0),NewUID(0){Expected=false;}
 virtual void OnFileReadCompletion(bool success){
  CHECK(!success && Peer && Bytes && Next && NewUID && !*NewUID);
  unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CancelPeerLockProbe probe;probe.Section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c);probe.Acquired=false;
  void* thread=CreateThread(0,0,ProbeCancelPeerLock,&probe,0,0);CHECK(thread && WaitForSingleObject(thread,2500)==0 && CloseHandle(thread));CHECK(probe.Acquired);
  {CCriticalSectionLock lock(*probe.Section,false);CHECK(*reinterpret_cast<long*>(manager+0x16c)==1 && Peer->openFlag_);}
  CloseWriteCompletion::OnFileReadCompletion(success);
  CThreadedFileObjectDesc desc;desc.PDst=Bytes;desc.FilePos=4096;desc.Size=4096;desc.PCallback=Next;desc.QueueWhenFinished=false;desc.Priority=THREADED_FILE_PRIORITY_HIGH;*NewUID=Peer->AddObject(desc);CHECK(*NewUID);CheckLiveStatistics(3,12288,0,0);
 }
};
static void ProbeCloseWrites(const char* ownedPath){
 long beforeChars=g_CCharStringInstanceCount_013BD800,beforeWide=g_CWideStringInstanceCount_013BCA20;unsigned char* bytes=static_cast<unsigned char*>(VirtualAlloc(0,12288,0x3000,4));CHECK(bytes && !(reinterpret_cast<unsigned long>(bytes)&4095));for(unsigned n=0;n<12288;++n)bytes[n]=static_cast<unsigned char>((n*17+29)%251);
 {
 CCharString name(ownedPath);CWideString path=name.ToWideString();CHECK(strlen(ownedPath)<850);char peerPath[1024];sprintf(peerPath,"%s.peer",ownedPath);CCharString peerName(peerPath);CWideString peerWide=peerName.ToWideString();CThreadedFile file,peer;CHECK(file.OpenForWrite(path,true) && peer.OpenForWrite(peerWide,true) && peer.physicalSortKey_>file.physicalSortKey_);
 CloseWriteCompletion completion[3];CancelAndQueuePeer cancelled;unsigned uid[4]={0,0,0,0};cancelled.Peer=&peer;cancelled.Bytes=bytes+8192;cancelled.Next=&completion[2];cancelled.NewUID=&uid[3];WriteIssued=CreateEventA(0,0,0,0);ReleaseWrite=CreateEventA(0,0,0,0);CHECK(WriteIssued && ReleaseWrite);PauseWrite=true;
 for(unsigned n=0;n<3;++n){CThreadedFileObjectDesc desc;desc.PDst=bytes+(n?4096:0);desc.FilePos=n==1?4096:0;desc.Size=4096;desc.PCallback=n==1?static_cast<CThreadedFileCallback*>(&cancelled):static_cast<CThreadedFileCallback*>(&completion[n==2?1:0]);desc.QueueWhenFinished=n==2;desc.Priority=THREADED_FILE_PRIORITY_HIGH;uid[n]=(n==2?peer:file).AddObject(desc);CHECK(uid[n] && (!n || uid[n]!=uid[0]));if(!n)CHECK(WaitForSingleObject(WriteIssued,5000)==0 && !PauseWrite && !completion[0].Completed);}
 CheckLiveStatistics(3,12288,0,0);
 CHECK(file.ObjectExists(uid[0]) && file.ObjectExists(uid[1]) && peer.ObjectExists(uid[2]) && file.ObjectExists(uid[2]) && peer.ObjectExists(uid[0]));
 CHECK(!file.IsObjectFinished(uid[0]) && !file.IsObjectFinished(uid[1]) && !peer.IsObjectFinished(uid[2]));
 CHECK(!file.ObjectExists(0) && file.IsObjectFinished(0));
 CHECK(uid[0]!=uid[1] && uid[0]!=uid[2] && uid[1]!=uid[2]);void* closer=CreateThread(0,0,CloseWriter,&file,0,0);CHECK(closer);unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);unsigned started=GetTickCount();
 for(;;){CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);if(*reinterpret_cast<long*>(manager+0x16c)){CHECK(*reinterpret_cast<long*>(manager+0x16c)==1 && file.openFlag_ && !completion[0].Completed && !cancelled.Completed && !uid[3]);break;}lock.Release();CHECK(GetTickCount()-started<5000);Sleep(0);}
 CHECK(WaitForSingleObject(closer,0)==258 && SetEvent(ReleaseWrite));CHECK(WaitForSingleObject(closer,5000)==0);CHECK(completion[0].Completed==1 && cancelled.Completed==1 && uid[3] && !file.openFlag_ && !file.IsOpenForWrite() && file.fileHandle_==reinterpret_cast<void*>(-1));CHECK(uid[3]!=uid[0] && uid[3]!=uid[1] && uid[3]!=uid[2] && !file.ObjectExists(uid[0]) && !file.ObjectExists(uid[1]));
 {CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);CHECK(!*reinterpret_cast<long*>(manager+0x16c));}
 peer.KeepUpdating();for(unsigned n=1;n<3;++n)CHECK(WaitForSingleObject(completion[n].Done,5000)==0 && completion[n].Completed==1);peer.WaitForObjectToFinish(uid[2]);peer.WaitForObjectToFinish(uid[3]);CHECK(peer.openFlag_ && peer.IsOpenForWrite() && peer.ObjectExists(uid[2]) && peer.IsObjectFinished(uid[2]) && file.ObjectExists(uid[2]) && file.IsObjectFinished(uid[2]) && !peer.ObjectExists(uid[3]) && peer.IsObjectFinished(uid[3]));CheckLiveStatistics(0,0,1,4096);peer.Close();CHECK(!peer.openFlag_ && !peer.IsOpenForWrite());CheckLiveStatistics(0,0,0,0);for(unsigned n=0;n<4;++n)CHECK(!file.ObjectExists(uid[n]) && file.IsObjectFinished(uid[n]) && !peer.ObjectExists(uid[n]) && peer.IsObjectFinished(uid[n]));CHECK(completion[1].Completed==1 && completion[2].Completed==1);VerifyWritten(ownedPath,bytes,4096);VerifyWritten(peerPath,bytes+4096,8192);file.Close();CHECK(CloseHandle(closer) && CloseHandle(WriteIssued) && CloseHandle(ReleaseWrite));WriteIssued=ReleaseWrite=0;
 }
 CHECK(VirtualFree(bytes,0,0x8000));CHECK(g_CCharStringInstanceCount_013BD800==beforeChars && g_CWideStringInstanceCount_013BCA20==beforeWide);printf("NATIVE_CLOSE_WRITE_PASS submitted=1 completed=1 queued_aborted=1 peer_completed=2 callback_submitted=1 callback_unlocked=1 query_states=4 retained_finished=1 statistics=4 bytes=12288 cleanup=1\n");
}


static void ProbeWrittenArchive(const char* sourcePath,const char* ownedPath){
 FILE* input=fopen(sourcePath,"rb");CHECK(input && !fseek(input,0,SEEK_END));long rawSize=ftell(input);CHECK(rawSize>0 && rawSize<1048576 && !fseek(input,0,SEEK_SET));unsigned size=static_cast<unsigned>(rawSize),rounded=(size+4095)&~4095u;CHECK(rounded>size);unsigned char* bytes=static_cast<unsigned char*>(VirtualAlloc(0,rounded,0x3000,4));CHECK(bytes && !(reinterpret_cast<unsigned long>(bytes)&4095));CHECK(fread(bytes,1,size,input)==size && !ferror(input) && !fclose(input));unsigned alignment=0;
 {
 CCharString filename(ownedPath);CWideString path=filename.ToWideString();
 {CThreadedFile writer;CHECK(writer.OpenForWrite(path,true) && writer.length_==0);WriteCompletion completed;CThreadedFileObjectDesc desc;desc.PDst=bytes;desc.FilePos=0;desc.Size=rounded;desc.PCallback=&completed;desc.QueueWhenFinished=false;desc.Priority=THREADED_FILE_PRIORITY_HIGH;unsigned uid=writer.AddObject(desc);CHECK(uid && WaitForSingleObject(completed.Done,5000)==0 && completed.Completions==1);writer.WaitForObjectToFinish(uid);CHECK(writer.length_==0 && writer.FinishAndGetFileHandle()==writer.fileHandle_);CHECK(writer.ResizeFile(size) && writer.length_==size);writer.Close();CHECK(!writer.openFlag_);}
 VerifyWritten(ownedPath,bytes,size);
 CBankFileManager& manager=g_FableBankFileManager_013CA79C;manager.UseRetailBanks=true;manager.OpenRetailBank(path,false);
 {
 NGameText::CDataBank bank;CCharString name("RESOURCE_TEST");CHECK(bank.OpenReadOnly(name,8|64|256));alignment=bank.Alignment;CResourceList& list=bank.ResourceList;list.SetMaximumMemory(0x7fffffff);list.SetResourceUnloadDelay(0);
 for(unsigned index=1;index<=64;++index){
  if(index%2){CIVCountedPointer<const NGameText::CTextEntry> value=bank.GetPTextEntry(index,true);CHECK(!value.IsNull());CheckText(*value.data_,(index-1)/2);}
  else{CIVCountedPointer<const NGameText::CGroupEntry> value=bank.GetPGroupEntry(index,true);CHECK(!value.IsNull());CheckGroup(*value.data_,(index-1)/2);}
 }
 CHECK(list.ResourceCount==64 && bank.AsyncDataInstances.empty());CheckList(list);list.SetMaximumMemory(0);unsigned passes=0;
 while(list.ResourceCount){CHECK(++passes<=8);unsigned before=list.ResourceCount;list.FrameEnd();list.MakeFreeSpace(0);CHECK(list.ResourceCount<before);CheckList(list);}
 CHECK(!list.ResourceCount && !list.AllocatedMemory && bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());bank.Close();bank.Close();
 }
 manager.Close();manager.Close();
 }
 CHECK(VirtualFree(bytes,0,0x8000));g_CAFileCurrentPath_013BCA10=L"";CHECK(g_CCharStringInstanceCount_013BD800==0 && g_CWideStringInstanceCount_013BCA20==2 && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());FableMemoryAllocatorShutdown_009f2d20();CHECK(!g_FableMemoryAllocator_013CAA90);printf("NATIVE_WRITTEN_ARCHIVE_PASS alignment=%u bytes=%u rounded=%u decoded=64 evicted=64 cleanup=1\n",alignment,size,rounded);
}


class PhaseRead : public CThreadedFileCallback {
public:
 void* Begun;void* ReleaseBegin;void* Done;unsigned Begins,Completed,BeginStatus;unsigned char Bytes[64];
 PhaseRead():Begun(CreateEventA(0,0,0,0)),ReleaseBegin(CreateEventA(0,0,0,0)),Done(CreateEventA(0,0,0,0)),Begins(0),Completed(0),BeginStatus(0){CHECK(Begun && ReleaseBegin && Done);memset(Bytes,0x95,sizeof(Bytes));}
 virtual ~PhaseRead(){CHECK(CloseHandle(Begun) && CloseHandle(ReleaseBegin) && CloseHandle(Done));}
 virtual bool OnFileReadBegin(void** destination,EThreadedFilePriority priority){CHECK(destination && *destination==Bytes+16 && priority==THREADED_FILE_PRIORITY_HIGH && ++Begins==1);BeginStatus=*reinterpret_cast<unsigned*>(reinterpret_cast<unsigned char*>(destination)+0x1c);CHECK(BeginStatus==1);CHECK(SetEvent(Begun) && WaitForSingleObject(ReleaseBegin,5000)==0);return true;}
 virtual void OnFileReadCompletion(bool success){CHECK(success && Begins==1 && ++Completed==1);CHECK(SetEvent(Done));}
};
static void CheckReadPhase(CThreadedFile& file,unsigned uid,bool exists,bool finished){
 unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);CHECK(file.ObjectExists(uid)==exists && file.IsObjectFinished(uid)==finished);CheckLiveStatistics(exists&&!finished?1:0,exists&&!finished?32:0,exists&&finished?1:0,exists&&finished?32:0);
}
static void ProbeReadPhases(const char* source){
 long beforeChars=g_CCharStringInstanceCount_013BD800,beforeWide=g_CWideStringInstanceCount_013BCA20;
 {
 CCharString name(source);CWideString path=name.ToWideString();CThreadedFile file;CHECK(file.Open(path,false));PhaseRead read;ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);PublishedReadIssued=CreateEventA(0,0,0,0);ReleasePublishedRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead && PublishedReadIssued && ReleasePublishedRead);PhaseReadMode=true;PauseRead=true;TraceReads=true;ReadTraceCount=0;OriginalPhaseCompletion=0;PublishedReadStatus=0;unsigned uid;
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);CThreadedFileObjectDesc desc;desc.PDst=read.Bytes+16;desc.FilePos=16;desc.Size=32;desc.PCallback=&read;desc.QueueWhenFinished=true;desc.Priority=THREADED_FILE_PRIORITY_HIGH;uid=file.AddObject(desc);CHECK(uid && !read.Begins && !ReadTraceCount);CheckReadPhase(file,uid,true,false);}
 CHECK(WaitForSingleObject(read.Begun,5000)==0 && read.BeginStatus==1 && !ReadTraceCount && !read.Completed);CheckReadPhase(file,uid,true,false);CHECK(SetEvent(read.ReleaseBegin));
 CHECK(WaitForSingleObject(ReadIssued,5000)==0 && !PauseRead && ReadTraceCount==1 && !read.Completed);CHECK(*reinterpret_cast<unsigned*>(reinterpret_cast<unsigned char*>(ActiveReadDesc)+0x1c)==2);CheckReadPhase(file,uid,true,false);CHECK(SetEvent(ReleaseRead));
 CHECK(WaitForSingleObject(PublishedReadIssued,5000)==0 && PublishedReadStatus==3 && !read.Completed);CheckReadPhase(file,uid,true,false);CHECK(SetEvent(ReleasePublishedRead));
 CHECK(WaitForSingleObject(read.Done,5000)==0 && read.Completed==1);file.WaitForObjectToFinish(uid);CheckReadPhase(file,uid,true,true);CHECK(ReadTraceCount==1 && ReadTrace[0].Offset==16 && ReadTrace[0].Priority==4);TraceReads=false;
 for(unsigned n=0;n<sizeof(read.Bytes);++n)if(n<16 || n>=48)CHECK(read.Bytes[n]==0x95);CheckArchiveWindow(source,16,read.Bytes+16,32);file.Close();CheckReadPhase(file,uid,false,true);CHECK(!file.openFlag_ && read.Completed==1);file.Close();
 CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead) && CloseHandle(PublishedReadIssued) && CloseHandle(ReleasePublishedRead));ReadIssued=ReleaseRead=PublishedReadIssued=ReleasePublishedRead=0;OriginalPhaseCompletion=0;ActiveReadDesc=0;PhaseReadMode=false;
 }
 CHECK(g_CCharStringInstanceCount_013BD800==beforeChars && g_CWideStringInstanceCount_013BCA20==beforeWide);printf("NATIVE_READ_PHASES_PASS queued=1 begin_locked=1 submitted=1 completed_native=1 retained=1 removed=1 payload=32 cleanup=1\n");
}

static void* StartPhaseReadClose(CThreadedFile& file){
 void* thread=CreateThread(0,0,CloseWriter,&file,0,0);CHECK(thread);unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);unsigned start=GetTickCount();for(;;){CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);if(*reinterpret_cast<long*>(manager+0x16c)){CHECK(*reinterpret_cast<long*>(manager+0x16c)==1 && file.openFlag_);break;}lock.Release();CHECK(GetTickCount()-start<5000);Sleep(0);}CHECK(WaitForSingleObject(thread,0)==258);return thread;
}
static void ProbeCloseReadPhases(const char* source,unsigned closeAt){
 long beforeChars=g_CCharStringInstanceCount_013BD800,beforeWide=g_CWideStringInstanceCount_013BCA20;
 {
 CCharString name(source);CWideString path=name.ToWideString();CThreadedFile file;CHECK(file.Open(path,false));PhaseRead read;ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);PublishedReadIssued=CreateEventA(0,0,0,0);ReleasePublishedRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead && PublishedReadIssued && ReleasePublishedRead);PhaseReadMode=true;PauseRead=true;TraceReads=true;ReadTraceCount=0;OriginalPhaseCompletion=0;PublishedReadStatus=0;unsigned uid;void* closer=0;
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);CThreadedFileObjectDesc desc;desc.PDst=read.Bytes+16;desc.FilePos=16;desc.Size=32;desc.PCallback=&read;desc.QueueWhenFinished=true;desc.Priority=THREADED_FILE_PRIORITY_HIGH;uid=file.AddObject(desc);CHECK(uid && !read.Begins && !ReadTraceCount);CheckReadPhase(file,uid,true,false);}
 CHECK(WaitForSingleObject(read.Begun,5000)==0 && read.BeginStatus==1 && !ReadTraceCount && !read.Completed);if(closeAt==1)closer=StartPhaseReadClose(file);CheckReadPhase(file,uid,true,false);CHECK(SetEvent(read.ReleaseBegin));
 CHECK(WaitForSingleObject(ReadIssued,5000)==0 && !PauseRead && ReadTraceCount==1 && !read.Completed);CHECK(*reinterpret_cast<unsigned*>(reinterpret_cast<unsigned char*>(ActiveReadDesc)+0x1c)==2);if(closeAt==2)closer=StartPhaseReadClose(file);CheckReadPhase(file,uid,true,false);CHECK(SetEvent(ReleaseRead));
 CHECK(WaitForSingleObject(PublishedReadIssued,5000)==0 && PublishedReadStatus==3 && !read.Completed);if(closeAt==3)closer=StartPhaseReadClose(file);CHECK(closer && WaitForSingleObject(closer,0)==258);CheckReadPhase(file,uid,true,false);CHECK(SetEvent(ReleasePublishedRead));
 CHECK(WaitForSingleObject(read.Done,5000)==0 && read.Completed==1);file.WaitForObjectToFinish(uid);CHECK(WaitForSingleObject(closer,5000)==0);CheckReadPhase(file,uid,false,true);CHECK(ReadTraceCount==1 && ReadTrace[0].Offset==16 && ReadTrace[0].Priority==4);TraceReads=false;
 for(unsigned n=0;n<sizeof(read.Bytes);++n)if(n<16 || n>=48)CHECK(read.Bytes[n]==0x95);CheckArchiveWindow(source,16,read.Bytes+16,32);file.Close();CheckReadPhase(file,uid,false,true);CHECK(!file.openFlag_ && read.Completed==1);file.Close();
 CHECK(CloseHandle(closer) && CloseHandle(ReadIssued) && CloseHandle(ReleaseRead) && CloseHandle(PublishedReadIssued) && CloseHandle(ReleasePublishedRead));ReadIssued=ReleaseRead=PublishedReadIssued=ReleasePublishedRead=0;OriginalPhaseCompletion=0;ActiveReadDesc=0;PhaseReadMode=false;
 }
 CHECK(g_CCharStringInstanceCount_013BD800==beforeChars && g_CWideStringInstanceCount_013BCA20==beforeWide);printf("NATIVE_CLOSE_READ_PHASE_PASS close_at=%u begin=1 submitted=1 native_completed=1 removed=1 payload=32 cleanup=1\n",closeAt);
}



static unsigned MetadataReadAttempts;
static int __stdcall GuardMetadataRead(void*,void*,unsigned long,_OVERLAPPED*,NativeReadCompletion){++MetadataReadAttempts;CHECK(false);return 0;}
class RefuseMetadataRead : public CThreadedFileCallback {
public:
 void* Begun;void* ReleaseBegin;bool AllowBegin;unsigned Begins,Completed;unsigned char Bytes[64];
 RefuseMetadataRead():Begun(CreateEventA(0,0,0,0)),ReleaseBegin(CreateEventA(0,0,0,0)),AllowBegin(false),Begins(0),Completed(0){CHECK(Begun && ReleaseBegin);memset(Bytes,0x95,sizeof(Bytes));}
 virtual ~RefuseMetadataRead(){CHECK(CloseHandle(Begun) && CloseHandle(ReleaseBegin));}
 virtual bool OnFileReadBegin(void** destination,EThreadedFilePriority priority){CHECK(AllowBegin && destination && *destination==Bytes+16 && priority==THREADED_FILE_PRIORITY_HIGH && ++Begins==1);CHECK(*reinterpret_cast<unsigned*>(reinterpret_cast<unsigned char*>(destination)+0x1c)==1);CHECK(SetEvent(Begun) && WaitForSingleObject(ReleaseBegin,5000)==0);return false;}
 virtual void OnFileReadCompletion(bool success){CHECK(!success && ++Completed==1);}
};
static void ProbeDescriptorMetadata(const char* source,unsigned pair){
 static const unsigned sizes[6][2]={{0,0},{1,0xffffffffu},{0x7fffffffu,1},{0x80000000u,0x80000000u},{0xffffffffu,2},{0xffffffffu,0xffffffffu}};CHECK(pair<6);unsigned sum=sizes[pair][0]+sizes[pair][1];long beforeChars=g_CCharStringInstanceCount_013BD800,beforeWide=g_CWideStringInstanceCount_013BCA20;NativeReadAPI saved=FixtureReadFileExSlot;FixtureReadFileExSlot=GuardMetadataRead;MetadataReadAttempts=0;
 {
 CCharString name(source);CWideString path=name.ToWideString();CThreadedFile file;CHECK(file.Open(path,false));RefuseMetadataRead callback[2];callback[0].AllowBegin=true;unsigned uid[2];unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);
 {CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);for(unsigned n=0;n<2;++n){CThreadedFileObjectDesc desc;desc.PDst=callback[n].Bytes+16;desc.FilePos=0;desc.Size=sizes[pair][n];desc.PCallback=&callback[n];desc.QueueWhenFinished=false;desc.Priority=THREADED_FILE_PRIORITY_HIGH;uid[n]=file.AddObject(desc);CHECK(uid[n] && (!n || uid[n]!=uid[0]));}CheckLiveStatistics(2,static_cast<long>(sum),0,0);}
 CHECK(WaitForSingleObject(callback[0].Begun,5000)==0 && callback[0].Begins==1 && !callback[1].Begins);void* closer=CreateThread(0,0,CloseWriter,&file,0,0);CHECK(closer);unsigned start=GetTickCount();for(;;){CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);if(*reinterpret_cast<long*>(manager+0x16c)){CHECK(*reinterpret_cast<long*>(manager+0x16c)==1 && file.openFlag_);break;}lock.Release();CHECK(GetTickCount()-start<5000);Sleep(0);}CHECK(WaitForSingleObject(closer,0)==258);CheckLiveStatistics(2,static_cast<long>(sum),0,0);CHECK(SetEvent(callback[0].ReleaseBegin));CHECK(WaitForSingleObject(closer,5000)==0 && !file.openFlag_ && !MetadataReadAttempts);CHECK(callback[0].Completed==1 && callback[1].Completed==1 && callback[0].Begins==1 && !callback[1].Begins);CheckLiveStatistics(0,0,0,0);for(unsigned n=0;n<2;++n){CHECK(!file.ObjectExists(uid[n]) && file.IsObjectFinished(uid[n]));for(unsigned k=0;k<64;++k)CHECK(callback[n].Bytes[k]==0x95);}file.Close();CHECK(CloseHandle(closer));
 }
 FixtureReadFileExSlot=saved;CHECK(!MetadataReadAttempts && g_CCharStringInstanceCount_013BD800==beforeChars && g_CWideStringInstanceCount_013BCA20==beforeWide);printf("NATIVE_DESCRIPTOR_ARITHMETIC_PASS pair=%u first=%u second=%u sum=%u os_reads=0 completed_false=2 cleanup=1\n",pair,sizes[pair][0],sizes[pair][1],sum);
}

typedef void (__fastcall* EofHardCallback)(const char*);
extern "C" EofHardCallback EofHardError;
#pragma comment(linker,"/alternatename:_EofHardError=?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA")
extern "C" void __fastcall NativeSetHardPolicy(EofHardCallback);
extern "C" void __fastcall NativeSetDirtyPolicy(bool);
#pragma comment(linker,"/alternatename:@NativeSetHardPolicy@4=?SetHardDiskErrorCallback@CThreadedFileManager@@SIXP6IXPBD@Z@Z")
#pragma comment(linker,"/alternatename:@NativeSetDirtyPolicy@4=?SetDirtyDiscError@CThreadedFileManager@@SIX_N@Z")
static unsigned EofHardCalls;static void* EofCompletingObject;
static void __fastcall ObserveEofHardError(const char* text){CHECK(text && !*text && EofCompletingObject && *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(EofCompletingObject)+0x1c)==2);++EofHardCalls;}
static NativeReadCompletion EofNativeCompletion;
extern "C" bool EofDirtyDisc;
#pragma comment(linker,"/alternatename:_EofDirtyDisc=?m_bDirtyDiscError@CThreadedFileManager@@0_NA")
static unsigned EofError,EofTransferred;
static void __stdcall EofCompletion(unsigned long error,unsigned long count,_OVERLAPPED* overlap){
 EofError=error;EofTransferred=count;EofCompletingObject=overlap->hEvent;EofNativeCompletion(error,count,overlap);EofCompletingObject=0;
}
static int __stdcall EofRead(void* file,void* buffer,unsigned long size,_OVERLAPPED* overlap,NativeReadCompletion completion){
 CHECK(!EofNativeCompletion);EofNativeCompletion=completion;return ReadFileEx(file,buffer,size,overlap,EofCompletion);
}
static void ProbeEof(const char* source,unsigned device=1){
 const char* argv[]={0,source};long chars=g_CCharStringInstanceCount_013BD800,wide=g_CWideStringInstanceCount_013BCA20;
 FILE* input=fopen(argv[1],"rb");CHECK(input);CHECK(!fseek(input,0,SEEK_END));long size=ftell(input);CHECK(size>32 && !fclose(input));
 bool savedDirty;EofHardCallback savedHard;NativeReadAPI savedRead=FixtureReadFileExSlot;
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);savedDirty=EofDirtyDisc;savedHard=EofHardError;NativeSetHardPolicy(ObserveEofHardError);FixtureReadFileExSlot=EofRead;}
 for(unsigned mode=0;mode<3;++mode){
  CCharString name(argv[1]);CWideString path=name.ToWideString();CThreadedFile file;CHECK(file.Open(path,false) && file.deviceId_==device);DirectRead read;EofHardCalls=0;EofNativeCompletion=0;EofError=EofTransferred=0xffffffffu;
  {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);NativeSetDirtyPolicy(false);}
  unsigned offset=static_cast<unsigned>(size)-(mode==0?13:0)+(mode==2?7:0);CThreadedFileObjectDesc desc;desc.PDst=read.Bytes+16;desc.FilePos=offset;desc.Size=32;desc.PCallback=&read;desc.QueueWhenFinished=true;desc.Priority=THREADED_FILE_PRIORITY_HIGH;unsigned uid=file.AddObject(desc);CHECK(uid);
  CHECK(WaitForSingleObject(read.Done,5000)==0);file.WaitForObjectToFinish(uid);CHECK(read.Begins==1 && read.Completions==1);CheckReadPhase(file,uid,true,true);
  CHECK(EofTransferred==(mode==0?13u:0u) && EofError==(mode==0?0u:38u));
  {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);CHECK(EofDirtyDisc==(device==1 && mode!=0));CHECK(EofHardCalls==(!device && mode!=0?1u:0u));}
  for(unsigned n=0;n<64;++n)if(n<16 || n>=16+EofTransferred)CHECK(read.Bytes[n]==0x95);if(EofTransferred)CheckArchiveWindow(argv[1],offset,read.Bytes+16,EofTransferred);
  printf("EOF_READ mode=%u error=%u transferred=%u requested=32 success=1\n",mode,EofError,EofTransferred);file.Close();CheckReadPhase(file,uid,false,true);CHECK(!file.openFlag_);
 }
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);NativeSetDirtyPolicy(savedDirty);NativeSetHardPolicy(savedHard);FixtureReadFileExSlot=savedRead;}
 EofNativeCompletion=0;CHECK(g_CCharStringInstanceCount_013BD800==chars && g_CWideStringInstanceCount_013BCA20==wide);printf("NATIVE_EOF_PASS cases=3 cleanup=1 device=%u\n",device);
}

static CThreadedFile* EofPeerFile;static DirectRead* EofPeerRead;
static unsigned EofPeerUID,EofPeerHardCalls,EofPeerNativeCalls;
static NativeReadCompletion EofPeerNativeCompletion;static void* EofPeerCompletingObject;
static unsigned EofPeerErrors[2],EofPeerTransfers[2];
static void __stdcall PeerNativeCompletion(unsigned long error,unsigned long count,_OVERLAPPED* overlap){
 CHECK(EofPeerNativeCalls<2);unsigned n=EofPeerNativeCalls++;EofPeerErrors[n]=error;EofPeerTransfers[n]=count;EofPeerCompletingObject=overlap->hEvent;EofPeerNativeCompletion(error,count,overlap);EofPeerCompletingObject=0;
}
static int __stdcall PeerNativeRead(void* file,void* buffer,unsigned long size,_OVERLAPPED* overlap,NativeReadCompletion completion){
 CHECK(!EofPeerNativeCompletion || EofPeerNativeCompletion==completion);EofPeerNativeCompletion=completion;return ReadFileEx(file,buffer,size,overlap,PeerNativeCompletion);
}
static void __fastcall AdmitEofPeer(const char* text){
 CHECK(text && !*text && ++EofPeerHardCalls==1 && EofPeerCompletingObject && EofPeerFile && EofPeerRead);
 unsigned char* object=static_cast<unsigned char*>(EofPeerCompletingObject);unsigned uid=*reinterpret_cast<unsigned*>(object+0x18);CThreadedFileObjectDesc* desc=reinterpret_cast<CThreadedFileObjectDesc*>(object);
 unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);
 CHECK(*reinterpret_cast<unsigned*>(object+0x1c)==2 && EofPeerFile->ObjectExists(uid) && !EofPeerFile->IsObjectFinished(uid));CheckLiveStatistics(1,32,0,0);
 EofPeerFile->SetObjectPriority(uid,THREADED_FILE_PRIORITY_HIGHEST);CHECK(desc->Priority==THREADED_FILE_PRIORITY_HIGH);
 CHECK(!EofPeerFile->RemoveObject(uid));
 EofPeerUID=AddDirect(*EofPeerFile,*EofPeerRead,16,THREADED_FILE_PRIORITY_HIGH);CHECK(EofPeerUID && EofPeerUID!=uid && EofPeerFile->ObjectExists(EofPeerUID));CheckLiveStatistics(2,64,0,0);
}
static void ProbeEofCallbackPeer(const char* source){
 const char* argv[]={0,source};long chars=g_CCharStringInstanceCount_013BD800,wide=g_CWideStringInstanceCount_013BCA20;
 FILE* input=fopen(argv[1],"rb");CHECK(input && !fseek(input,0,SEEK_END));long size=ftell(input);CHECK(size>64 && !fclose(input));
 bool savedDirty;EofHardCallback savedHard;NativeReadAPI savedRead=FixtureReadFileExSlot;
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);savedDirty=EofDirtyDisc;savedHard=EofHardError;NativeSetDirtyPolicy(false);NativeSetHardPolicy(AdmitEofPeer);FixtureReadFileExSlot=PeerNativeRead;}
 {
 CCharString name(argv[1]);CWideString path=name.ToWideString();CThreadedFile file;CHECK(file.Open(path,false) && file.deviceId_==0);DirectRead first,peer;EofPeerFile=&file;EofPeerRead=&peer;
 CThreadedFileObjectDesc desc;desc.PDst=first.Bytes+16;desc.FilePos=size;desc.Size=32;desc.PCallback=&first;desc.QueueWhenFinished=true;desc.Priority=THREADED_FILE_PRIORITY_HIGH;unsigned uid=file.AddObject(desc);CHECK(uid);
 CHECK(WaitForSingleObject(first.Done,5000)==0 && WaitForSingleObject(peer.Done,5000)==0);file.WaitForObjectToFinish(uid);file.WaitForObjectToFinish(EofPeerUID);
 CHECK(EofPeerHardCalls==1 && EofPeerNativeCalls==2 && EofPeerErrors[0]==38 && !EofPeerTransfers[0] && !EofPeerErrors[1] && EofPeerTransfers[1]==32);CHECK(first.Completions==1 && peer.Completions==1);CheckReadPhase(file,uid,true,true);CHECK(!file.ObjectExists(EofPeerUID) && file.IsObjectFinished(EofPeerUID) && !EofDirtyDisc);
 for(unsigned n=0;n<64;++n){CHECK(first.Bytes[n]==0x95);if(n<16 || n>=48)CHECK(peer.Bytes[n]==0x95);}CheckArchiveWindow(argv[1],16,peer.Bytes+16,32);
 file.Close();CheckReadPhase(file,uid,false,true);CHECK(!file.openFlag_ && first.Completions==1 && peer.Completions==1);EofPeerFile=0;EofPeerRead=0;
 }
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);NativeSetDirtyPolicy(savedDirty);NativeSetHardPolicy(savedHard);FixtureReadFileExSlot=savedRead;}
 CHECK(g_CCharStringInstanceCount_013BD800==chars && g_CWideStringInstanceCount_013BCA20==wide);printf("NATIVE_EOF_CALLBACK_PEER_PASS reads=2 hard_callbacks=1 peer_admissions=1 protected_remove=1 protected_priority=1 cleanup=1\n");
}

static unsigned PublicPolicyCalls,PublicPolicyMode;
static void __fastcall PublicEofPolicy(const char* text){
 CHECK(text && !*text && EofCompletingObject && *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(EofCompletingObject)+0x1c)==2);++PublicPolicyCalls;
 if(PublicPolicyMode==2)NativeSetHardPolicy(0);
}
static void ProbePublicEofPolicy(const char* source){
 const char* argv[]={0,source};long chars=g_CCharStringInstanceCount_013BD800,wide=g_CWideStringInstanceCount_013BCA20;
 FILE* input=fopen(argv[1],"rb");CHECK(input && !fseek(input,0,SEEK_END));long size=ftell(input);CHECK(size>64 && !fclose(input));
 bool savedDirty;EofHardCallback savedHard;NativeReadAPI savedRead=FixtureReadFileExSlot;
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);savedDirty=EofDirtyDisc;savedHard=EofHardError;FixtureReadFileExSlot=EofRead;}
 for(unsigned policy=0;policy<3;++policy){
  PublicPolicyMode=policy;PublicPolicyCalls=0;
  {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);NativeSetHardPolicy(policy?PublicEofPolicy:0);NativeSetDirtyPolicy(true);CHECK(EofDirtyDisc);NativeSetDirtyPolicy(false);CHECK(!EofDirtyDisc && EofHardError==(policy?PublicEofPolicy:0));}
  for(unsigned mode=0;mode<3;++mode){
   CCharString name(argv[1]);CWideString path=name.ToWideString();CThreadedFile file;CHECK(file.Open(path,false) && file.deviceId_==0);DirectRead read;EofNativeCompletion=0;EofError=EofTransferred=0xffffffffu;
   CThreadedFileObjectDesc desc;desc.PDst=read.Bytes+16;desc.FilePos=static_cast<unsigned>(size)-(mode==0?13:0)+(mode==2?7:0);desc.Size=32;desc.PCallback=&read;desc.QueueWhenFinished=true;desc.Priority=THREADED_FILE_PRIORITY_HIGH;unsigned uid=file.AddObject(desc);CHECK(uid);
   CHECK(WaitForSingleObject(read.Done,5000)==0);file.WaitForObjectToFinish(uid);CheckReadPhase(file,uid,true,true);
   unsigned expected=policy==1?mode:policy==2 && mode?1u:0u;CHECK(PublicPolicyCalls==expected && !EofDirtyDisc);CHECK(EofHardError==(policy==1 || (policy==2 && !mode)?PublicEofPolicy:0));
   CHECK(EofTransferred==(mode==0?13u:0u) && EofError==(mode==0?0u:38u) && read.Begins==1 && read.Completions==1);
   for(unsigned n=0;n<64;++n)if(n<16 || n>=16+EofTransferred)CHECK(read.Bytes[n]==0x95);if(EofTransferred)CheckArchiveWindow(argv[1],desc.FilePos,read.Bytes+16,EofTransferred);
   file.Close();CheckReadPhase(file,uid,false,true);CHECK(!file.openFlag_ && read.Completions==1);printf("PUBLIC_EOF_POLICY policy=%u mode=%u calls=%u error=%u bytes=%u\n",policy,mode,PublicPolicyCalls,EofError,EofTransferred);
  }
 }
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);NativeSetDirtyPolicy(savedDirty);NativeSetHardPolicy(savedHard);FixtureReadFileExSlot=savedRead;}
 EofNativeCompletion=0;CHECK(g_CCharStringInstanceCount_013BD800==chars && g_CWideStringInstanceCount_013BCA20==wide);printf("NATIVE_PUBLIC_EOF_POLICY_PASS policies=3 reads=9 callback_mutation=1 cleanup=1\n");
}

static unsigned BankEofUID;
static int __stdcall BankEofRead(void* file,void* buffer,unsigned long size,_OVERLAPPED* overlap,NativeReadCompletion completion){BankEofUID=*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(overlap->hEvent)+0x18);return EofRead(file,buffer,size,overlap,completion);}
static void ProbeBankCallerEof(const char* source){
 const char* argv[]={0,source};long chars=g_CCharStringInstanceCount_013BD800,wide=g_CWideStringInstanceCount_013BCA20;
 FILE* input=fopen(argv[1],"rb");CHECK(input && !fseek(input,0,SEEK_END));long size=ftell(input);CHECK(size>64 && !fclose(input));
 bool savedDirty;EofHardCallback savedHard;NativeReadAPI savedRead=FixtureReadFileExSlot;
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);savedDirty=EofDirtyDisc;savedHard=EofHardError;NativeSetHardPolicy(0);FixtureReadFileExSlot=BankEofRead;}
 {
 CCharString filename(argv[1]);CWideString path=filename.ToWideString();CBankFileManager& manager=g_FableBankFileManager_013CA79C;manager.UseRetailBanks=true;manager.OpenRetailBank(path,false);
 {
 NGameText::CDataBank bank;CCharString name("RESOURCE_TEST");CHECK(bank.OpenReadOnly(name,8|64|256) && !bank.NonCached && !bank.DisableAsync && bank.ThreadedFile->deviceId_==1);
 {
 CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(1);CHECK(entry.Data);unsigned base=bank.GetEntryDataOffset(1);CHECK(base<static_cast<unsigned>(size));
 for(unsigned mode=0;mode<3;++mode){
  unsigned char bytes[64];memset(bytes,0x95,sizeof(bytes));EofNativeCompletion=0;EofError=EofTransferred=0xffffffffu;NativeSetDirtyPolicy(false);
  unsigned absolute=static_cast<unsigned>(size)-(mode==0?13:0)+(mode==2?7:0);
  {
  CCountedPointer<CBankFileAsyncData> data=entry->BeginReadData(absolute-base,bytes+16,32,32,THREADED_FILE_PRIORITY_HIGH);CHECK(data.Data && data->Data==bytes+16 && data->Buffer==bytes+16 && !data->InternalAllocatedBufferSize);
  CHECK(WaitPublished(*data));CHECK(BankEofUID);bank.ThreadedFile->WaitForObjectToFinish(BankEofUID);CHECK(!bank.ThreadedFile->ObjectExists(BankEofUID));CHECK(data->GetData()==bytes+16 && data->Finished && data->Length==32 && !data->AllowRestart && data->MemoryHandle==-1 && !data->Aborted && data->Sizeof()==sizeof(CBankFileAsyncData));
  CHECK(EofTransferred==(mode==0?13u:0u) && EofError==(mode==0?0u:38u) && EofDirtyDisc==(mode!=0));CHECK(bank.AsyncDataInstances.size()==1 && bank.ReadsToRestart.empty() && bank.DisposalList.empty());
  CHECK(data->LoadDesc.FilePos==absolute && data->LoadDesc.Size==32 && data->LoadDesc.PCallback==data.Data && !data->LoadDesc.QueueWhenFinished);
  for(unsigned n=0;n<64;++n)if(n<16 || n>=16+EofTransferred)CHECK(bytes[n]==0x95);if(EofTransferred)CheckArchiveWindow(argv[1],absolute,bytes+16,EofTransferred);
  // Join the worker after bank publication before releasing caller storage.
  unsigned char* nativeManager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(nativeManager+0x14c),false);CheckLiveStatistics(0,0,0,0);
  }
  CHECK(bank.AsyncDataInstances.empty() && bank.ReadsToRestart.empty() && bank.DisposalList.empty());printf("BANK_CALLER_EOF mode=%u error=%u bytes=%u published=1 cleanup=1\n",mode,EofError,EofTransferred);
 }
 }
 CHECK(bank.AsyncDataInstances.empty() && !bank.ResourceList.ResourceCount);bank.Close();bank.Close();
 }
 manager.Close();manager.Close();
 }
 g_CAFileCurrentPath_013BCA10=L"";
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);NativeSetDirtyPolicy(savedDirty);NativeSetHardPolicy(savedHard);FixtureReadFileExSlot=savedRead;}
 EofNativeCompletion=0;CHECK(g_CCharStringInstanceCount_013BD800==chars && g_CWideStringInstanceCount_013BCA20==wide);printf("NATIVE_BANK_CALLER_EOF_PASS reads=3 external_bytes=64 decoded=0 cleanup=1\n");
}

static int __stdcall AllocatedEofRead(void* file,void* buffer,unsigned long size,_OVERLAPPED* overlap,NativeReadCompletion completion){
 CHECK(buffer && size==32);memset(buffer,0x95,size);BankEofUID=*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(overlap->hEvent)+0x18);int result=EofRead(file,buffer,size,overlap,completion);CHECK(result && SetEvent(ReadIssued) && WaitForSingleObject(ReleaseRead,5000)==0);return result;
}
static void ProbeAllocatedEofDisposal(const char* source){
 const char* argv[]={0,source};long chars=g_CCharStringInstanceCount_013BD800,wide=g_CWideStringInstanceCount_013BCA20;
 FILE* input=fopen(argv[1],"rb");CHECK(input && !fseek(input,0,SEEK_END));long size=ftell(input);CHECK(size>64 && !fclose(input));
 bool savedDirty;EofHardCallback savedHard;NativeReadAPI savedRead=FixtureReadFileExSlot;
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);savedDirty=EofDirtyDisc;savedHard=EofHardError;NativeSetHardPolicy(0);FixtureReadFileExSlot=AllocatedEofRead;}
 {
 CCharString filename(argv[1]);CWideString path=filename.ToWideString();CBankFileManager& manager=g_FableBankFileManager_013CA79C;manager.UseRetailBanks=true;manager.OpenRetailBank(path,false);
 {
 NGameText::CDataBank bank;CCharString name("RESOURCE_TEST");CHECK(bank.OpenReadOnly(name,8|64|256) && !bank.NonCached && !bank.DisableAsync && bank.Alignment==1 && bank.ThreadedFile->deviceId_==1);
 {
 CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(1);CHECK(entry.Data);unsigned base=bank.GetEntryDataOffset(1);
 for(unsigned pooled=0;pooled<2;++pooled){
  __declspec(align(16)) unsigned char storage[4096];memset(storage,0x67,sizeof(storage));CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);pool->InitialiseFromPreallocatedBuffer(storage,sizeof(storage),true);if(pooled)bank.SetLoadingMemoryPool(pool);
  for(unsigned mode=0;mode<3;++mode){
   EofNativeCompletion=0;EofError=EofTransferred=0xffffffffu;NativeSetDirtyPolicy(false);ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead);
   unsigned absolute=static_cast<unsigned>(size)-(mode==0?13:0)+(mode==2?7:0);
   CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(absolute-base,32,THREADED_FILE_PRIORITY_HIGH);CHECK(WaitForSingleObject(ReadIssued,5000)==0 && data.Data && !data->Finished && data.Info->RefCount==1);
   CHECK(data->Buffer && data->Data==data->Buffer && data->Length==32 && !data->SkipBytes);CHECK(pooled?(data->MemoryHandle!=-1 && pool->AllocatedSize>0 && !data->InternalAllocatedBufferSize):(data->MemoryHandle==-1 && data->InternalAllocatedBufferSize==32 && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==1));
   CBankFileAsyncData* held=data.Data;bank.DiscardAsyncData(data);CHECK(data.IsNull() && bank.DisposalList.size()==1 && bank.DisposalList.front().Data==held && bank.DisposalList.front().Info->RefCount==1);bank.AsyncUpdatePending();CHECK(!held->Finished && bank.AsyncDataInstances.size()==1 && bank.DisposalList.size()==1);
   CHECK(SetEvent(ReleaseRead) && WaitPublished(*held));CHECK(BankEofUID);bank.ThreadedFile->WaitForObjectToFinish(BankEofUID);CHECK(!bank.ThreadedFile->ObjectExists(BankEofUID));
   CHECK(EofTransferred==(mode==0?13u:0u) && EofError==(mode==0?0u:38u) && EofDirtyDisc==(mode!=0));CHECK(held->Finished && held->Length==32 && held->GetData()==held->Buffer && !held->AllowRestart && bank.DisposalList.size()==1);
   unsigned char* bytes=static_cast<unsigned char*>(held->GetData());for(unsigned n=EofTransferred;n<32;++n)CHECK(bytes[n]==0x95);if(EofTransferred)CheckArchiveWindow(argv[1],absolute,bytes,EofTransferred);
   bank.AsyncUpdatePending();CHECK(bank.DisposalList.empty() && bank.AsyncDataInstances.empty() && bank.ReadsToRestart.empty() && !pool->AllocatedSize && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
   CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead));ReadIssued=ReleaseRead=0;printf("ALLOCATED_EOF_DISPOSAL pooled=%u mode=%u error=%u bytes=%u retained=1 freed=1\n",pooled,mode,EofError,EofTransferred);
  }
  if(pooled)bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());CHECK(pool.Info->RefCount==1);
 }
 }
 CHECK(bank.AsyncDataInstances.empty());bank.Close();bank.Close();
 }
 manager.Close();manager.Close();
 }
 g_CAFileCurrentPath_013BCA10=L"";
 {unsigned char* manager=reinterpret_cast<unsigned char*>(&g_FableThreadedFileManager_013BC870);CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(manager+0x14c),false);NativeSetDirtyPolicy(savedDirty);NativeSetHardPolicy(savedHard);FixtureReadFileExSlot=savedRead;}
 CHECK(g_CCharStringInstanceCount_013BD800==chars && g_CWideStringInstanceCount_013BCA20==wide && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());FableMemoryAllocatorShutdown_009f2d20();CHECK(!g_FableMemoryAllocator_013CAA90);EofNativeCompletion=0;printf("NATIVE_ALLOCATED_EOF_DISPOSAL_PASS heap=3 pool=3 decoded=0 cleanup=1\n");
}

int main(int argc,char** argv){CHECK(argc==2 || (argc==3 && (!strcmp(argv[2],"--seek-exception") || !strcmp(argv[2],"--priority") || !strcmp(argv[2],"--file-keys") || !strcmp(argv[2],"--devices") || !strcmp(argv[2],"--writes") || !strcmp(argv[2],"--pending-write") || !strcmp(argv[2],"--written-archive") || !strcmp(argv[2],"--read-phases") || !strcmp(argv[2],"--close-read") || !strcmp(argv[2],"--descriptor-meta") || !strcmp(argv[2],"--eof") || !strcmp(argv[2],"--eof-harddisk") || !strcmp(argv[2],"--eof-callback-peer") || !strcmp(argv[2],"--eof-public-policy") || !strcmp(argv[2],"--bank-caller-eof") || !strcmp(argv[2],"--bank-allocated-eof-disposal"))));unsigned loads=0,passes=0;
 if(argc==3 && !strcmp(argv[2],"--bank-allocated-eof-disposal")){ProbeAllocatedEofDisposal(argv[1]);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--bank-caller-eof")){ProbeBankCallerEof(argv[1]);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--eof-public-policy")){const char* source=getenv("FABLE_NATIVE_EOF_HARDDISK_SOURCE");CHECK(source && source[0]);ProbePublicEofPolicy(source);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--eof-callback-peer")){const char* source=getenv("FABLE_NATIVE_EOF_HARDDISK_SOURCE");CHECK(source && source[0]);ProbeEofCallbackPeer(source);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--eof-harddisk")){const char* source=getenv("FABLE_NATIVE_EOF_HARDDISK_SOURCE");CHECK(source && source[0]);ProbeEof(source,0);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--eof")){ProbeEof(argv[1]);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--descriptor-meta")){const char* pair=getenv("FABLE_NATIVE_DESCRIPTOR_PAIR");CHECK(pair);ProbeDescriptorMetadata(argv[1],atoi(pair));fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--close-read")){const char* phase=getenv("FABLE_NATIVE_CLOSE_READ_AT");CHECK(phase);unsigned closeAt=atoi(phase);CHECK(closeAt>=1 && closeAt<=3);ProbeCloseReadPhases(argv[1],closeAt);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--read-phases")){ProbeReadPhases(argv[1]);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--written-archive")){const char* source=getenv("FABLE_NATIVE_ROUNDTRIP_SOURCE");CHECK(source && source[0]);ProbeWrittenArchive(source,argv[1]);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--pending-write")){ProbeCloseWrites(argv[1]);fflush(stdout);ExitProcess(0);}
 if(argc==3 && !strcmp(argv[2],"--writes")){ProbeWrites(argv[1]);fflush(stdout);ExitProcess(0);}
 {CCharString filename(argv[1]);CWideString path=filename.ToWideString();CBankFileManager& manager=g_FableBankFileManager_013CA79C;manager.UseRetailBanks=true;manager.OpenRetailBank(path,false);
 {NGameText::CDataBank bank;CCharString name("RESOURCE_TEST");CResourceList& list=bank.ResourceList;
 if(argc==3){CHECK(bank.OpenReadOnly(name,8|64|256));if(!strcmp(argv[2],"--seek-exception"))ProbeNativeFileSeekExceptions(bank.BankFile.Data,argv[1]);else if(!strcmp(argv[2],"--devices")){const char* reference=getenv("FABLE_NATIVE_DEVICE_REFERENCE");CHECK(reference && reference[0]);for(unsigned level=1;level<7;++level){ProbeDevices(path,argv[1],reference,false,static_cast<EThreadedFilePriority>(level));ProbeDevices(path,argv[1],reference,true,static_cast<EThreadedFilePriority>(level));}printf("ALL_PRIORITY_DEVICE_CHILD_PASS reads=60 devices=2 priorities=6 modes=2 restored=12 cleanup=1\n");fflush(stdout);ExitProcess(0);}else if(!strcmp(argv[2],"--file-keys")){ProbeFileKeys(path,argv[1]);printf("NATIVE_FILE_KEYS_CHILD_PASS reads=4\n");fflush(stdout);ExitProcess(0);}else{ProbePriority(bank,argv[1],0);ProbePriority(bank,argv[1],1);ProbePriority(bank,argv[1],2);ProbePriority(bank,argv[1],3);printf("NATIVE_PRIORITY_CHILD_PASS reads=16\n");fflush(stdout);ExitProcess(0);}CHECK(false);}
 for(unsigned delay=0;delay<2;++delay){list.SetMaximumMemory(0x7fffffff);list.SetResourceUnloadDelay(delay);CHECK(bank.OpenReadOnly(name,8|64|256));unsigned long sizes[65],addresses[65],total=0;
 for(unsigned index=1;index<=64;++index){
  if(index%2){CIVCountedPointer<const NGameText::CTextEntry> value=bank.GetPTextEntry(index,true);CHECK(!value.IsNull());CheckText(*value.data_,(index-1)/2);sizes[index]=value->ResourceSize;addresses[index]=(unsigned long)value.data_;}
  else{CIVCountedPointer<const NGameText::CGroupEntry> value=bank.GetPGroupEntry(index,true);CHECK(!value.IsNull());CheckGroup(*value.data_,(index-1)/2);sizes[index]=value->ResourceSize;addresses[index]=(unsigned long)value.data_;}total+=sizes[index];++loads;
 }
 CHECK(list.ResourceCount==64 && list.AllocatedMemory==total);CheckList(list);CheckOwners(bank,0,addresses);list.SetMaximumMemory(0);unsigned removed=0;
 if(delay){CHECK(list.ResourceCount==64 && list.AllocatedMemory==total && list.UnloadedThisFrame==64);CHECK(!list.MakeFreeSpace(0));CHECK(list.ResourceCount==64 && list.UnloadedThisFrame==64);CheckOwners(bank,0,addresses);list.FrameEnd();CHECK(!list.UnloadedThisFrame);}
 else{removed=32;for(unsigned i=1;i<=removed;++i)total-=sizes[i];++passes;CHECK(list.ResourceCount==32 && list.AllocatedMemory==total);CheckList(list);CheckOwners(bank,removed,addresses);}
 while(removed<64){unsigned before=64-removed,drop=(before+1)/2;for(unsigned i=removed+1;i<=removed+drop;++i)total-=sizes[i];removed+=drop;bool result=list.MakeFreeSpace(0);CHECK(result==(removed==64));CHECK(list.ResourceCount==64-removed && list.AllocatedMemory==total);CheckList(list);CheckOwners(bank,removed,addresses);++passes;printf("EVICT\t%u\t%u\t%lu\n",delay,64-removed,total);}
 CHECK(!total && list.ResourceCount==0 && list.AllocatedMemory==0 && bank.AsyncDataInstances.empty() && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==0);CHECK(!list.UnloadNextResource(0));bank.Close();bank.Close();
 }
 list.SetMaximumMemory(0x7fffffff);list.SetResourceUnloadDelay(1);CHECK(bank.OpenReadOnly(name,8|64|256));
 {CIVCountedPointer<const NGameText::CTextEntry> text=bank.GetPTextEntry(1,true);CIVCountedPointer<const NGameText::CGroupEntry> group=bank.GetPGroupEntry(2,true);CHECK(text->IVRefCount==2 && group->IVRefCount==2);unsigned total=text->ResourceSize+group->ResourceSize;CHECK(list.ResourceCount==2 && list.AllocatedMemory==total);list.FrameEnd();CHECK(list.UnloadNextResource(total+1) && list.UnloadNextResource(total+1) && !list.UnloadNextResource(0));CHECK(!bank.GetTextInfo(1)->PText.IsNull() && !bank.GetGroupInfo(2)->PGroup.IsNull() && list.UnloadedThisFrame==2 && text->IVRefCount==2 && group->IVRefCount==2);list.FrameEnd();list.SetMaximumMemory(0);CHECK(bank.GetTextInfo(1)->PText.IsNull() && bank.GetGroupInfo(2)->PGroup.IsNull() && text->IVRefCount==1 && group->IVRefCount==1 && list.ResourceCount==2 && list.AllocatedMemory==total && list.UnloadedThisFrame==2);CheckList(list);CheckText(*text.data_,0);CheckGroup(*group.data_,0);CHECK(!list.MakeFreeSpace(0));text.Release();CHECK(list.ResourceCount==1 && list.AllocatedMemory==group->ResourceSize);list.FrameEnd();CHECK(!list.MakeFreeSpace(0) && group->IVRefCount==1 && list.UnloadedThisFrame==1);CheckGroup(*group.data_,0);CheckList(list);group.Release();CHECK(list.ResourceCount==0 && list.AllocatedMemory==0 && list.MakeFreeSpace(0));CHECK(!list.UnloadNextResource(0));loads+=2;
 }bank.Close();bank.Close();CHECK(bank.AsyncDataInstances.empty());
 // Hold the bank lock on the submitting thread: the real worker cannot finish
 // either read, while recursive owner-thread accessors remain available.
 list.SetMaximumMemory(0x7fffffff);list.SetResourceUnloadDelay(0);CHECK(bank.OpenReadOnly(name,8|64|256));
 {
  CCriticalSectionLock pending(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank.CriticalSection),false);
  BeginNativeText(bank.GetTextInfo(1),0,1,bank);BeginNativeGroup(bank.GetGroupInfo(2),0,2,bank);
  NGameText::CTextEntry* text=bank.GetTextInfo(1)->PText.data_;
  NGameText::CGroupEntry* group=bank.GetGroupInfo(2)->PGroup.data_;
  CHECK(text && group && text->PFileData.Data && group->PFileData.Data);
  CHECK(!text->PFileData->Finished && !group->PFileData->Finished && !text->IsLoaded() && !group->IsLoaded());
  unsigned total=text->ResourceSize+group->ResourceSize;
  CHECK(list.ResourceCount==2 && list.AllocatedMemory==total && bank.AsyncDataInstances.size()==2);
  list.FrameEnd();list.SetMaximumMemory(0);
  CHECK(bank.GetTextInfo(1)->PText.data_==text && bank.GetGroupInfo(2)->PGroup.data_==group);
  CHECK(text->IVRefCount==1 && group->IVRefCount==1 && list.ResourceCount==2 && list.AllocatedMemory==total && list.UnloadedThisFrame==2);
  CHECK(!list.MakeFreeSpace(0));CheckList(list);
  CHECK(!text->PFileData->Finished && !group->PFileData->Finished);
  pending.Release();
  CHECK(WaitPublished(*text->PFileData) && WaitPublished(*group->PFileData));
  // Completion alone allows eviction, without forcing payload decoding.
  list.FrameEnd();CHECK(!list.MakeFreeSpace(0));CHECK(list.ResourceCount==1);
  list.FrameEnd();CHECK(list.MakeFreeSpace(0));CHECK(!list.ResourceCount && !list.AllocatedMemory);
  CHECK(bank.GetTextInfo(1)->PText.IsNull() && bank.GetGroupInfo(2)->PGroup.IsNull());
  CHECK(bank.AsyncDataInstances.empty() && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==0);
 }
 printf("PENDING_EVICTION_PASS reads=2 preserved=2 released=2\n");
 // Populate the native disposal queue with completed real reads. One retained
 // caller handle must survive the drain; the other two are queue-owned only.
 {
  CCountedPointer<CBankFileAsyncData> retained;
  for(unsigned index=1;index<=3;++index){
   CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(index);
   CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(0,bank.GetEntryDataSize(index),g_NativeTextLoadingPriority_01396314);
   CHECK(WaitPublished(*data) && data->Finished && data->GetData());
   CHECK(data.Info->RefCount==1);bank.DisposalList.push_back(data);CHECK(data.Info->RefCount==2);
   if(index==1)retained.ShareData(data.Data,data.Info);
  }
  CHECK(bank.DisposalList.size()==3 && bank.AsyncDataInstances.size()==3 && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==3);
  CHECK(retained.Info->RefCount==2);void* buffer=retained->GetData();unsigned char first=*(unsigned char*)buffer;
  bank.AsyncUpdatePending();
  CHECK(bank.DisposalList.empty() && bank.AsyncDataInstances.size()==1 && retained.Info->RefCount==1);
  CHECK(retained->GetData()==buffer && *(unsigned char*)buffer==first && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==1);
  retained.ReleaseData();CHECK(bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  bank.AsyncUpdatePending();CHECK(bank.DisposalList.empty());
 }
 printf("DISPOSAL_COMPLETED_PASS queued=3 retained=1 released=3\n");
 list.SetMaximumMemory(0x7fffffff);
 {
  CCriticalSectionLock pending(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank.CriticalSection),false);
  CCountedPointer<CBankFileAsyncEntry> first=bank.GetAsyncEntry(1),second=bank.GetAsyncEntry(2);
  CCountedPointer<CBankFileAsyncData> a=first->BeginReadDataAutoAlloc(0,bank.GetEntryDataSize(1),g_NativeTextLoadingPriority_01396314);
  CCountedPointer<CBankFileAsyncData> b=second->BeginReadDataAutoAlloc(0,bank.GetEntryDataSize(2),g_NativeTextLoadingPriority_01396314);
  QueuedRead one(bank,a),two(bank,b);unsigned total=one.ResourceSize+two.ResourceSize;
  bank.AddToLoadingQueue(one);bank.AddToLoadingQueue(two);
  CHECK(bank.LoadingQueue.ResourceCount==2 && bank.LoadingQueue.AllocatedMemory==total);CheckList(bank.LoadingQueue);
  bank.ProcessLoadingQueue(false,0);
  CHECK(!one.Notifications && !two.Notifications && !list.ResourceCount && bank.LoadingQueue.ResourceCount==2);
  CHECK(!a->Finished && !b->Finished);pending.Release();CHECK(WaitPublished(*a) && WaitPublished(*b));
  bank.ProcessLoadingQueue(true,-1.0);
  CHECK(!one.Notifications && !two.Notifications && bank.LoadingQueue.ResourceCount==2 && !list.ResourceCount);
  bank.ProcessLoadingQueue(false,0);
  CHECK(one.Notifications==1 && two.Notifications==1 && !bank.LoadingQueue.ResourceCount && !bank.LoadingQueue.AllocatedMemory);
  CHECK(list.ResourceCount==2 && list.AllocatedMemory==total && one.ResourceList==&list && two.ResourceList==&list);
  CheckList(bank.LoadingQueue);CheckList(list);bank.ProcessLoadingQueue(false,0);CHECK(one.Notifications==1 && two.Notifications==1);
 }
 CHECK(!list.ResourceCount && !list.AllocatedMemory && bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
 printf("READ_LOADING_QUEUE_PASS pending=2 deadline_stops=1 notifications=2\n");
 // Exercise the logical noncached alignment path on an ordinary cached OS
 // handle. Expected bytes are checked independently against the archive.
 unsigned windows=0;bool savedNonCached=bank.NonCached;
 for(unsigned mode=0;mode<2;++mode){bank.NonCached=mode!=0;
  for(unsigned index=1;index<=4;++index){unsigned size=bank.GetEntryDataSize(index);unsigned offsets[]={0,1,bank.Alignment-1,bank.Alignment,bank.Alignment+1,size-1};
   for(unsigned n=0;n<6;++n){unsigned offset=offsets[n];if(offset>=size)continue;unsigned count=size-offset;if(count>17)count=17;
    CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(index);
    CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(offset,count,g_NativeTextLoadingPriority_01396314);
    CHECK(WaitPublished(*data));unsigned skip=mode?offset%bank.Alignment:0;
    CHECK(data->Length==count && data->SkipBytes==skip && data->InternalAllocatedBufferSize==bank.Align(count+skip));
    // AddObject may finish before SetupRead stores its returned UID. Retail
    // can retain that completed UID; buffer validity and ownership are stable.
    CHECK(data->Data==(char*)data->Buffer+skip && data.Info->RefCount==1);
    printf("ALIGNED_READ\t%lu\t%u\t%u\t%u\t%u\t",bank.Alignment,mode,index,offset,count);
    unsigned char* bytes=(unsigned char*)data->GetData();for(unsigned k=0;k<count;++k)printf("%02x",bytes[k]);printf("\n");++windows;
   }
  }
 }
 bank.NonCached=savedNonCached;CHECK(bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
 printf("ALIGNED_READ_PASS alignment=%lu windows=%u\n",bank.Alignment,windows);
 for(unsigned scenario=0;scenario<3;++scenario){
  __declspec(align(16)) unsigned char storage[4096];
  CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);
  unsigned poolSize=scenario==0?4096:scenario==1?64:4096;
  pool->InitialiseFromPreallocatedBuffer(storage,poolSize,true);PoolFailure failure(*pool,scenario==2);pool->FailureHandler=&failure;
  if(scenario==2)CHECK(pool->Allocate(&failure.Blocker,4000,2));
  bank.SetLoadingMemoryPool(pool);CHECK(pool.Info->RefCount==2);
  {
   CCriticalSectionLock pending(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank.CriticalSection),false);
   CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(1);
   // A fixed 128-byte window is inside the padded archive, including tiny
   // payloads; these cases verify buffer ownership rather than decoded text.
   CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
   void* output=0;bool began=data->OnFileReadBegin(&output,g_NativeTextLoadingPriority_01396314);
   CHECK(began==(scenario==0) && failure.Calls==0);
   if(scenario==0)CHECK(data->MemoryHandle!=-1 && data->Buffer && !data->InternalAllocatedBufferSize);
   else CHECK(data->MemoryHandle==-1 && !data->Buffer && !data->Data && !data->InternalAllocatedBufferSize);
   pending.Release();CHECK(WaitPublished(*data) && data->GetData());
   CHECK(data->Finished && data->MemoryPool==pool.Data);
   if(scenario==1)CHECK(failure.Calls==1 && data->MemoryHandle==-1 && data->InternalAllocatedBufferSize==bank.Align(128) && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==1);
   else CHECK(failure.Calls==(scenario==2?1:0) && data->MemoryHandle!=-1 && !data->InternalAllocatedBufferSize && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
   if(data->MemoryHandle!=-1)CHECK(pool->InternalGetResource(data->MemoryHandle)==static_cast<CMovableResource*>(data.Data));
   printf("POOL_READ\t%lu\t%u\t",bank.Alignment,scenario);unsigned char* bytes=(unsigned char*)data->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");
  }
  CHECK(!pool->AllocatedSize && bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  pool->FailureHandler=0;bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());CHECK(pool.Info->RefCount==1);
 }
 printf("POOL_READ_PASS success=1 fallback=1 recovered=1\n");
 {
  __declspec(align(16)) unsigned char storage[4096];
  CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);
  CCountedPointer<CLoadingMemoryFailureHandler> handler(new CLoadingMemoryFailureHandler);
  pool->InitialiseFromPreallocatedBuffer(storage,bank.Align(128)+64,true);pool->FailureHandler=handler.Data;
  bank.SetLoadingMemoryPool(pool);bank.SetLoadingMemoryFailureHandler(handler);
  CHECK(handler->Banks.size()==1 && handler->Banks[0]==&bank && handler.Info->RefCount==2);
  {
   CCountedPointer<CBankFileAsyncEntry> first=bank.GetAsyncEntry(1),second=bank.GetAsyncEntry(3);
   CCountedPointer<CBankFileAsyncData> a=first->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
   CHECK(WaitRestartable(*a) && a->MemoryHandle!=-1 && a->Buffer && bank.ReadsToRestart.empty());
   CCriticalSectionLock pending(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank.CriticalSection),false);
   CCountedPointer<CBankFileAsyncData> b=second->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
   void* output=0;CHECK(!b->OnFileReadBegin(&output,g_NativeTextLoadingPriority_01396314));
   CHECK(ForceNativeBuffer(b.Data,0,true));
   CHECK(!a->Finished && !a->Buffer && !a->Data && a->MemoryHandle==-1 && a->InternalAllocatedBufferSize==0);
   CHECK(bank.ReadsToRestart.size()==1 && bank.LastRestartingData==a.Data && b->Buffer && b->MemoryHandle!=-1 && !b->InternalAllocatedBufferSize);
   bank.AsyncUpdatePending();CHECK(bank.ReadsToRestart.empty() && a->ThreadedFileUID);
   pending.Release();CHECK(WaitPublished(*b));
   printf("RESTART_READ\t%lu\t3\t",bank.Alignment);unsigned char* bytes=(unsigned char*)b->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");
   b.ReleaseData();CHECK(WaitPublished(*a));CHECK(a->MemoryHandle!=-1 && !a->InternalAllocatedBufferSize);
   printf("RESTART_READ\t%lu\t1\t",bank.Alignment);bytes=(unsigned char*)a->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");
   CHECK(!g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  }
  CHECK(bank.AsyncDataInstances.empty() && bank.ReadsToRestart.empty() && !pool->AllocatedSize && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  bank.SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler>());CHECK(handler->Banks.empty() && handler.Info->RefCount==1);
  pool->FailureHandler=0;bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());
 }
 printf("NATIVE_POOL_RESTART_PASS reclaimed=1 resubmitted=1 reads=2\n");
 {
  __declspec(align(16)) unsigned char storage[4096];CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);
  pool->InitialiseFromPreallocatedBuffer(storage,sizeof(storage),true);bank.SetLoadingMemoryPool(pool);
  ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead);PauseRead=true;
  CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(1);
  CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
  CHECK(WaitForSingleObject(ReadIssued,5000)==0 && !PauseRead);
  CHECK(!data->Finished && data->ThreadedFileUID && data.Info->RefCount==1 && pool->AllocatedSize>0);
  CBankFileAsyncData* held=data.Data;bank.DiscardAsyncData(data);
  CHECK(data.IsNull() && bank.DisposalList.size()==1 && bank.DisposalList.front().Data==held && bank.DisposalList.front().Info->RefCount==1);
  CHECK(!held->Finished && bank.AsyncDataInstances.size()==1 && pool->AllocatedSize>0);
  bank.AsyncUpdatePending();CHECK(bank.DisposalList.size()==1 && !held->Finished && pool->AllocatedSize>0);
  CHECK(SetEvent(ReleaseRead));CHECK(WaitPublished(*held));
  CHECK(bank.DisposalList.size()==1 && pool->AllocatedSize>0);
  printf("PENDING_DISPOSAL_READ\t%lu\t",bank.Alignment);unsigned char* bytes=(unsigned char*)held->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");
  bank.AsyncUpdatePending();CHECK(bank.DisposalList.empty() && bank.AsyncDataInstances.empty() && !pool->AllocatedSize && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead));ReadIssued=ReleaseRead=0;
  bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());
 }
 printf("PENDING_DISPOSAL_PASS transferred=1 retained=1 drained=1\n");
 {
  __declspec(align(16)) unsigned char storage[8192];memset(storage,0xa5,sizeof(storage));CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);
  pool->InitialiseFromPreallocatedBuffer(storage,sizeof(storage),true);bank.SetLoadingMemoryPool(pool);long hole=-1;CHECK(pool->Allocate(&hole,64,2));
  bool saved=bank.NonCached;bank.NonCached=true;
  {
   CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(1);
   CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(1,127,g_NativeTextLoadingPriority_01396314);
   CHECK(WaitPublished(*data) && data->MemoryHandle!=-1 && !data->BufferLocked && !pool->LockCount);
   void* oldBuffer=data->Buffer;long handle=data->MemoryHandle;unsigned skip=1%bank.Alignment;unsigned char expected[127];memcpy(expected,data->Data,127);
   CHECK(data->CanRelocateData(handle));pool->Free(hole);long allocated=pool->AllocatedSize;pool->Defragment();
   CHECK(data->MemoryHandle==handle && data->Buffer!=oldBuffer && (unsigned long)oldBuffer-(unsigned long)data->Buffer==96);
   CHECK(data->Buffer==pool->GetPointerFromHandle(handle) && data->Data==(char*)data->Buffer+skip && !memcmp(expected,data->Data,127));
   CHECK(pool->InternalGetResource(handle)==static_cast<CMovableResource*>(data.Data) && pool->AllocatedSize==allocated && !pool->LockCount);
   unsigned char* bytes=(unsigned char*)data->GetData();CHECK(data->BufferLocked && pool->LockCount==1 && !data->CanRelocateData(handle));void* lockedBuffer=data->Buffer;
   long secondHole=-1;CHECK(pool->Allocate(&secondHole,64,2));CCountedPointer<CBankFileAsyncEntry> second=bank.GetAsyncEntry(3);
   CCountedPointer<CBankFileAsyncData> other=second->BeginReadDataAutoAlloc(1,127,g_NativeTextLoadingPriority_01396314);CHECK(WaitPublished(*other) && !other->BufferLocked && other->CanRelocateData(other->MemoryHandle));
   void* otherBuffer=other->Buffer;pool->Free(secondHole);pool->Defragment();CHECK(data->Buffer==lockedBuffer && other->Buffer==otherBuffer && pool->LockCount==1);
   printf("LOCKED_RELOCATION_READ\t%lu\t",bank.Alignment);unsigned char* heldBytes=(unsigned char*)other->Data;for(unsigned k=0;k<127;++k)printf("%02x",heldBytes[k]);printf("\n");
   printf("RELOCATED_READ\t%lu\t",bank.Alignment);for(unsigned k=0;k<127;++k)printf("%02x",bytes[k]);printf("\n");
  }
  // Retail Free clears the header lock but retains this pool counter until
  // Uninitialise. Do not repair that observable accounting in recreation.
  CHECK(!pool->AllocatedSize && pool->LockCount==1 && bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  bank.NonCached=saved;bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());
 }
 printf("POOL_RELOCATION_PASS moved=1 locked_stops=1\n");
 {
  __declspec(align(16)) unsigned char storage[8192];memset(storage,0x96,sizeof(storage));CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);
  unsigned chunk=(bank.Align(128)+15)/16*16,front=chunk/2,capacity=front+2*chunk+64+4*32;
  pool->InitialiseFromPreallocatedBuffer(storage,capacity,true);bank.SetLoadingMemoryPool(pool);bool saved=bank.NonCached;bank.NonCached=false;
  long firstHole=-1,secondHole=-1,tail=-1;CHECK(pool->Allocate(&firstHole,front,2));
  ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead);PauseRead=true;
  {
   CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(1);
   CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
   CHECK(WaitForSingleObject(ReadIssued,5000)==0 && !data->Finished);
   CHECK(pool->Allocate(&secondHole,chunk,2) && pool->Allocate(&tail,64,2));CHECK(pool->AllocatedSize==pool->PoolSize);
   pool->Free(firstHole);pool->Free(secondHole);void* oldBuffer=data->Buffer;long handle=data->MemoryHandle;
   CHECK(!data->CanRelocateData(handle) && pool->LargestFreeArea==(long)chunk);
   CHECK(!pool->PartialDefragment(chunk+1) && data->Buffer==oldBuffer && !data->Finished);
   CHECK(SetEvent(ReleaseRead) && WaitPublished(*data));CHECK(data->CanRelocateData(handle));
   CHECK(pool->PartialDefragment(chunk+1) && data->Buffer!=oldBuffer && data->MemoryHandle==handle);
   CHECK(data->Buffer==pool->GetPointerFromHandle(handle) && data->Data==data->Buffer && pool->LargestFreeArea>=(long)chunk+1);
   CHECK(pool->InternalGetResource(handle)==static_cast<CMovableResource*>(data.Data));
   printf("PARTIAL_RELOCATION_READ\t%lu\t",bank.Alignment);unsigned char* bytes=(unsigned char*)data->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");
  }
  pool->Free(tail);CHECK(!pool->AllocatedSize && pool->LockCount==1 && bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead));ReadIssued=ReleaseRead=0;
  bank.NonCached=saved;bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());
 }
 printf("PARTIAL_RELOCATION_PASS pending_refused=1 completed_moved=1\n");
 for(unsigned kind=0;kind<2;++kind){
  __declspec(align(16)) unsigned char storage[8192];memset(storage,0x7d,sizeof(storage));CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);
  pool->InitialiseFromPreallocatedBuffer(storage,sizeof(storage),true);bank.SetLoadingMemoryPool(pool);list.SetMaximumMemory(0x7fffffff);list.SetResourceUnloadDelay(0);
  long hole=-1;CHECK(pool->Allocate(&hole,64,2));CBankFileAsyncData* read;
  {
   CCriticalSectionLock pending(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank.CriticalSection),false);
   if(!kind){BeginNativeText(bank.GetTextInfo(3),0,3,bank);read=bank.GetTextInfo(3)->PText->PFileData.Data;}
   else{BeginNativeGroup(bank.GetGroupInfo(4),0,4,bank);read=bank.GetGroupInfo(4)->PGroup->PFileData.Data;}
   CHECK(read && !read->Finished);
  }
  CHECK(WaitPublished(*read) && read->MemoryHandle!=-1 && !read->BufferLocked);void* oldBuffer=read->Buffer;pool->Free(hole);pool->Defragment();CHECK(read->Buffer!=oldBuffer);
  unsigned stale=0;
  if(!kind){
   CIVCountedPointer<const NGameText::CTextEntry> value=bank.GetPTextEntry(3,true);CHECK(value.data_ && value->IVRefCount==2 && value->PFileData.IsNull());CheckText(*value.data_,1);stale=value->ResourceSize-value->Sizeof();CHECK(list.AllocatedMemory==value->ResourceSize);
  }else{
   CIVCountedPointer<const NGameText::CGroupEntry> value=bank.GetPGroupEntry(4,true);CHECK(value.data_ && value->IVRefCount==2 && value->PFileData.IsNull());CheckGroup(*value.data_,1);stale=value->ResourceSize-value->Sizeof();CHECK(list.AllocatedMemory==value->ResourceSize);
  }
  CHECK(stale==sizeof(CBankFileAsyncData) && !pool->AllocatedSize && pool->LockCount==1 && bank.AsyncDataInstances.empty());
  CHECK(list.ResourceCount==1);CheckList(list);printf("POOLED_DECODE\t%lu\t%u\t%u\n",bank.Alignment,kind,stale);
  list.FrameEnd();list.SetMaximumMemory(0);CHECK(!list.ResourceCount && !list.AllocatedMemory && bank.GetTextInfo(3)->PText.IsNull() && bank.GetGroupInfo(4)->PGroup.IsNull());
  CHECK(!g_FableMemoryAllocator_013CAA90->GetNoAllocations());bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());
 }
 printf("POOLED_DECODE_PASS text=1 group=1 stale_async_bytes=72\n");
 for(unsigned direction=0;direction<2;++direction){
  __declspec(align(16)) unsigned char storage[4096];CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);CCountedPointer<CLoadingMemoryFailureHandler> handler(new CLoadingMemoryFailureHandler);
  pool->InitialiseFromPreallocatedBuffer(storage,bank.Align(128)+64,true);pool->FailureHandler=handler.Data;bank.SetLoadingMemoryPool(pool);bank.SetLoadingMemoryFailureHandler(handler);
  CCountedPointer<CBankFileAsyncData> retained;unsigned char savedBytes[128];
  {
   NGameText::CDataBank other;CHECK(other.OpenReadOnly(name,8|64|256));other.SetLoadingMemoryPool(pool);other.SetLoadingMemoryFailureHandler(handler);
   CHECK(handler->Banks.size()==2 && handler->Banks[0]==&bank && handler->Banks[1]==&other && handler.Info->RefCount==3 && pool.Info->RefCount==3);
   CHECK(other.ThreadedFile.control_==bank.ThreadedFile.control_ && bank.ThreadedFile.control_->RefCount==3);
   CBankFileAsync& victim=direction?static_cast<CBankFileAsync&>(other):static_cast<CBankFileAsync&>(bank);
   CBankFileAsync& requester=direction?static_cast<CBankFileAsync&>(bank):static_cast<CBankFileAsync&>(other);
   CCountedPointer<CBankFileAsyncEntry> first=victim.GetAsyncEntry(1),second=requester.GetAsyncEntry(3);
   retained=first->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);CHECK(WaitRestartable(*retained));
   {
    CCriticalSectionLock pending(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(requester.CriticalSection),false);
    CCountedPointer<CBankFileAsyncData> data=second->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
    CHECK(ForceNativeBuffer(data.Data,0,true));CHECK(victim.ReadsToRestart.size()==1 && requester.ReadsToRestart.empty() && !retained->Finished && !retained->Buffer && data->MemoryHandle!=-1 && !data->InternalAllocatedBufferSize);
    requester.AsyncUpdatePending();CHECK(victim.ReadsToRestart.size()==1 && requester.ReadsToRestart.empty());pending.Release();CHECK(WaitPublished(*data));
    printf("CROSS_BANK_READ\t%lu\t%u\t3\t",bank.Alignment,direction);unsigned char* bytes=(unsigned char*)data->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");
   }
   victim.AsyncUpdatePending();CHECK(victim.ReadsToRestart.empty() && WaitPublished(*retained) && retained->MemoryHandle!=-1 && !retained->InternalAllocatedBufferSize);
   memcpy(savedBytes,retained->GetData(),128);printf("CROSS_BANK_READ\t%lu\t%u\t1\t",bank.Alignment,direction);for(unsigned k=0;k<128;++k)printf("%02x",savedBytes[k]);printf("\n");
   if(direction)retained.ReleaseData(); // Never leave this bank's own data alive at destruction.
   other.Close();other.Close();CHECK(handler->Banks.size()==2 && bank.ThreadedFile.control_->RefCount==3);
  }
  CHECK(handler->Banks.size()==1 && handler->Banks[0]==&bank && handler.Info->RefCount==2 && pool.Info->RefCount==2 && bank.ThreadedFile.control_->RefCount==2);
  if(!direction){CHECK(retained->Finished && retained.Info->RefCount==1 && !memcmp(retained->GetData(),savedBytes,128));retained.ReleaseData();}
  CHECK(bank.AsyncDataInstances.empty() && bank.ReadsToRestart.empty() && !pool->AllocatedSize && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  bank.SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler>());CHECK(handler->Banks.empty() && handler.Info->RefCount==1);pool->FailureHandler=0;bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());
 }
 printf("CROSS_BANK_RESTART_PASS directions=2 reads=4 retained_across_peer=1\n");
 CHECK(!PauseRead && !ReadIssued && !ReleaseRead && bank.AsyncDataInstances.empty());unsigned nullCases=0,retries=0;
 for(unsigned force=0;force<2;++force)for(unsigned lengthCase=0;lengthCase<2;++lengthCase)for(unsigned skipCase=0;skipCase<2;++skipCase){
  unsigned length=lengthCase?127:0,skip=skipCase?3:0,aligned=bank.Align(length+skip),requested=(aligned?aligned:1)+20;
  CBankFileAsyncData* raw=static_cast<CBankFileAsyncData*>(::operator new(sizeof(CBankFileAsyncData)));CHECK(ConstructNativeAllocatedData(raw,0,length,skip,0,bank)==raw);CCountedPointer<CBankFileAsyncData> data(raw);
  CHECK(!data->ThreadedFileUID && !data->Buffer && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());NullAllocator policy;policy.Expected=requested;
  {
   CCriticalSectionLock bankLock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank.CriticalSection),false);CCriticalSectionLock allocatorLock(g_CMemoryAllocatorCriticalSection_013CAA70,false);
   unsigned char snapshot[sizeof(CMemoryAllocator)];memcpy(snapshot,g_FableMemoryAllocator_013CAA90,sizeof(snapshot));
   // Verified retail private offsets: category-zero max fixed size +424 and
   // its size-to-allocator table pointer +460. Change only one virtual leaf.
   long maximum;CAllocSizeToAllocator* table;memcpy(&maximum,(char*)g_FableMemoryAllocator_013CAA90+0x424,4);memcpy(&table,(char*)g_FableMemoryAllocator_013CAA90+0x460,4);CHECK(maximum>=0 && table);unsigned index=requested;if(index>(unsigned long)maximum+1)index=maximum+1;
   CMemoryAllocatorBase* original=table[index].PAllocator;CHECK(original);table[index].PAllocator=&policy;
   bool result=ForceNativeBuffer(data.Data,0,force!=0);table[index].PAllocator=original;
   CHECK(result && policy.Calls==1 && !memcmp(snapshot,g_FableMemoryAllocator_013CAA90,sizeof(snapshot)));
  }
  CHECK(!data->Buffer && data->Data==reinterpret_cast<void*>(skip) && data->InternalAllocatedBufferSize==aligned && data->MemoryHandle==-1 && !data->Finished && !data->ThreadedFileUID);
  printf("HEAP_NULL_RESULT\t%lu\t%u\t%u\t%u\t%u\n",bank.Alignment,force,length,skip,aligned);++nullCases;
  if(aligned){CHECK(ForceNativeBuffer(data.Data,0,true) && data->Buffer && data->Data==(char*)data->Buffer+skip && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==1);++retries;}
  data.ReleaseData();CHECK(bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
 }
 CHECK(nullCases==8 && retries==6);printf("HEAP_NULL_PASS cases=%u retries=%u submitted=0\n",nullCases,retries);
 for(unsigned force=0;force<2;++force){
  CBankFileAsyncData* raw=static_cast<CBankFileAsyncData*>(::operator new(sizeof(CBankFileAsyncData)));CHECK(ConstructNativeAllocatedData(raw,0,0,0,0,bank)==raw);CCountedPointer<CBankFileAsyncData> data(raw);
  CHECK(ForceNativeBuffer(data.Data,0,force!=0) && data->Buffer && data->Data==data->Buffer && !data->InternalAllocatedBufferSize && !data->ThreadedFileUID);
  CHECK(g_FableMemoryAllocator_013CAA90->GetNoAllocations()==1);void* orphan=data->Buffer;data.ReleaseData();
  // Retail's zero size charge skips heap deallocation in the data destructor.
  // Record that ownership result, then explicitly clean up only in the fixture.
  CHECK(bank.AsyncDataInstances.empty() && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==1);
  printf("ZERO_HEAP_OWNER\t%lu\t%u\t1\n",bank.Alignment,force);CMemoryAllocator::Deallocate(orphan);CHECK(!g_FableMemoryAllocator_013CAA90->GetNoAllocations());
 }
 printf("ZERO_HEAP_OWNER_PASS cases=2 retained_after_owner=2 cleanup=2 submitted=0\n");
 {
  __declspec(align(16)) unsigned char storage[4096];CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);
  pool->InitialiseFromPreallocatedBuffer(storage,sizeof(storage),true);bank.SetLoadingMemoryPool(pool);
  ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead);PauseRead=true;
  {
   CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(1);
   CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
   CHECK(WaitForSingleObject(ReadIssued,5000)==0 && !data->Finished && pool->AllocatedSize>0);void* buffer=data->Buffer;
   bank.Close();bank.Close();CHECK(!bank.Size && !bank.BankFile.Data && bank.ThreadedFile.control_->RefCount==2);
   CHECK(!data->Finished && data->Buffer==buffer && bank.AsyncDataInstances.size()==1 && pool->AllocatedSize>0);
   CHECK(SetEvent(ReleaseRead) && WaitPublished(*data));CHECK(data->Buffer==buffer);
   printf("CLOSED_READ\t%lu\t",bank.Alignment);unsigned char* bytes=(unsigned char*)data->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");
  }
  CHECK(bank.AsyncDataInstances.empty() && !pool->AllocatedSize && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead));ReadIssued=ReleaseRead=0;bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());
 }
 CHECK(bank.OpenReadOnly(name,8|64|256));list.SetMaximumMemory(0x7fffffff);
 {CIVCountedPointer<const NGameText::CTextEntry> value=bank.GetPTextEntry(1,true);CHECK(value.data_);CheckText(*value.data_,0);}
 list.FrameEnd();list.SetMaximumMemory(0);CHECK(!list.ResourceCount && !list.AllocatedMemory && bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
 printf("PENDING_CLOSE_PASS completed=1 reopened=1 decoded=1\n");
 list.SetMaximumMemory(0x7fffffff);
 {
  CCriticalSectionLock pending(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bank.CriticalSection),false);BeginNativeText(bank.GetTextInfo(3),0,3,bank);
  NGameText::CTextEntry* text=bank.GetTextInfo(3)->PText.data_;CHECK(text && text->PFileData.Data && !text->PFileData->Finished);
  CCountedPointer<CBankFileAsyncData> data=text->PFileData;QueuedRead observer(bank,data);bank.AddToLoadingQueue(observer);
  unsigned resourceFrame=list.CurrentFrame,loadingFrame=bank.LoadingQueue.CurrentFrame;list.UnloadedThisFrame=7;bank.LoadingQueue.UnloadedThisFrame=9;
  bank.FrameEnd();CHECK(list.CurrentFrame==resourceFrame+1 && bank.LoadingQueue.CurrentFrame==loadingFrame+1 && !list.UnloadedThisFrame && !bank.LoadingQueue.UnloadedThisFrame);
  bank.ProcessLoadingQueue(false,0);CHECK(!observer.Notifications && list.ResourceCount==1 && bank.LoadingQueue.ResourceCount==1 && !data->Finished);
  pending.Release();CHECK(WaitPublished(*data));bank.ProcessLoadingQueue(false,0);
  CHECK(observer.Notifications==1 && observer.ResourceList==&list && list.ResourceCount==2 && !bank.LoadingQueue.ResourceCount);CheckList(list);CheckList(bank.LoadingQueue);
  {CIVCountedPointer<const NGameText::CTextEntry> value=bank.GetPTextEntry(3,true);CHECK(value.data_==text && value->PFileData.IsNull());CheckText(*value.data_,1);}
  CHECK(data.Info->RefCount==2 && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==1);
  list.UnloadedThisFrame=5;bank.LoadingQueue.UnloadedThisFrame=6;bank.FrameEnd();CHECK(list.CurrentFrame==resourceFrame+2 && bank.LoadingQueue.CurrentFrame==loadingFrame+2 && !list.UnloadedThisFrame && !bank.LoadingQueue.UnloadedThisFrame);
  list.SetMaximumMemory(0);CHECK(bank.GetTextInfo(3)->PText.IsNull() && list.ResourceCount==1 && list.AllocatedMemory==observer.ResourceSize && observer.ResourceList==&list && observer.Notifications==1);
  CHECK(bank.AsyncDataInstances.size()==1 && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==1);CheckList(list);
 }
 CHECK(!list.ResourceCount && !list.AllocatedMemory && !bank.LoadingQueue.ResourceCount && !bank.LoadingQueue.AllocatedMemory && bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
 printf("RESOURCE_BANK_FRAME_PASS pending=1 notifications=1 frames=2 retained_async=1\n");
 {
  __declspec(align(16)) unsigned char storage[4096];CCountedPointer<CMovableResourceMemoryPool> pool(new CMovableResourceMemoryPool);pool->InitialiseFromPreallocatedBuffer(storage,sizeof(storage),true);bank.SetLoadingMemoryPool(pool);
  ReadIssued=CreateEventA(0,0,0,0);ReleaseRead=CreateEventA(0,0,0,0);CHECK(ReadIssued && ReleaseRead);PauseRead=true;
  CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(1);CCountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
  QueuedAbortRead observer(data);data.ReleaseData();bank.AddToLoadingQueue(observer);CHECK(WaitForSingleObject(ReadIssued,5000)==0 && observer.Data.Info->RefCount==1 && !observer.Data->Finished);CBankFileAsyncData* held=observer.Data.Data;
  CResourceBank::SetResourceAbortingEnabled(true);bank.FrameEnd();bank.ProcessLoadingQueue(false,0);CHECK(!observer.Aborts && !observer.Loaded && bank.LoadingQueue.ResourceCount==1);
  bank.FrameEnd();CResourceBank::SetResourceAbortingEnabled(false);bank.ProcessLoadingQueue(false,0);CHECK(!observer.Aborts && bank.LoadingQueue.ResourceCount==1);
  CResourceBank::SetResourceAbortingEnabled(true);bank.ProcessLoadingQueue(false,0);CResourceBank::SetResourceAbortingEnabled(false);
  CHECK(observer.Aborts==1 && !observer.Loaded && observer.Data.IsNull() && !observer.ResourceList && !bank.LoadingQueue.ResourceCount && !bank.LoadingQueue.AllocatedMemory);
  CHECK(bank.DisposalList.size()==1 && bank.DisposalList.front().Data==held && bank.DisposalList.front().Info->RefCount==1 && !held->Finished && pool->AllocatedSize>0);
  bank.AsyncUpdatePending();CHECK(bank.DisposalList.size()==1 && !held->Finished);CHECK(SetEvent(ReleaseRead) && WaitPublished(*held));
  printf("QUEUE_ABORT_READ\t%lu\t",bank.Alignment);unsigned char* bytes=(unsigned char*)held->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");
  bank.AsyncUpdatePending();CHECK(bank.DisposalList.empty() && bank.AsyncDataInstances.empty() && !pool->AllocatedSize && pool->LockCount==1 && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
  CheckList(bank.LoadingQueue);CHECK(CloseHandle(ReadIssued) && CloseHandle(ReleaseRead));ReadIssued=ReleaseRead=0;bank.SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>());
 }
 printf("QUEUE_ABORT_PASS age_stops=1 global_stops=1 aborted=1 disposal=1\n");
 {
  CCountedPointer<CBankFileAsyncEntry> first=bank.GetAsyncEntry(1),second=bank.GetAsyncEntry(3),third=bank.GetAsyncEntry(5);
  CCountedPointer<CBankFileAsyncData> a=first->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314),b=second->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314),c=third->BeginReadDataAutoAlloc(0,128,g_NativeTextLoadingPriority_01396314);
  CHECK(WaitPublished(*a) && WaitPublished(*b) && WaitPublished(*c));double now=GFGetTime();__int64 savedStart=g_FableTimerStartTick_013CB2F0,shift=g_FableHighResolutionTimer_013CAA00?g_FableTimerFrequency_013CAA08*60:60000;CHECK(shift>0);
  BudgetRead one(bank,a,shift),two(bank,b,0),three(bank,c,0);bank.AddToLoadingQueue(one);bank.AddToLoadingQueue(two);bank.AddToLoadingQueue(three);
  // Advance only this process's clock input during the first callback, then
  // restore it before checking results. Actual timer APIs and GFGetTime run.
  bank.ProcessLoadingQueue(true,now+30.0);g_FableTimerStartTick_013CB2F0=savedStart;
  CHECK(one.Notifications==1 && !two.Notifications && !three.Notifications && bank.LoadingQueue.ResourceCount==2 && list.ResourceCount==1);
  CheckList(bank.LoadingQueue);CheckList(list);bank.ProcessLoadingQueue(true,now+30.0);
  CHECK(one.Notifications==1 && two.Notifications==1 && three.Notifications==1 && !bank.LoadingQueue.ResourceCount && !bank.LoadingQueue.AllocatedMemory && list.ResourceCount==3 && g_FableTimerStartTick_013CB2F0==savedStart);
  for(unsigned slot=0;slot<3;++slot){CBankFileAsyncData* data=slot==0?a.Data:slot==1?b.Data:c.Data;printf("DEADLINE_READ\t%lu\t%u\t",bank.Alignment,1+slot*2);unsigned char* bytes=(unsigned char*)data->GetData();for(unsigned k=0;k<128;++k)printf("%02x",bytes[k]);printf("\n");}
  CHECK(g_FableMemoryAllocator_013CAA90->GetNoAllocations()==3);CheckList(list);
 }
 CHECK(!list.ResourceCount && !list.AllocatedMemory && !bank.LoadingQueue.ResourceCount && bank.AsyncDataInstances.empty() && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
 printf("LOADING_DEADLINE_PASS first=1 remaining=2 drained=3 clock_restored=1\n");
 {
  typedef CCharString* (__fastcall* EnumFunction)(const NGameText::CDataBank*,void*,CCharString*,unsigned long);
  typedef CDateAndTime* (__fastcall* DateFunction)(NGameText::CDataBank*,void*,CDateAndTime*,const CCharString&);
  void** vtable=*reinterpret_cast<void***>(&bank);EnumFunction enumeration=reinterpret_cast<EnumFunction>(vtable[19]);DateFunction timestamp=reinterpret_cast<DateFunction>(vtable[18]);
  CHECK(reinterpret_cast<void*>(enumeration)==reinterpret_cast<void*>(&NativeBankEnumeration) && reinterpret_cast<void*>(timestamp)==reinterpret_cast<void*>(&NativeBankTimestamp));
  unsigned char bankSnapshot[sizeof(bank)];memcpy(bankSnapshot,&bank,sizeof(bank));unsigned long kinds[]={0,1,2,3,0x10000,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0;
  for(unsigned fill=0;fill<2;++fill)for(unsigned i=0;i<8;++i){unsigned char pattern=fill?0x95:0;__declspec(align(16)) unsigned char storage[64];memset(storage,pattern,sizeof(storage));CCharString* out=reinterpret_cast<CCharString*>(storage+16);long before=g_CCharStringInstanceCount_013BD800;
   const char* expected=kinds[i]==0?"EGameText":kinds[i]==1?"ETextGroup":"";
   CHECK(enumeration(&bank,0,out,kinds[i])==out && !strcmp((const char*)*out,expected) && out->GetLength()==strlen(expected) && g_CCharStringInstanceCount_013BD800==before+1);
   {CCharString copy(*out);out->~CCharString();CHECK(g_CCharStringInstanceCount_013BD800==before+1 && !strcmp((const char*)copy,expected));}
   CHECK(g_CCharStringInstanceCount_013BD800==before);for(unsigned n=0;n<sizeof(storage);++n)if(n<16 || n>=16+sizeof(CCharString))CHECK(storage[n]==pattern);
   memset(storage,pattern,sizeof(storage));CDateAndTime* date=reinterpret_cast<CDateAndTime*>(storage+16);
   {CCharString input(i%2?"unused entry":"");CHECK(timestamp(&bank,0,date,input)==date && date->Year==1 && date->Month==1 && date->Day==1 && date->Hour==1 && date->Minute==1 && !date->Second && !date->Millisecond);}
   for(unsigned n=0;n<sizeof(storage);++n)if(n<16 || n>=16+sizeof(CDateAndTime))CHECK(storage[n]==pattern);
   CHECK(!memcmp(bankSnapshot,&bank,sizeof(bank)));printf("BANK_METADATA_LEAF\t%lu\t%lu\t%s\n",bank.Alignment,kinds[i],expected);++cases;
  }
  CHECK(cases==16 && !g_FableMemoryAllocator_013CAA90->GetNoAllocations());
 }
 printf("BANK_METADATA_LEAVES_PASS enum=16 date=16 guarded=32 slots=18,19\n");
 {
  // Retail copies the complete local result, including three unspecified
  // padding bytes. Guard only the caller storage outside the 88-byte object.
  typedef CFileInformation* (__fastcall* InformationFunction)(CDiskFileWin32*,void*,CFileInformation*);
  CDiskFileWin32* disk=bank.BankFile.Data;CHECK(disk);InformationFunction information=reinterpret_cast<InformationFunction>((*reinterpret_cast<void***>(disk))[15]);CHECK(reinterpret_cast<void*>(information)==reinterpret_cast<void*>(&NativeDiskInformation));
  unsigned char diskSnapshot[sizeof(*disk)];memcpy(diskSnapshot,disk,sizeof(*disk));void* handle;memcpy(&handle,(char*)disk+0x10,4);unsigned long attributes=GetFileAttributesA(argv[1]);CHECK(attributes!=0xffffffff && !(attributes&1));
  for(unsigned mode=0;mode<2;++mode){
   CHECK(SetFileAttributesA(argv[1],mode?attributes|1:attributes));FixtureFileInfo before,after;bool beforeOk=GetFileInformationByHandle(handle,&before)!=0;
   __declspec(align(16)) unsigned char storage[128];memset(storage,0x95,sizeof(storage));CFileInformation* out=reinterpret_cast<CFileInformation*>(storage+16);CFileInformation* returned=information(disk,0,out);bool afterOk=GetFileInformationByHandle(handle,&after)!=0;bool restored=SetFileAttributesA(argv[1],attributes)!=0;
   CHECK(beforeOk && afterOk && restored && returned==out && out->ReadOnly==(mode!=0) && (before.Attributes&1)==mode && (after.Attributes&1)==mode);
   CHECK((MatchFileStamp(out->CreationTime,before.Creation)||MatchFileStamp(out->CreationTime,after.Creation)) && (MatchFileStamp(out->LastAccessTime,before.Access)||MatchFileStamp(out->LastAccessTime,after.Access)) && (MatchFileStamp(out->LastWriteTime,before.Write)||MatchFileStamp(out->LastWriteTime,after.Write)));
   for(unsigned n=0;n<sizeof(storage);++n)if(n<16 || n>=16+sizeof(CFileInformation))CHECK(storage[n]==0x95);
   CHECK(!memcmp(diskSnapshot,disk,sizeof(*disk)));printf("DISK_INFORMATION\t%lu\t%u\n",bank.Alignment,mode);
  }
  __declspec(align(16)) unsigned char diskStorage[sizeof(CDiskFileWin32)];memset(diskStorage,0,sizeof(diskStorage));CDiskFileWin32* closed=::new(diskStorage) CDiskFileWin32;
  __declspec(align(16)) unsigned char storage[128];memset(storage,0x95,sizeof(storage));CFileInformation* out=reinterpret_cast<CFileInformation*>(storage+16);CHECK(NativeDiskInformation(closed,0,out)==out && ZeroFileStamp(out->CreationTime) && ZeroFileStamp(out->LastAccessTime) && ZeroFileStamp(out->LastWriteTime) && !out->ReadOnly);
  for(unsigned n=0;n<sizeof(storage);++n)if(n<16 || n>=16+sizeof(CFileInformation))CHECK(storage[n]==0x95);closed->~CDiskFileWin32();
  CHECK(GetFileAttributesA(argv[1])==attributes);printf("DISK_INFORMATION\t%lu\t2\n",bank.Alignment);
 }
 printf("DISK_INFORMATION_PASS success=2 failure=1 guards=3 attributes_restored=1\n");
 {
  CDiskFileWin32* source=bank.BankFile.Data;CHECK(source && source->IsOpen());long savedPosition=source->GetPosition();unsigned references=bank.BankFile.Info->RefCount;
  CHECK((*reinterpret_cast<void***>(source))[14]==reinterpret_cast<void*>(&NativeDiskClone));source->SetPosition(37);CAFile* copy=source->Clone();
  CHECK(copy && copy!=source && copy->IsOpen() && copy->GetLength()==source->GetLength() && copy->GetPosition()==0 && source->GetPosition()==37 && bank.BankFile.Info->RefCount==references);
  {CWideString a=source->GetPathname(),b=copy->GetPathname();CHECK(!wcscmp((const wchar_t*)a,(const wchar_t*)b));}
  unsigned char bytes[32];memset(bytes,0x95,sizeof(bytes));copy->Read(bytes+8,13,0);CHECK(copy->GetPosition()==13 && source->GetPosition()==37);for(unsigned n=0;n<sizeof(bytes);++n)if(n<8 || n>=21)CHECK(bytes[n]==0x95);
  printf("CLONE_READ\t%lu\t0\t13\t",bank.Alignment);for(unsigned n=8;n<21;++n)printf("%02x",bytes[n]);printf("\n");
  source->Read(bytes+8,7,0);CHECK(source->GetPosition()==44 && copy->GetPosition()==13);printf("CLONE_READ\t%lu\t37\t7\t",bank.Alignment);for(unsigned n=8;n<15;++n)printf("%02x",bytes[n]);printf("\n");
  CFileInformation info=copy->GetFileInformation();CHECK(info.CreationTime.Year>=1601 && !info.ReadOnly);delete copy;CHECK(source->IsOpen() && source->GetPosition()==44 && bank.BankFile.Info->RefCount==references);source->SetPosition(savedPosition);CHECK(source->GetPosition()==savedPosition);
 }
 printf("DISK_CLONE_PASS independent=1 reads=2 source_alive=1\n");
 {
  CDiskFileWin32* source=bank.BankFile.Data;long savedPosition=source->GetPosition();CAFile* file=source->Clone();CHECK(file && file->IsOpen() && file->GetPosition()==0);unsigned char bytes[32];
  file->MovePosition(13);CHECK(file->GetPosition()==13);memset(bytes,0x95,sizeof(bytes));file->Read(bytes+8,17,0);CHECK(file->GetPosition()==30);CheckArchiveWindow(argv[1],13,bytes+8,17);for(unsigned n=0;n<sizeof(bytes);++n)if(n<8 || n>=25)CHECK(bytes[n]==0x95);printf("RELATIVE_READ\t%lu\t13\t17\t",bank.Alignment);for(unsigned n=8;n<25;++n)printf("%02x",bytes[n]);printf("\n");
  file->MovePosition(-7);CHECK(file->GetPosition()==23);file->Read(bytes+8,11,0);CHECK(file->GetPosition()==34);CheckArchiveWindow(argv[1],23,bytes+8,11);printf("RELATIVE_READ\t%lu\t23\t11\t",bank.Alignment);for(unsigned n=8;n<19;++n)printf("%02x",bytes[n]);printf("\n");
  file->MovePosition(0);CHECK(file->GetPosition()==34);file->MovePosition(-34);CHECK(file->GetPosition()==0);file->Read(bytes+8,4,0);CheckArchiveWindow(argv[1],0,bytes+8,4);CHECK(file->GetPosition()==4);printf("RELATIVE_READ\t%lu\t0\t4\t",bank.Alignment);for(unsigned n=8;n<12;++n)printf("%02x",bytes[n]);printf("\n");
  file->MovePosition(file->GetLength()-4);CHECK(file->GetPosition()==file->GetLength());file->MovePosition(0);CHECK(file->GetPosition()==file->GetLength());file->MovePosition(-file->GetLength());CHECK(file->GetPosition()==0 && source->GetPosition()==savedPosition);delete file;CHECK(source->IsOpen() && source->GetPosition()==savedPosition);
 }
 printf("RELATIVE_SEEK_PASS moves=7 reads=3 bounds=2 source_unchanged=1\n");
 ProbePriority(bank,argv[1],0);ProbePriority(bank,argv[1],1);ProbePriority(bank,argv[1],2);ProbePriority(bank,argv[1],3);ProbeFileKeys(path,argv[1]);printf("NATIVE_FILE_KEYS_PASS handles=3 reads=4 guards=4 callbacks=4 cleanup=1\n");printf("NATIVE_PRIORITY_PASS queued=12 reads=16 promoted=1 equal_priority=1 offset_tie=1 active_unchanged=1 cleanup=1 wait_promotion=0\n");bank.Close();bank.Close();
 }manager.Close();manager.Close();}
 g_CAFileCurrentPath_013BCA10=L"";CHECK(g_CCharStringInstanceCount_013BD800==0 && g_CWideStringInstanceCount_013BCA20==2);CHECK(g_FableMemoryAllocator_013CAA90->GetNoAllocations()==0);FableMemoryAllocatorShutdown_009f2d20();CHECK(!g_FableMemoryAllocator_013CAA90);CHECK(passes==14);printf("NATIVE_TEXT_EVICTION_PASS loads=%u shrink_passes=%u retained=2\n",loads,passes);return 0;
}
bool CResourceBank::ResourceAbortingEnabled=false;
#include "NativeFileInputStreamColdAliases.h"
#include "NativeBankLifetimeColdAliases.h"
#include "NativeTextMetadataColdAliases.h"
#pragma comment(linker,"/alternatename:_UnusedBankBoundary=_ColdBoundary")
#pragma comment(linker,"/alternatename:?UnloadNextResource@CResourceList@@QAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?HandleFailedAllocation@CResourceList@@UAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z=_ColdBoundary")


// Random relocation stress is outside this bounded pool composition.
#pragma comment(linker,"/alternatename:?GFROR13@@YIKK@Z=_ColdBoundary")
