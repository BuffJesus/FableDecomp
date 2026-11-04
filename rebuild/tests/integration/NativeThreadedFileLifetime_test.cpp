// Real owned handles and allocation; manager and string/base children controlled.
#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"
#include "NativeThreadedReadAPC_win32.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
__declspec(dllimport) BOOL __stdcall GetHandleInformation(HANDLE,DWORD*);
unsigned char LifetimeManagerStorage[0x178];
extern DWORD LifetimeFileVtable[];
void* __fastcall LifetimeDeleting(CThreadedFile*,void*,unsigned);
}
#pragma comment(linker,"/alternatename:?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A=_LifetimeManagerStorage")
#pragma comment(linker,"/alternatename:_LifetimeFileVtable=??_7CThreadedFile@@6B@")
#pragma comment(linker,"/alternatename:@LifetimeDeleting@12=??_GCThreadedFile@@UAEPAXI@Z")
static void FailLife(const char* expr,int line) {
 fprintf(stderr,"NATIVE_FILE_LIFETIME_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK_LIFE(x) do {if(!(x)) FailLife(#x,__LINE__);}while(0)
static unsigned g_trace[16],g_count,g_freed;
static unsigned char* g_allocation;
static unsigned char g_snapshot[96];
static CThreadedFile* g_object;
static HANDLE g_replacement;
static unsigned g_handleMode;
static void Record(unsigned n) {CHECK_LIFE(g_count<16);g_trace[g_count++]=n;}
static DWORD WordLife(const void* p) {DWORD x;memcpy(&x,p,4);return x;}
static void PutLife(void* p,DWORD x) {memcpy(p,&x,4);}
CBaseClass::CBaseClass() {Record(1);}
CBaseClass::~CBaseClass() {Record(7);PutLife(this,0x43211234);}
CBaseClassNonCopyable::CBaseClassNonCopyable() {Record(2);}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {CHECK_LIFE(this==g_object);Record(6);}
CWideString::CWideString() {Record(3);storage_=0;}
CWideString::~CWideString() {CHECK_LIFE(this==&g_object->filenameStorage_);Record(5);storage_=0;}
void CThreadedFileManager::CloseAllStreamsForFile(CThreadedFile* file) {
 CHECK_LIFE(reinterpret_cast<void*>(this)==LifetimeManagerStorage && file==g_object);Record(4);
 CHECK_LIFE(WordLife(file)==reinterpret_cast<DWORD>(LifetimeFileVtable));
 if(g_handleMode==3)file->fileHandle_=g_replacement;
}
void __cdecl operator delete(void* p) {
 CHECK_LIFE(p==g_object && !g_freed && g_trace[g_count-1]==7);Record(8);g_freed=1;
 memcpy(g_snapshot,g_allocation,96);free(g_allocation);g_allocation=0;
}
static void __fastcall BaseConstructLife(CBaseClassNonCopyable* p) {new(p) CBaseClassNonCopyable;}
static void __fastcall WideConstructLife(CWideString* p) {new(p) CWideString;}
static void __fastcall WideDestroyLife(CWideString* p) {p->~CWideString();}
static void __fastcall BaseDestroyLife(CBaseClassNonCopyable* p) {p->CBaseClassNonCopyable::~CBaseClassNonCopyable();}
static void* __fastcall RebuiltConstructLife(void* p,void*) {return new(p) CThreadedFile;}
static DWORD ManagerCloseAddressLife() {
 void(CThreadedFileManager::*m)(CThreadedFile*)=&CThreadedFileManager::CloseAllStreamsForFile;DWORD p;CHECK_LIFE(sizeof(m)==4);memcpy(&p,&m,4);return p;
}
static BOOL (__stdcall* pCloseLife)(HANDLE)=CloseHandle;
static void (__cdecl* pDeleteLife)(void*)=::operator delete;
typedef void* (__fastcall* ConstructLife)(void*,void*);
typedef void* (__fastcall* DeleteLife)(CThreadedFile*,void*,unsigned);
static void* LoadLife(const char* path,unsigned size,DWORD entry,bool constructor) {
 unsigned char raw[88];FILE* f=fopen(path,"rb");CHECK_LIFE(f);CHECK_LIFE(fread(raw,1,size,f)==size && fgetc(f)==EOF);CHECK_LIFE(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK_LIFE(code);memcpy(code,raw,size);
 const unsigned ctorSites[3]={4,13,18},deleteSites[7]={9,17,22,32,55,62,75};
 const DWORD ctorRetail[3]={0x99a2f0,0x129a158,0x99aed0},deleteRetail[7]={0x129a158,0x13bc870,0x98e7d0,0x143fe30,0x99b510,0x99a300,0xbfe9bc};
 const DWORD ctorTargets[3]={reinterpret_cast<DWORD>(BaseConstructLife),reinterpret_cast<DWORD>(LifetimeFileVtable),reinterpret_cast<DWORD>(WideConstructLife)};
 const DWORD deleteTargets[7]={reinterpret_cast<DWORD>(LifetimeFileVtable),reinterpret_cast<DWORD>(LifetimeManagerStorage),ManagerCloseAddressLife(),reinterpret_cast<DWORD>(&pCloseLife),reinterpret_cast<DWORD>(WideDestroyLife),reinterpret_cast<DWORD>(BaseDestroyLife),reinterpret_cast<DWORD>(pDeleteLife)};
 for(unsigned i=0;i<(constructor?3:7);++i) {
  unsigned site=constructor?ctorSites[i]:deleteSites[i];DWORD value=WordLife(raw+site);bool relative=constructor?i!=1:i==2 || i>=4;
  CHECK_LIFE((relative?DWORD(entry+site+4+LONG(value)):value)==(constructor?ctorRetail[i]:deleteRetail[i]));
  DWORD target=constructor?ctorTargets[i]:deleteTargets[i];PutLife(code+site,relative?target-(reinterpret_cast<DWORD>(code)+site+4):target);
 }
 DWORD old;CHECK_LIFE(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK_LIFE(FlushInstructionCache(GetCurrentProcess(),code,size));return code;
}
static bool ValidLife(HANDLE h) {DWORD flags;return GetHandleInformation(h,&flags)!=0;}
static void RunLife(ConstructLife construct,DeleteLife destroy,const char* path,unsigned handleMode,unsigned flags,bool virtualDelete) {
 g_count=g_freed=0;g_handleMode=handleMode;g_allocation=static_cast<unsigned char*>(malloc(96));CHECK_LIFE(g_allocation);memset(g_allocation,0xa7,96);g_object=reinterpret_cast<CThreadedFile*>(g_allocation+32);
 unsigned char expected[96];memcpy(expected,g_allocation,96);
 CHECK_LIFE(construct(g_object,0)==g_object && g_count==3 && g_trace[0]==1 && g_trace[1]==2 && g_trace[2]==3);
 PutLife(expected+32,reinterpret_cast<DWORD>(LifetimeFileVtable));PutLife(expected+32+8,0xffffffff);PutLife(expected+32+0xc,0);expected[32+0x18]=expected[32+0x19]=0;
 CHECK_LIFE(memcmp(g_allocation,expected,96)==0 && LifetimeFileVtable[0]==reinterpret_cast<DWORD>(LifetimeDeleting));
 HANDLE first=INVALID_HANDLE_VALUE,second=INVALID_HANDLE_VALUE;
 if(handleMode>=2) {first=CreateFileA(path,GENERIC_READ,3,0,OPEN_EXISTING,0,0);CHECK_LIFE(first!=INVALID_HANDLE_VALUE);}
 if(handleMode==3) {second=CreateFileA(path,GENERIC_READ,3,0,OPEN_EXISTING,0,0);CHECK_LIFE(second!=INVALID_HANDLE_VALUE);}
 g_replacement=second;g_object->fileHandle_=handleMode==1?0:first;
 g_object->openedForWrite_=g_object->openFlag_=true;PutLife(&g_object->filenameStorage_,0x13579bdf);
 PutLife(expected+32,0x43211234);PutLife(expected+32+8,0xffffffff);PutLife(expected+32+0xc,0);expected[32+0x18]=expected[32+0x19]=handleMode==0?1:0;
 if(virtualDelete)delete g_object;else CHECK_LIFE(destroy(g_object,0,flags)==g_object);
 const unsigned* trace=g_trace+3;unsigned count=0;
 if(handleMode)CHECK_LIFE(trace[count++]==4);
 CHECK_LIFE(trace[count++]==5 && trace[count++]==6 && trace[count++]==7);
 bool freed=virtualDelete || (flags&1)!=0;
 if(freed)CHECK_LIFE(trace[count++]==8);
 CHECK_LIFE(g_count==count+3 && g_freed==unsigned(freed));
 CHECK_LIFE(memcmp(freed?g_snapshot:g_allocation,expected,96)==0);
 if(handleMode>=2) {
  HANDLE closed=handleMode==3?second:first;CHECK_LIFE(!ValidLife(closed) && GetLastError()==6);
  if(handleMode==3)CHECK_LIFE(ValidLife(first) && CloseHandle(first));
 }
 if(!freed) {free(g_allocation);g_allocation=0;}g_object=0;
}
int main(int argc,char** argv) {
 CHECK_LIFE(argc==4);ConstructLife originalCtor=reinterpret_cast<ConstructLife>(LoadLife(argv[1],41,0x98dfd0,true));DeleteLife originalDelete=reinterpret_cast<DeleteLife>(LoadLife(argv[2],88,0x991450,false));
 const unsigned flags[6]={0,1,2,3,0x100,0xffffffff};unsigned cases=0;
 unsigned char managerExpected[0x178];memset(LifetimeManagerStorage,0x6d,sizeof(LifetimeManagerStorage));memcpy(managerExpected,LifetimeManagerStorage,sizeof(managerExpected));
 for(unsigned mode=0;mode<2;++mode)for(unsigned handle=0;handle<4;++handle)for(unsigned f=0;f<6;++f) {RunLife(mode?RebuiltConstructLife:originalCtor,mode?LifetimeDeleting:originalDelete,argv[3],handle,flags[f],false);++cases;}
 for(unsigned handle=0;handle<4;++handle) {RunLife(RebuiltConstructLife,LifetimeDeleting,argv[3],handle,1,true);++cases;}
 CHECK_LIFE(memcmp(managerExpected,LifetimeManagerStorage,sizeof(managerExpected))==0);
 CHECK_LIFE(VirtualFree(reinterpret_cast<void*>(originalCtor),0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalDelete),0,MEM_RELEASE));
 printf("NATIVE_FILE_LIFETIME_OS_PASS cases=%u direct=48 virtual_deletes=4\n",cases);return 0;
}
