// Owned Windows worker submission probe; no installed-game process is started.
#include "fable_threaded_file_object.h"
#include "fable_file_installer.h"
#include "NativeThreadedReadAPC_win32.h"
#include <stdio.h>
#include <string.h>
extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
__declspec(dllimport) DWORD __stdcall WaitForSingleObjectEx(HANDLE,DWORD,BOOL);
__declspec(dllimport) BOOL __stdcall WriteFileEx(HANDLE,const void*,DWORD,OVERLAPPED*,IOCompletion);
void __fastcall FixtureProcess(CThreadedFileManager*,void*);
void __stdcall FixtureReadCompletion(DWORD,DWORD,OVERLAPPED*);
typedef void (__fastcall* HardError)(const char*);
extern HardError FixtureHardError;
}
#pragma comment(linker,"/alternatename:@FixtureProcess@8=?ProcessObjects@CThreadedFileManager@@AAEXXZ")
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
static bool g_allow;
static CThreadedFileManager::CFileObject* g_request;
static unsigned long* g_manager;
void CThreadedFileManager::RemoveObjectFromPendingMap(CFileObject* object) {
 CHECK(object==g_request && reinterpret_cast<unsigned long*>(this)==g_manager);
 CHECK(object->LoadingStatus==(g_begun ? FILE_LOADING_STATUS_PENDING_LOCKED : FILE_LOADING_STATUS_PENDING));
 CHECK(g_manager[(0x14c+8)/4]==1);
 ++g_removed;
}
void CThreadedFileManager::OnFinishObject(CFileObject*) {++g_finished;Fail("finish boundary",__LINE__);}
static bool __fastcall Begin(void* self,void*,void** dst,EThreadedFilePriority priority) {
 CHECK(self && dst==&g_request->Desc.PDst && priority==g_request->Desc.Priority);
 CHECK(g_request->LoadingStatus==CThreadedFileManager::FILE_LOADING_STATUS_PENDING_LOCKED);
 CHECK(g_manager[(0x14c+8)/4]==0);
 ++g_begun;return g_allow;
}
static void Put(unsigned char* p,unsigned off,DWORD x) {memcpy(p+off,&x,4);}
static DWORD Word(const unsigned char* p,unsigned off) {DWORD x;memcpy(&x,p+off,4);return x;}
static void (__stdcall* pEnter)(void*)=EnterCriticalSection;
static void (__stdcall* pLeave)(void*)=LeaveCriticalSection;
static DWORD (__stdcall* pWait)(HANDLE,DWORD,BOOL)=WaitForSingleObjectEx;
static DWORD (__stdcall* pSleep)(DWORD,BOOL)=SleepEx;
static BOOL (__stdcall* pEvent)(HANDLE)=SetEvent;
static BOOL (__stdcall* pRead)(HANDLE,void*,DWORD,OVERLAPPED*,IOCompletion)=ReadFileEx;
static BOOL (__stdcall* pWrite)(HANDLE,const void*,DWORD,OVERLAPPED*,IOCompletion)=WriteFileEx;
static const char empty[]="";
struct Fixup {unsigned site,kind,target;};
#include "original_fixups.h"
// Private member address access is supplied through public linker aliases.
extern "C" void __fastcall FixtureRemove(CThreadedFileManager*,void*,CThreadedFileManager::CFileObject*);
extern "C" void __fastcall FixtureFinish(CThreadedFileManager*,void*,CThreadedFileManager::CFileObject*);
#pragma comment(linker,"/alternatename:@FixtureRemove@12=?RemoveObjectFromPendingMap@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z")
#pragma comment(linker,"/alternatename:@FixtureFinish@12=?OnFinishObject@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z")
static DWORD Target(unsigned t) {
 switch(t) {
 case 0x143fe0c:return reinterpret_cast<DWORD>(&pEnter);
 case 0x143fe10:return reinterpret_cast<DWORD>(&pLeave);
 case 0x143fe08:return reinterpret_cast<DWORD>(&pWait);
 case 0x143fddc:return reinterpret_cast<DWORD>(&pSleep);
 case 0x143fde0:return reinterpret_cast<DWORD>(&pEvent);
 case 0x143fdf4:return reinterpret_cast<DWORD>(&pRead);
 case 0x143fdd8:return reinterpret_cast<DWORD>(&pWrite);
 case 0x98ebc0:return reinterpret_cast<DWORD>(FixtureFinish);
 case 0x98e770:return reinterpret_cast<DWORD>(FixtureRemove);
 case 0x9e1bc0:return reinterpret_cast<DWORD>(GFGetTime);
 case 0x98e320:return reinterpret_cast<DWORD>(FixtureReadCompletion);
 case 0x13bc864:return reinterpret_cast<DWORD>(&FixtureHardError);
 case 0x13ca818:return reinterpret_cast<DWORD>(&g_CFileInstallerSingleton_013CA818);
 case 0x122d70e:return reinterpret_cast<DWORD>(empty);
 case 0x404440:return reinterpret_cast<DWORD>(&CFileInstallerSingleton::Get);
 case 0x9c31e0:{void (CFileInstaller::*m)()=&CFileInstaller::Pause;DWORD x;CHECK(sizeof(m)==4);memcpy(&x,&m,4);return x;}
 case 0x9c3250:{void (CFileInstaller::*m)()=&CFileInstaller::Unpause;DWORD x;CHECK(sizeof(m)==4);memcpy(&x,&m,4);return x;}
 }Fail("unknown fixup",__LINE__);return 0;
}
typedef void (__fastcall* Worker)(CThreadedFileManager*,void*);
static Worker LoadOriginal(const char* path) {
 unsigned char original[786];FILE* f=fopen(path,"rb");CHECK(f);CHECK(fread(original,1,sizeof(original),f)==sizeof(original));CHECK(fgetc(f)==EOF);CHECK(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK(code);memcpy(code,original,sizeof(original));
 for(unsigned i=0;i<sizeof(fixups)/sizeof(fixups[0]);++i) {
  const Fixup& x=fixups[i];DWORD old=Word(original,x.site);
  CHECK((x.kind==20 ? DWORD(0x98ecd0+x.site+4+LONG(old)):old)==x.target);
  DWORD target=Target(x.target);Put(code,x.site,x.kind==20 ? target-(reinterpret_cast<DWORD>(code)+x.site+4):target);
 }
 DWORD old;CHECK(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK(FlushInstructionCache(GetCurrentProcess(),code,sizeof(original)));return reinterpret_cast<Worker>(code);
}
static unsigned char Payload(unsigned i) {return static_cast<unsigned char>(i*37+23);}
static DWORD Run(Worker worker,HANDLE file,unsigned priority,unsigned device,unsigned offset,unsigned length,bool callback,bool allow,bool write) {
 __declspec(align(8)) unsigned long manager[0x178/4];memset(manager,0,sizeof(manager));
 unsigned long headers[14][8];memset(headers,0,sizeof(headers));
 unsigned long node[6];memset(node,0,sizeof(node));
 unsigned long fileStorage[0x1c/4];memset(fileStorage,0,sizeof(fileStorage));fileStorage[1]=device;fileStorage[2]=reinterpret_cast<DWORD>(file);
 OVERLAPPED ov;memset(&ov,0xab,sizeof(ov));unsigned char buffer[8192+32];memset(buffer,0xcd,sizeof(buffer));
 reinterpret_cast<unsigned char*>(fileStorage)[0x18]=write;
 if(write)for(unsigned i=0;i<length;++i)buffer[16+i]=Payload(offset+i)^0x5a;
 CThreadedFileManager::CFileObject request;memset(&request,0,sizeof(request));
 request.Desc.PDst=buffer+16;request.Desc.FilePos=offset;request.Desc.Size=length;request.Desc.Priority=static_cast<EThreadedFilePriority>(priority);
 request.PFile=reinterpret_cast<CThreadedFile*>(fileStorage);request.PManager=reinterpret_cast<CThreadedFileManager*>(manager);request.Overlapped=&ov;
 void* vtable[2]={0,reinterpret_cast<void*>(Begin)};void* cb=vtable;request.Desc.PCallback=callback ? reinterpret_cast<CThreadedFileCallback*>(&cb):0;
 g_request=&request;g_manager=manager;g_allow=allow;g_begun=g_removed=g_finished=0;
 for(unsigned d=0;d<2;++d)for(unsigned p=0;p<7;++p) {
  unsigned q=1+(d*7+p)*4;unsigned long* h=headers[d*7+p];h[2]=h[3]=reinterpret_cast<DWORD>(h);
  manager[q]=reinterpret_cast<DWORD>(h);manager[q+3]=reinterpret_cast<DWORD>(h);
  if(d==device && p==priority) {h[1]=h[2]=h[3]=reinterpret_cast<DWORD>(node);manager[q+1]=1;node[0]=1;node[1]=reinterpret_cast<DWORD>(h);node[4]=node[5]=reinterpret_cast<DWORD>(&request);}
 }
 for(unsigned p=0;p<7;++p)manager[0xe4/4+p]=1-device;
 reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(manager)+0x100)[priority]=9.0;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[0]=0.25f;
 reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(manager)+0x170)[1]=0.5f;
 HANDLE event=CreateEventA(0,1,0,0);CHECK(event);manager[0x164/4]=reinterpret_cast<DWORD>(event);
 InitializeCriticalSection(manager+0x14c/4);
 DWORD started=GetTickCount();worker(reinterpret_cast<CThreadedFileManager*>(manager),0);CHECK(GetTickCount()-started<5000);
 CHECK(g_finished==0 && g_begun==unsigned(callback && !write));
 CHECK(manager[(0x14c+8)/4]==0);
 DWORD hash=2166136261UL;
 if(callback && !allow && !write) {
  CHECK(g_removed==0 && request.LoadingStatus==0 && !manager[0x138/4]);
  for(unsigned i=0;i<sizeof(buffer);++i)CHECK(buffer[i]==0xcd);
  const unsigned char* b=reinterpret_cast<const unsigned char*>(&ov);for(unsigned i=0;i<20;++i)CHECK(b[i]==0xab);
 } else {
  CHECK(g_removed==1 && request.LoadingStatus==3 && manager[0x138/4]==reinterpret_cast<DWORD>(&request));
  CHECK(ov.Offset==offset && ov.OffsetHigh==0 && ov.hEvent==&request);
  CHECK(WaitForSingleObject(event,0)==WAIT_OBJECT_0);
  unsigned transferred=offset>=0x9000 ? 0 : ((0x9000-offset)<length ? (0x9000-offset):length);
  for(unsigned i=0;i<transferred;++i) {CHECK(buffer[16+i]==(write ? (Payload(offset+i)^0x5a):Payload(offset+i)));hash=(hash^buffer[16+i])*16777619UL;}
  for(unsigned i=0;i<16;++i)CHECK(buffer[i]==0xcd);
  for(unsigned i=16+transferred;i<sizeof(buffer);++i)CHECK(buffer[i]==0xcd);
 }
 DeleteCriticalSection(manager+0x14c/4);CHECK(CloseHandle(event));g_request=0;g_manager=0;request.Overlapped=0; // Borrowed stack OVERLAPPED.
 return hash;
}
int main(int argc,char** argv) {
 CHECK(argc==4);CHECK(!FixtureHardError);Worker original=LoadOriginal(argv[1]);
 HANDLE file=CreateFileA(argv[2],GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);CHECK(file!=INVALID_HANDLE_VALUE);
 HANDLE writable=CreateFileA(argv[3],GENERIC_READ|0x40000000UL,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);CHECK(writable!=INVALID_HANDLE_VALUE);
 unsigned count=0;
 for(unsigned w=0;w<2;++w)for(unsigned p=0;p<7;++p)for(unsigned d=0;d<2;++d)for(unsigned c=0;c<3;++c) {
  HANDLE handle=w ? writable:file;
  DWORD a=Run(original,handle,p,d,p*17,17+p,c!=0,c!=2,w!=0);
  DWORD b=Run(FixtureProcess,handle,p,d,p*17,17+p,c!=0,c!=2,w!=0);CHECK(a==b);++count;
 }
 CHECK(CloseHandle(file) && CloseHandle(writable));CHECK(VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE));
 printf("NATIVE_WORKER_IO_PASS cases=%u original_rebuilt=PASS\n",count);return 0;
}
