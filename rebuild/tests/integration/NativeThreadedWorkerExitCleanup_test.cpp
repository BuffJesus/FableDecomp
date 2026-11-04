// Owned Windows worker submission probe; no installed-game process is started.
#include "fable_threaded_file_object.h"
#include "fable_file_installer.h"
#include "NativeThreadedReadAPC_win32.h"
#include <process.h>
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
static void Fail(const char* expr,int line) {
 fprintf(stderr,"NATIVE_WORKER_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK(x) do {if(!(x)) Fail(#x,__LINE__);}while(0)
CCountedFileInstaller g_CFileInstallerSingleton_013CA818={0,0};
CFileInstaller& __fastcall CFileInstallerSingleton::Get() {Fail("installer absent",__LINE__);return *static_cast<CFileInstaller*>(0);}
void CFileInstaller::Pause() {Fail("installer Pause boundary",__LINE__);}
void CFileInstaller::Unpause() {Fail("installer Unpause boundary",__LINE__);}
double __fastcall GFGetTime() {return 10.0;}
static unsigned g_removed,g_finished,g_begun;
static HANDLE g_finishEvent;
static DWORD g_mainThread,g_workerThread;
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
 h[1]=0;h[2]=h[3]=reinterpret_cast<DWORD>(h);g_manager[q+1]=0;g_manager[q+3]=reinterpret_cast<DWORD>(h);
 ++g_removed;
}
typedef _STL::map<unsigned long,CThreadedFileManager::CFileObject*>::_Rep_type FixtureUidTree;
template<> FixtureUidTree::size_type FixtureUidTree::erase(const unsigned long&) {Fail("unexpected retained UID erase",__LINE__);return 0;}
static unsigned g_completionCalls;
static void __fastcall Complete(void* self,void*,bool success) {
 CHECK(self && success && GetCurrentThreadId()==g_workerThread);
 CHECK(g_request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_READ_COMPLETED);
 CHECK(g_manager[(0x14c+8)/4]==0);++g_completionCalls;
}
static bool __fastcall Begin(void* self,void*,void** dst,EThreadedFilePriority priority) {
 CHECK(self && dst==&g_request->Desc.PDst && priority==g_request->Desc.Priority);
 CHECK(g_request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED);
 CHECK(g_manager[(0x14c+8)/4]==0);
 ++g_begun;return g_allow;
}
static unsigned __stdcall Launch(void* context) {FixtureBeginThread(context);return 0;}
static unsigned char Payload(unsigned i) {return static_cast<unsigned char>(i*37+23);}
static DWORD Run(HANDLE file,unsigned priority,unsigned device,unsigned offset,unsigned length,bool callback,bool write) {
 __declspec(align(8)) unsigned long manager[0x178/4];memset(manager,0,sizeof(manager));
 unsigned long headers[14][8];memset(headers,0,sizeof(headers));
 unsigned long node[6];memset(node,0,sizeof(node));
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
 for(unsigned d=0;d<2;++d)for(unsigned p=0;p<7;++p) {
  unsigned q=1+(d*7+p)*4;unsigned long* h=headers[d*7+p];h[2]=h[3]=reinterpret_cast<DWORD>(h);
  manager[q]=reinterpret_cast<DWORD>(h);manager[q+3]=reinterpret_cast<DWORD>(h);
  if(d==device && p==priority) {h[1]=h[2]=h[3]=reinterpret_cast<DWORD>(node);manager[q+1]=1;node[0]=1;node[1]=reinterpret_cast<DWORD>(h);node[4]=node[5]=reinterpret_cast<DWORD>(&request);}
 }
 for(unsigned p=0;p<7;++p)manager[0xe4/4+p]=1-device;
 reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(manager)+0x100)[priority]=9.0;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[0]=0.25f;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[1]=0.5f;
 unsigned long uidHeader[6];memset(uidHeader,0,sizeof(uidHeader));unsigned long uidNode[6];memset(uidNode,0,sizeof(uidNode));uidNode[0]=1;uidNode[1]=reinterpret_cast<DWORD>(uidHeader);uidNode[4]=1234;uidNode[5]=reinterpret_cast<DWORD>(&request);
 uidHeader[1]=uidHeader[2]=uidHeader[3]=reinterpret_cast<DWORD>(uidNode);manager[0x13c/4]=reinterpret_cast<DWORD>(uidHeader);manager[0x140/4]=1;request.UID=1234;
 reinterpret_cast<unsigned char*>(manager)[0x148]=1;
 HANDLE event=CreateEventA(0,0,0,0);CHECK(event);manager[0x164/4]=reinterpret_cast<DWORD>(event);
 InitializeCriticalSection(manager+0x14c/4);
 g_finishEvent=CreateEventA(0,1,0,0);CHECK(g_finishEvent);g_mainThread=GetCurrentThreadId();g_workerThread=0;
 unsigned threadID=0;HANDLE thread=reinterpret_cast<HANDLE>(_beginthreadex(0,0,Launch,manager,0,&threadID));CHECK(thread);
 DWORD started=GetTickCount();
 for(;;) {
  EnterCriticalSection(manager+0x14c/4);
  bool finished=request.LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_FINISHED;
  LeaveCriticalSection(manager+0x14c/4);
  if(finished)break;
  CHECK(GetTickCount()-started<5000);Sleep(1);
 }
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
 CHECK(WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);
 CHECK(g_workerThread==threadID && g_workerThread!=g_mainThread);
 CHECK(g_finished==1 && g_begun==unsigned(callback && !write));
 CHECK(g_completionCalls==unsigned(callback));
 CHECK(!reinterpret_cast<unsigned char*>(manager)[0x148] && reinterpret_cast<unsigned char*>(manager)[0x149] && reinterpret_cast<unsigned char*>(manager)[0x14a]);
 CHECK(manager[0x164/4]==0);
 CHECK(manager[0x140/4]==1 && uidHeader[1]==reinterpret_cast<DWORD>(uidNode));
 CHECK(CloseHandle(thread) && CloseHandle(g_finishEvent));g_finishEvent=0;g_request=0;g_manager=0;return hash;
}
int main(int argc,char** argv) {
 CHECK(argc==3 && !FixtureHardError);
 HANDLE input=CreateFileA(argv[1],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
 HANDLE output=CreateFileA(argv[2],GENERIC_READ|0x40000000UL,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
 CHECK(input!=INVALID_HANDLE_VALUE && output!=INVALID_HANDLE_VALUE);
 unsigned count=0;
 for(unsigned w=0;w<2;++w)for(unsigned p=0;p<7;++p)for(unsigned d=0;d<2;++d)for(unsigned cb=0;cb<2;++cb) {
  Run(w ? output:input,p,d,p*17,17+p,cb!=0,w!=0);++count;
 }
 CHECK(CloseHandle(input) && CloseHandle(output));
 printf("NATIVE_WORKER_EXIT_CLEANUP_PASS cases=%u reads=28 writes=28 joins=56 shutdowns=56 callbacks=28 exit_objects=56\n",count);return 0;
}
