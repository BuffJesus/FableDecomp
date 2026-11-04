// Actual rooted path/string ownership composed with native file construction/deletion.
#include "fable_filesystem.h"
#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"
#include "NativeThreadedReadAPC_win32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <new>
#include <direct.h>
#include <ctype.h>
__declspec(noreturn) static void FailWrite(const char* expr,int line) {
 fprintf(stderr,"NATIVE_FILE_READ_PATH_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK_WRITE(x) do {if(!(x)) FailWrite(#x,__LINE__);}while(0)
static wchar_t g_realPath[260];
static const CWideString* g_expectedName;
static unsigned g_copies,g_allocated,g_released;
static CWideStringData* g_blocks[16];
static CCharStringData* g_charBlocks[16];
static unsigned g_charAllocated,g_charReleased;
static void* g_charArrays[32];
static unsigned g_arrayAllocated,g_arrayReleased;
static unsigned char* g_fileAllocation;
static CThreadedFile* g_fileObject;
static unsigned char g_fileSnapshot[96];
static unsigned g_fileFreed,g_managerCloses;
static bool g_finishReplace;
static unsigned g_finishCalls,g_finishCallbacks,g_finishMutations;
static unsigned char* g_controlAllocation;
static CCPPointerInfo* g_controlBlock;
static unsigned g_controlAllocated,g_controlReleased;
extern "C" {
unsigned char LifetimeManagerStorage[0x178];
extern DWORD LifetimeFileVtable[];
extern DWORD LifetimeBaseVtable[];
void* __fastcall LifetimeDeleting(CThreadedFile*,void*,unsigned);
__declspec(dllimport) BOOL __stdcall GetHandleInformation(HANDLE,DWORD*);
}
#pragma comment(linker,"/alternatename:?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A=_LifetimeManagerStorage")
#pragma comment(linker,"/alternatename:_LifetimeFileVtable=??_7CThreadedFile@@6B@")
#pragma comment(linker,"/alternatename:_LifetimeBaseVtable=??_7CBaseClass@@6B@")
#pragma comment(linker,"/alternatename:@LifetimeDeleting@12=??_GCThreadedFile@@UAEPAXI@Z")
void CThreadedFileManager::CloseAllStreamsForFile(CThreadedFile* file) {
 CHECK_WRITE(reinterpret_cast<void*>(this)==LifetimeManagerStorage && file==g_fileObject);
 DWORD flags;CHECK_WRITE(GetHandleInformation(file->fileHandle_,&flags));++g_managerCloses;
 if(g_finishReplace)file->fileHandle_=INVALID_HANDLE_VALUE;
}
static CWideStringData* DataOf(const CWideString& value) {
 CWideStringData* data;CHECK_WRITE(sizeof(value)==sizeof(data));memcpy(&data,&value,sizeof(data));return data;
}
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
void* __cdecl operator new[](size_t size) {
 CHECK_WRITE(size>0 && size<=4096);void* p=malloc(size);CHECK_WRITE(p);unsigned i=0;while(i<32 && g_charArrays[i])++i;CHECK_WRITE(i<32);
 g_charArrays[i]=p;++g_arrayAllocated;return p;
}
void __cdecl operator delete[](void* p) {
 unsigned i=0;while(i<32 && g_charArrays[i]!=p)++i;CHECK_WRITE(i<32 && p);g_charArrays[i]=0;++g_arrayReleased;free(p);
}
void* __cdecl operator new(size_t size) {
 if(size==sizeof(CCPPointerInfo)) {
  CHECK_WRITE(!g_controlAllocation && !g_controlBlock);
  g_controlAllocation=static_cast<unsigned char*>(malloc(44));CHECK_WRITE(g_controlAllocation);memset(g_controlAllocation,0x6b,44);
  g_controlBlock=reinterpret_cast<CCPPointerInfo*>(g_controlAllocation+16);++g_controlAllocated;return g_controlBlock;
 }
 if(size==sizeof(CCharStringData)) {
  void* p=malloc(size);CHECK_WRITE(p);unsigned i=0;while(i<16 && g_charBlocks[i])++i;CHECK_WRITE(i<16);
  g_charBlocks[i]=static_cast<CCharStringData*>(p);++g_charAllocated;return p;
 }
 CHECK_WRITE(size==sizeof(CWideStringData));void* p=malloc(size);CHECK_WRITE(p);
 unsigned i=0;while(i<16 && g_blocks[i])++i;CHECK_WRITE(i<16);g_blocks[i]=static_cast<CWideStringData*>(p);++g_allocated;return p;
}
void __cdecl operator delete(void* p) {
 if(p==g_controlBlock) {
  CHECK_WRITE(g_controlBlock && g_controlAllocation && g_controlBlock->RefCount==0 && g_fileFreed==1);
  CHECK_WRITE(g_controlBlock->Data==g_fileObject && g_controlBlock->DeleteFunc==&CCountedPointer<CThreadedFile>::DeleteData);
  for(unsigned guard=0;guard<16;++guard)CHECK_WRITE(g_controlAllocation[guard]==0x6b && g_controlAllocation[28+guard]==0x6b);
  free(g_controlAllocation);g_controlAllocation=0;g_controlBlock=0;++g_controlReleased;return;
 }
 if(p==g_fileObject) {
  CHECK_WRITE(g_fileAllocation && !g_fileFreed);
  DWORD finalVtable,finalFilename;memcpy(&finalVtable,p,4);memcpy(&finalFilename,static_cast<unsigned char*>(p)+12,4);
  CHECK_WRITE(finalVtable==reinterpret_cast<DWORD>(LifetimeBaseVtable) && finalFilename==0);
  memcpy(g_fileSnapshot,g_fileAllocation,96);free(g_fileAllocation);g_fileAllocation=0;g_fileFreed=1;return;
 }
 for(unsigned c=0;c<16;++c)if(g_charBlocks[c]==p) {CHECK_WRITE(g_charBlocks[c]->owners<=0);g_charBlocks[c]=0;++g_charReleased;free(p);return;}
 unsigned i=0;while(i<16 && g_blocks[i]!=p)++i;CHECK_WRITE(i<16 && g_blocks[i]->owners<=0);g_blocks[i]=0;++g_released;free(p);
}
CWideString CWideString::GetSubString(unsigned long,unsigned long) const {FailWrite("cold dot-path substring",__LINE__);}
CWideString __fastcall CAFile::GetAbsoluteFilePath(const CWideString&,const CWideString&) {FailWrite("cold parent-path normalization",__LINE__);}






typedef bool (__fastcall* OpenWrite)(CThreadedFile*,void*,const CWideString&,bool);
static bool __fastcall CompiledWrite(CThreadedFile* file,void*,const CWideString& name,bool overwrite) {return file->OpenForWrite(name,overwrite);}
extern "C" __declspec(dllimport) HANDLE __stdcall CreateFileW(const wchar_t*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
extern "C" __declspec(dllimport) DWORD __stdcall GetFileSize(HANDLE,DWORD*);
static HANDLE (__stdcall* pCreate)(const wchar_t*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE)=CreateFileW;
static DWORD (__stdcall* pSizeWrite)(HANDLE,DWORD*)=GetFileSize;
static void __fastcall DestroyWide(CWideString* value) {value->~CWideString();}
static DWORD g_oracleCounter;
static CWideString (__fastcall* pConvertWrite)(const CWideString&)=CAFile::ConvertPathToFullPath;
static DWORD WidePointerAddress() {
 const wchar_t* (CWideString::*member)() const=&CWideString::operator const wchar_t*;
 DWORD result;CHECK_WRITE(sizeof(member)==4);memcpy(&result,&member,4);return result;
}
static OpenWrite LoadWrite(const char* path) {
 unsigned char original[143];FILE* f=fopen(path,"rb");CHECK_WRITE(f);CHECK_WRITE(fread(original,1,143,f)==143 && fgetc(f)==EOF);CHECK_WRITE(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK_WRITE(code);memcpy(code,original,143);
 const DWORD sites[8]={12,60,67,84,99,107,117,133};
 const DWORD retail[8]={0x999110,0x99af00,0x143fe2c,0x99b510,0x143fdf0,0x13bc9e8,0x13bc9e8,0x99b510};
 const DWORD targets[8]={reinterpret_cast<DWORD>(pConvertWrite),WidePointerAddress(),reinterpret_cast<DWORD>(&pCreate),reinterpret_cast<DWORD>(DestroyWide),reinterpret_cast<DWORD>(&pSizeWrite),reinterpret_cast<DWORD>(&g_oracleCounter),reinterpret_cast<DWORD>(&g_oracleCounter),reinterpret_cast<DWORD>(DestroyWide)};
 for(unsigned i=0;i<8;++i) {
  DWORD old;memcpy(&old,original+sites[i],4);bool relative=i==0 || i==1 || i==3 || i==7;
  CHECK_WRITE((relative ? DWORD(0x98e000+sites[i]+4+LONG(old)):old)==retail[i]);
  DWORD target=relative ? targets[i]-(reinterpret_cast<DWORD>(code)+sites[i]+4):targets[i];memcpy(code+sites[i],&target,4);
 }
 DWORD old;CHECK_WRITE(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK_WRITE(FlushInstructionCache(GetCurrentProcess(),code,143));return reinterpret_cast<OpenWrite>(code);
}
static void PutWrite(unsigned char* p,unsigned offset,DWORD value) {memcpy(p+offset,&value,4);}
static void PrepareWrite(bool existing) {
 if(existing) {FILE* f=_wfopen(g_realPath,L"wb");CHECK_WRITE(f);for(unsigned i=0;i<1234;++i)CHECK_WRITE(fputc((i*37+23)&255,f)!=EOF);CHECK_WRITE(fclose(f)==0);}
 else {_wremove(g_realPath);FILE* f=_wfopen(g_realPath,L"rb");CHECK_WRITE(!f);}
}

extern "C" void __fastcall RootedOwnCopy(CWideString*,void*);
extern "C" CWideStringData* __fastcall RootedAllocateData(CWideString*,void*,const wchar_t*,long);
#pragma comment(linker,"/alternatename:@RootedOwnCopy@8=?MakeOwnCopyOfData@CWideString@@AAEXXZ")
#pragma comment(linker,"/alternatename:@RootedAllocateData@16=?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z")
typedef const CWideString& (__fastcall* CharacterAppend)(CWideString*,void*,wchar_t);
static wchar_t* (__fastcall* pRootedFill)(wchar_t*,size_t,const wchar_t&)=_STL::uninitialized_fill_n<wchar_t>;
static const CWideString& __fastcall RebuiltCharacter(CWideString* value,void*,wchar_t c) {return *value+=c;}
static DWORD ReserveAddressRooted() {
 void (_STL::basic_string<wchar_t>::*member)(size_t)=&_STL::basic_string<wchar_t>::reserve;
 DWORD result;CHECK_WRITE(sizeof(member)==4);memcpy(&result,&member,4);return result;
}
static DWORD LengthErrorAddressRooted() {
 typedef _STL::_String_base<wchar_t,_STL::allocator<wchar_t> > Base;
 void (Base::*member)() const=&Base::_M_throw_length_error;
 DWORD result;CHECK_WRITE(sizeof(member)==4);memcpy(&result,&member,4);return result;
}
static CharacterAppend LoadCharacterRooted(const char* path) {
 unsigned char original[203];FILE* f=fopen(path,"rb");CHECK_WRITE(f);CHECK_WRITE(fread(original,1,203,f)==203 && fgetc(f)==EOF);CHECK_WRITE(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK_WRITE(code);memcpy(code,original,203);
 const DWORD sites[5]={18,96,114,151,186},retail[5]={0x99b560,0x99c990,0x5123a0,0x99c550,0x99b3c0};
 const DWORD targets[5]={reinterpret_cast<DWORD>(RootedOwnCopy),ReserveAddressRooted(),reinterpret_cast<DWORD>(pRootedFill),LengthErrorAddressRooted(),reinterpret_cast<DWORD>(RootedAllocateData)};
 for(unsigned i=0;i<5;++i) {DWORD value;memcpy(&value,original+sites[i],4);CHECK_WRITE(DWORD(0x99b9a0+sites[i]+4+LONG(value))==retail[i]);value=targets[i]-(reinterpret_cast<DWORD>(code)+sites[i]+4);memcpy(code+sites[i],&value,4);}
 DWORD old;CHECK_WRITE(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK_WRITE(FlushInstructionCache(GetCurrentProcess(),code,203));return reinterpret_cast<CharacterAppend>(code);
}
static void CheckCharacterRooted(CharacterAppend original) {
 const wchar_t* initial[4]={L"",L"A",L"ABC",L"ABCD"};const wchar_t characters[4]={0,L'X',0xd800,0xffff};
 CharacterAppend modes[2]={original,RebuiltCharacter};unsigned cases=0,allocationCounts[2];
 for(unsigned mode=0;mode<2;++mode) {
  unsigned start=g_allocated;
  for(unsigned i=0;i<4;++i)for(unsigned c=0;c<4;++c)for(unsigned shared=0;shared<2;++shared)for(unsigned spare=0;spare<2;++spare) {
   {
    CWideString value(initial[i]);unsigned n=static_cast<unsigned>(value.GetLength());wchar_t expected[16];memcpy(expected,initial[i],(n+1)*2);
    CWideStringData* oldData=DataOf(value);if(spare && oldData) {oldData->Data.reserve(n+4);CHECK_WRITE(oldData->Data.capacity()==n+4);}
    unsigned long aliasStorage;if(shared)new(&aliasStorage) CWideString(value);
    CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==int(2+shared));
    const CWideString& returned=modes[mode](&value,0,characters[c]);CHECK_WRITE(&returned==&value);
    unsigned wanted=n;if(n || characters[c]) {expected[n]=characters[c];expected[n+1]=0;++wanted;}
    CHECK_WRITE(value.GetLength()==static_cast<long>(wanted) && memcmp(value.GetAsCharArray(),expected,(wanted+1)*2)==0);
    if(shared) {
     CWideString* alias=reinterpret_cast<CWideString*>(&aliasStorage);
     CHECK_WRITE(alias->GetLength()==static_cast<long>(n) && memcmp(alias->GetAsCharArray(),initial[i],(n+1)*2)==0);
     if(oldData)CHECK_WRITE(DataOf(value)!=oldData && oldData->owners==1);
     alias->~CWideString();
    }
   }
   CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);++cases;
  }
  allocationCounts[mode]=g_allocated-start;
 }
 CHECK_WRITE(cases==128 && allocationCounts[0]==allocationCounts[1]);
}
static void CheckReserveRooted() {
 const wchar_t* initial[3]={L"A",L"ABC",L"ABCDEFG"};unsigned checks=0;
 for(unsigned i=0;i<3;++i) {
  {CWideString value(initial[i]);CWideStringData* data=DataOf(value);unsigned n=static_cast<unsigned>(value.GetLength());
   data->Data.reserve(n+4);wchar_t* old=data->text;data->Data.reserve(n+4);
   CHECK_WRITE(data->text!=old && data->Data.capacity()==n+4 && memcmp(data->text,initial[i],(n+1)*2)==0);++checks;
   old=data->text;data->Data.reserve(0);CHECK_WRITE(data->text!=old && data->Data.capacity()==n && data->Data.size()==n && memcmp(data->text,initial[i],(n+1)*2)==0);++checks;
  }
  CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
 }
 CHECK_WRITE(checks==6);
}
static void CheckFillRooted() {
 const unsigned counts[4]={0,1,7,16};const wchar_t values[4]={0,L'X',0xd800,0xffff};unsigned cases=0;
 for(unsigned n=0;n<4;++n)for(unsigned v=0;v<4;++v) {
  wchar_t buffer[20],expected[20];for(unsigned i=0;i<20;++i)buffer[i]=expected[i]=0xa7a7;
  for(unsigned i=0;i<counts[n];++i)expected[2+i]=values[v];
  wchar_t* result=_STL::uninitialized_fill_n<wchar_t>(buffer+2,counts[n],values[v]);
  CHECK_WRITE(result==buffer+2+counts[n] && memcmp(buffer,expected,sizeof(buffer))==0);++cases;
 }
 CHECK_WRITE(cases==16);
}

static DWORD WordLife(const void* p) {DWORD x;memcpy(&x,p,4);return x;}
static void PutLife(void* p,DWORD x) {memcpy(p,&x,4);}
static void __fastcall BaseConstructLife(CBaseClassNonCopyable* p) {new(p) CBaseClassNonCopyable;}
static void __fastcall WideConstructLife(CWideString* p) {new(p) CWideString;}
static void __fastcall WideDestroyLife(CWideString* p) {p->~CWideString();}
static void __fastcall BaseDestroyLife(CBaseClassNonCopyable* p) {p->CBaseClassNonCopyable::~CBaseClassNonCopyable();}
static void* __fastcall RebuiltConstructLife(void* p,void*) {return new(p) CThreadedFile;}
static DWORD ManagerCloseAddressLife() {
 void(CThreadedFileManager::*m)(CThreadedFile*)=&CThreadedFileManager::CloseAllStreamsForFile;DWORD p;CHECK_WRITE(sizeof(m)==4);memcpy(&p,&m,4);return p;
}
static BOOL (__stdcall* pCloseLife)(HANDLE)=CloseHandle;
static void (__cdecl* pDeleteLife)(void*)=::operator delete;
typedef void* (__fastcall* ConstructLife)(void*,void*);
typedef void* (__fastcall* DeleteLife)(CThreadedFile*,void*,unsigned);
static void* LoadLife(const char* path,unsigned size,DWORD entry,bool constructor) {
 unsigned char raw[88];FILE* f=fopen(path,"rb");CHECK_WRITE(f);CHECK_WRITE(fread(raw,1,size,f)==size && fgetc(f)==EOF);CHECK_WRITE(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK_WRITE(code);memcpy(code,raw,size);
 const unsigned ctorSites[3]={4,13,18},deleteSites[7]={9,17,22,32,55,62,75};
 const DWORD ctorRetail[3]={0x99a2f0,0x129a158,0x99aed0},deleteRetail[7]={0x129a158,0x13bc870,0x98e7d0,0x143fe30,0x99b510,0x99a300,0xbfe9bc};
 const DWORD ctorTargets[3]={reinterpret_cast<DWORD>(BaseConstructLife),reinterpret_cast<DWORD>(LifetimeFileVtable),reinterpret_cast<DWORD>(WideConstructLife)};
 const DWORD deleteTargets[7]={reinterpret_cast<DWORD>(LifetimeFileVtable),reinterpret_cast<DWORD>(LifetimeManagerStorage),ManagerCloseAddressLife(),reinterpret_cast<DWORD>(&pCloseLife),reinterpret_cast<DWORD>(WideDestroyLife),reinterpret_cast<DWORD>(BaseDestroyLife),reinterpret_cast<DWORD>(pDeleteLife)};
 for(unsigned i=0;i<(constructor?3:7);++i) {
  unsigned site=constructor?ctorSites[i]:deleteSites[i];DWORD value=WordLife(raw+site);bool relative=constructor?i!=1:i==2 || i>=4;
  CHECK_WRITE((relative?DWORD(entry+site+4+LONG(value)):value)==(constructor?ctorRetail[i]:deleteRetail[i]));
  DWORD target=constructor?ctorTargets[i]:deleteTargets[i];PutLife(code+site,relative?target-(reinterpret_cast<DWORD>(code)+site+4):target);
 }
 DWORD old;CHECK_WRITE(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK_WRITE(FlushInstructionCache(GetCurrentProcess(),code,size));return code;
}

extern "C" {
__declspec(dllimport) DWORD __stdcall SetFilePointer(HANDLE,LONG,LONG*,DWORD);
__declspec(dllimport) BOOL __stdcall SetEndOfFile(HANDLE);
__declspec(dllimport) BOOL __stdcall GetDiskFreeSpaceW(const wchar_t*,DWORD*,DWORD*,DWORD*,DWORD*);
}
typedef bool (__fastcall* Resize)(CThreadedFile*,void*,DWORD);
static bool __fastcall CompiledResize(CThreadedFile* file,void*,DWORD size) {return file->ResizeFile(size);}
static DWORD (__stdcall* pPointer)(HANDLE,LONG,LONG*,DWORD)=SetFilePointer;
static BOOL (__stdcall* pEnd)(HANDLE)=SetEndOfFile;
static DWORD (__stdcall* pSize)(HANDLE,DWORD*)=GetFileSize;
static Resize LoadResize(const char* path) {
 unsigned char original[58];FILE* f=fopen(path,"rb");CHECK_WRITE(f);CHECK_WRITE(fread(original,1,58,f)==58 && fgetc(f)==EOF);CHECK_WRITE(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK_WRITE(code);memcpy(code,original,58);
 const DWORD sites[3]={19,29,41},retail[3]={0x143fde8,0x143fdec,0x143fdf0};
 const DWORD targets[3]={reinterpret_cast<DWORD>(&pPointer),reinterpret_cast<DWORD>(&pEnd),reinterpret_cast<DWORD>(&pSize)};
 for(unsigned i=0;i<3;++i) {DWORD value;memcpy(&value,original+sites[i],4);CHECK_WRITE(value==retail[i]);memcpy(code+sites[i],targets+i,4);}
 DWORD old;CHECK_WRITE(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK_WRITE(FlushInstructionCache(GetCurrentProcess(),code,58));return reinterpret_cast<Resize>(code);
}

typedef void (__fastcall* CountedReset)(CCountedPointer<CThreadedFile>*,void*,CThreadedFile*);
static void __fastcall RebuiltReset(CCountedPointer<CThreadedFile>* owner,void*,CThreadedFile* file) {owner->Reset(file);}
static CountedReset LoadCountedReset(const char* path) {
 unsigned char raw[103];FILE* f=fopen(path,"rb");CHECK_WRITE(f);CHECK_WRITE(fread(raw,1,103,f)==103 && fgetc(f)==EOF);CHECK_WRITE(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK_WRITE(code);memcpy(code,raw,103);
 const unsigned sites[3]={31,58,80};const DWORD retail[3]={0xbfe9bc,0xbfea1a,0x9a9040};
 void* (__cdecl* allocate)(size_t)=::operator new;
 const DWORD targets[3]={reinterpret_cast<DWORD>(pDeleteLife),reinterpret_cast<DWORD>(allocate),reinterpret_cast<DWORD>(&CCountedPointer<CThreadedFile>::DeleteData)};
 for(unsigned i=0;i<3;++i) {DWORD old=WordLife(raw+sites[i]);CHECK_WRITE((i<2?DWORD(0x9a9c80+sites[i]+4+LONG(old)):old)==retail[i]);PutLife(code+sites[i],i<2?targets[i]-(reinterpret_cast<DWORD>(code)+sites[i]+4):targets[i]);}
 DWORD old;CHECK_WRITE(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK_WRITE(FlushInstructionCache(GetCurrentProcess(),code,103));return reinterpret_cast<CountedReset>(code);
}

struct ResizeOutcome {DWORD returned,length,position,error;};
static ResizeOutcome g_resizeOutcomes[144][6];

extern "C" {
void* __fastcall ReadCharConstruct(CCharString*,void*,const char*,long);
void* __fastcall ReadBasicConstruct(CBasicString<char>*,void*,const char*);
void __fastcall ReadCharUnassign(CCharString*,void*);
__declspec(dllimport) BOOL __stdcall ReadFile(HANDLE,void*,DWORD,DWORD*,OVERLAPPED*);
__declspec(dllimport) BOOL __stdcall GetOverlappedResult(HANDLE,OVERLAPPED*,DWORD*,BOOL);
}
#pragma comment(linker,"/alternatename:@ReadCharConstruct@16=??0CCharString@@QAE@PBDJ@Z")
#pragma comment(linker,"/alternatename:@ReadBasicConstruct@12=??0?$CBasicString@D@@QAE@PBD@Z")
#pragma comment(linker,"/alternatename:@ReadCharUnassign@8=?UnassignString@CCharString@@AAEXXZ")
static CCharStringData* CharData(const CCharString& value) {CCharStringData* p;memcpy(&p,&value,4);return p;}
static void __fastcall DestroyReadChar(CCharString* value) {value->~CCharString();}
static CWideString& __fastcall AssignReadWide(CWideString* value,void*,const CWideString& other) {return *value=other;}
static DWORD ReadNarrowAddress() {CCharString(CWideString::*m)() const=&CWideString::ToCharString;DWORD p;CHECK_WRITE(sizeof(m)==4);memcpy(&p,&m,4);return p;}
static DWORD ReadUpperAddress() {CCharString(CCharString::*m)() const=&CCharString::GetAsUpperCase;DWORD p;CHECK_WRITE(sizeof(m)==4);memcpy(&p,&m,4);return p;}
static DWORD ReadEqualAddress() {bool(CBasicString<char>::*m)(const char*) const=&CBasicString<char>::operator==;DWORD p;CHECK_WRITE(sizeof(m)==4);memcpy(&p,&m,4);return p;}
static unsigned long g_readOracleCounter;
static const char g_readEmpty[1]={0},g_readDrive[3]={'D',':',0};
static void* LoadReadBlob(const char* path,unsigned size,DWORD entry,const unsigned* sites,const DWORD* retail,const DWORD* targets,const bool* relative,unsigned count) {
 unsigned char raw[310];CHECK_WRITE(size<=sizeof(raw));FILE* f=fopen(path,"rb");CHECK_WRITE(f);CHECK_WRITE(fread(raw,1,size,f)==size && fgetc(f)==EOF);CHECK_WRITE(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK_WRITE(code);memcpy(code,raw,size);
 for(unsigned i=0;i<count;++i) {DWORD old=WordLife(raw+sites[i]);CHECK_WRITE((relative[i]?DWORD(entry+sites[i]+4+LONG(old)):old)==retail[i]);PutLife(code+sites[i],relative[i]?targets[i]-(reinterpret_cast<DWORD>(code)+sites[i]+4):targets[i]);}
 DWORD old;CHECK_WRITE(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK_WRITE(FlushInstructionCache(GetCurrentProcess(),code,size));return code;
}
typedef void* (__fastcall* FinishHandle)(CThreadedFile*,void*);
static FinishHandle g_originalFinish;
static void* __fastcall RebuiltFinish(CThreadedFile* file,void*) {return file->FinishAndGetFileHandle();}
static FinishHandle LoadFinish(const char* path) {
 const unsigned sites[2]={11,16};const DWORD retail[2]={0x13bc870,0x98e7d0};
 const DWORD targets[2]={reinterpret_cast<DWORD>(LifetimeManagerStorage),ManagerCloseAddressLife()};const bool relative[2]={false,true};
 return reinterpret_cast<FinishHandle>(LoadReadBlob(path,25,0x98ec70,sites,retail,targets,relative,2));
}
static void CheckFinish(CThreadedFile* file) {
 FinishHandle modes[2]={g_originalFinish,RebuiltFinish};unsigned char before[96],expected[96];
 memcpy(before,g_fileAllocation,96);HANDLE handle=file->fileHandle_;unsigned previous=g_managerCloses;
 for(unsigned mode=0;mode<2;++mode)for(unsigned mutate=0;mutate<2;++mutate) {
  g_finishReplace=mutate!=0;unsigned callbacks=g_managerCloses;
  void* returned=modes[mode](file,0);bool valid=handle!=INVALID_HANDLE_VALUE;
  CHECK_WRITE(returned==(mutate && valid?INVALID_HANDLE_VALUE:handle));
  CHECK_WRITE(g_managerCloses==callbacks+unsigned(valid));
  memcpy(expected,before,96);if(mutate && valid)PutLife(expected+40,0xffffffff);
  CHECK_WRITE(memcmp(expected,g_fileAllocation,96)==0);
  file->fileHandle_=handle;CHECK_WRITE(memcmp(before,g_fileAllocation,96)==0);
  ++g_finishCalls;g_finishCallbacks+=unsigned(valid);g_finishMutations+=unsigned(valid && mutate);
 }
 g_finishReplace=false;g_managerCloses=previous;
}

extern "C" __declspec(dllimport) DWORD __stdcall GetFileAttributesW(const wchar_t*);
static DWORD (__stdcall* pAttributes)(const wchar_t*)=GetFileAttributesW;
typedef bool (__fastcall* PathExistsProbe)(const CWideString&);
static PathExistsProbe LoadPathExists(const char* path) {
 const unsigned sites[10]={11,20,32,42,51,60,69,76,90,101};
 const DWORD retail[10]={0x99aed0,0x997f80,0x998c90,0x99b7d0,0x99b510,0x99b510,0x99af00,0x143fdbc,0x99b510,0x99b510};
 CWideString (__fastcall* current)()=CAFile::GetCurrentPath;
 CWideString (__fastcall* expand)(const CWideString&,const CWideString&)=CAFile::ExpandPath;
 const DWORD targets[10]={reinterpret_cast<DWORD>(WideConstructLife),reinterpret_cast<DWORD>(current),reinterpret_cast<DWORD>(expand),reinterpret_cast<DWORD>(AssignReadWide),reinterpret_cast<DWORD>(DestroyWide),reinterpret_cast<DWORD>(DestroyWide),WidePointerAddress(),reinterpret_cast<DWORD>(&pAttributes),reinterpret_cast<DWORD>(DestroyWide),reinterpret_cast<DWORD>(DestroyWide)};
 const bool relative[10]={true,true,true,true,true,true,true,false,true,true};
 return reinterpret_cast<PathExistsProbe>(LoadReadBlob(path,111,0x999230,sites,retail,targets,relative,10));
}
static void CheckPathExists(const char* oraclePath) {
 PathExistsProbe original=LoadPathExists(oraclePath);PathExistsProbe modes[2]={original,CAFile::PathExists};
 wchar_t directory[320];wcscpy(directory,g_realPath);wchar_t* last=wcsrchr(directory,L'\\');CHECK_WRITE(last);*last=0;
 const wchar_t* base=wcsrchr(g_realPath,L'\\')+1;unsigned cases=0;
 for(unsigned state=0;state<4;++state)for(unsigned form=0;form<3;++form)for(unsigned alias=0;alias<2;++alias) {
  PrepareWrite(state==0);
  const wchar_t* input=state==3?L"":state==2?(form==2?directory+2:directory):(form==2?g_realPath+2:form==1?base:g_realPath);
  const wchar_t* resolved=state>=2?directory:g_realPath;DWORD attributes=GetFileAttributesW(resolved);
  CHECK_WRITE((attributes!=0xffffffff)==(state!=1));
  {
   CWideString name(input);unsigned long storage;CWideString* shared=reinterpret_cast<CWideString*>(&storage);if(alias)new(shared) CWideString(name);
   CWideStringData* data=DataOf(name);int owners=data?data->owners:0;int instances=g_CWideStringInstanceCount_013BCA20;
   for(unsigned mode=0;mode<2;++mode) {
    CHECK_WRITE(modes[mode](name)==(attributes!=0xffffffff));
    CHECK_WRITE(DataOf(name)==data && wcscmp(name,input)==0 && g_CWideStringInstanceCount_013BCA20==instances);
    if(data)CHECK_WRITE(data->owners==owners);++cases;
   }
   if(alias)shared->~CWideString();
  }
  CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
 }
 CHECK_WRITE(cases==48 && VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE));
 printf("NATIVE_PATH_EXISTS_DETAIL cases=48 file_directory_missing_empty=1\n");
}

char g_FableEmptyNarrowString_0129AAF4[1]={0};
struct QueryAttributes {DWORD attributes,creation[2],access[2],write[2],high,low;};
extern "C" {
__declspec(dllimport) BOOL __stdcall GetFileAttributesExW(const wchar_t*,int,void*);
__declspec(dllimport) BOOL __stdcall SetFileAttributesW(const wchar_t*,DWORD);
}
static void CheckFileQueries() {
 wchar_t directory[320];wcscpy(directory,g_realPath);wchar_t* last=wcsrchr(directory,L'\\');CHECK_WRITE(last);*last=0;
 const wchar_t* base=wcsrchr(g_realPath,L'\\')+1;unsigned snapshots=0,modified=0,sets=0;
 for(unsigned state=0;state<4;++state)for(unsigned form=0;form<3;++form)for(unsigned alias=0;alias<2;++alias) {
  PrepareWrite(state==0);
  const wchar_t* input=state==3?L"":state==2?(form==2?directory+2:directory):(form==2?g_realPath+2:form==1?base:g_realPath);
  const wchar_t* resolved=state>=2?directory:g_realPath;
  {
   CWideString name(input);unsigned long storage;CWideString* shared=reinterpret_cast<CWideString*>(&storage);if(alias)new(shared) CWideString(name);
   CWideStringData* data=DataOf(name);int owners=data?data->owners:0;int instances=g_CWideStringInstanceCount_013BCA20;
   for(unsigned step=0;step<(state==0?3u:1u);++step) {
    DWORD before=GetFileAttributesW(resolved);
    if(state==0) {
     CAFile::SetFileReadOnly(name,step==1);++sets;
     CHECK_WRITE(GetFileAttributesW(resolved)==(step==1?before|1:before&~1ul));
    }
    DWORD attributes=GetFileAttributesW(resolved);
    CHECK_WRITE(CAFile::IsFileReadOnly(name)==((attributes&1)!=0));
    CHECK_WRITE(CAFile::IsPathFile(name)==(attributes!=0xffffffff && !(attributes&16)));
    if(state==0) {
     QueryAttributes expected;memset(&expected,0,sizeof(expected));CHECK_WRITE(sizeof(expected)==36 && GetFileAttributesExW(resolved,0,&expected));
     unsigned __int64 timestamp=(static_cast<unsigned __int64>(expected.write[1])<<32)|expected.write[0];
     CHECK_WRITE(timestamp && CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(name)==timestamp);++modified;
    }
    CHECK_WRITE(DataOf(name)==data && wcscmp(name,input)==0 && g_CWideStringInstanceCount_013BCA20==instances);
    if(data)CHECK_WRITE(data->owners==owners);++snapshots;
   }
   if(alias)shared->~CWideString();
  }
  CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
 }
 CHECK_WRITE(snapshots==36 && modified==18 && sets==18);
 printf("NATIVE_FILE_QUERY_DETAIL snapshots=36 query_calls=72 modified=18 sets=18 missing_readonly=1\n");
}

typedef CCharString* (__fastcall* NarrowRead)(const CWideString*,void*,CCharString*);
typedef CCharString* (__fastcall* UpperRead)(const CCharString*,void*,CCharString*);
static CCharString* __fastcall RebuiltNarrowRead(const CWideString* input,void*,CCharString* output) {return new(output) CCharString(input->ToCharString());}
static CCharString* __fastcall RebuiltUpperRead(const CCharString* input,void*,CCharString* output) {return new(output) CCharString(input->GetAsUpperCase());}
static NarrowRead LoadReadNarrow(const char* path) {
 const unsigned sites[4]={32,95,152,162};const DWORD retail[4]={0xbfea0e,0x129a8e0,0x99ebf0,0xbfea14};
 const DWORD targets[4]={reinterpret_cast<DWORD>(&malloc),reinterpret_cast<DWORD>(g_FableEmptyWideString_0129A8E0),reinterpret_cast<DWORD>(ReadCharConstruct),reinterpret_cast<DWORD>(&free)};
 const bool relative[4]={true,false,true,true};return reinterpret_cast<NarrowRead>(LoadReadBlob(path,177,0x99b2c0,sites,retail,targets,relative,4));
}
static UpperRead LoadReadUpper(const char* path) {
 void* (__cdecl* allocate)(size_t)=::operator new;void (__cdecl* releaseArray)(void*)=::operator delete[];
 int (__cdecl* uppercase)(int)=toupper;
 const unsigned sites[12]={5,20,57,74,96,132,159,172,186,224,257,266};
 const DWORD retail[12]={0x13bd800,0x13bd800,0xbfea1a,0x9a0590,0x99e9b0,0xbfed16,0x13bd800,0x13bd800,0x99e9b0,0xbfeb1c,0xbfe9bc,0x13bd800};
 DWORD counter=reinterpret_cast<DWORD>(&g_CCharStringInstanceCount_013BD800);
 const DWORD targets[12]={counter,counter,reinterpret_cast<DWORD>(allocate),reinterpret_cast<DWORD>(ReadBasicConstruct),reinterpret_cast<DWORD>(ReadCharUnassign),reinterpret_cast<DWORD>(uppercase),counter,counter,reinterpret_cast<DWORD>(ReadCharUnassign),reinterpret_cast<DWORD>(releaseArray),reinterpret_cast<DWORD>(pDeleteLife),counter};
 const bool relative[12]={false,false,true,true,true,true,false,false,true,true,true,false};return reinterpret_cast<UpperRead>(LoadReadBlob(path,278,0x99ee20,sites,retail,targets,relative,12));
}
static bool __fastcall CompiledRead(CThreadedFile* file,void*,const CWideString& name,bool noCaching) {return file->Open(name,noCaching);}
static OpenWrite LoadReadOpen(const char* path) {
 const unsigned sites[21]={16,25,44,67,74,87,92,113,118,129,138,189,196,213,222,245,257,265,275,287,296};
 const DWORD retail[21]={0x999110,0x99aed0,0x997620,0x99b2c0,0x99ee20,0x122d70e,0x129a15c,0x129a15c,0x4115a0,0x99eae0,0x99eae0,0x99af00,0x143fe2c,0x99b510,0x99b510,0x99b7d0,0x143fdf0,0x13bc9ec,0x13bc9ec,0x99b510,0x99b510};
 void (__fastcall* split)(const CWideString&,CWideString*,CWideString*,CWideString*,CWideString*)=CAFile::SplitPath;
 const DWORD targets[21]={reinterpret_cast<DWORD>(pConvertWrite),reinterpret_cast<DWORD>(WideConstructLife),reinterpret_cast<DWORD>(split),ReadNarrowAddress(),ReadUpperAddress(),reinterpret_cast<DWORD>(g_readEmpty),reinterpret_cast<DWORD>(g_readDrive),reinterpret_cast<DWORD>(g_readDrive),ReadEqualAddress(),reinterpret_cast<DWORD>(DestroyReadChar),reinterpret_cast<DWORD>(DestroyReadChar),WidePointerAddress(),reinterpret_cast<DWORD>(&pCreate),reinterpret_cast<DWORD>(DestroyWide),reinterpret_cast<DWORD>(DestroyWide),reinterpret_cast<DWORD>(AssignReadWide),reinterpret_cast<DWORD>(&pSizeWrite),reinterpret_cast<DWORD>(&g_readOracleCounter),reinterpret_cast<DWORD>(&g_readOracleCounter),reinterpret_cast<DWORD>(DestroyWide),reinterpret_cast<DWORD>(DestroyWide)};
 const bool relative[21]={true,true,true,true,true,false,false,false,true,true,true,true,false,true,true,true,false,false,false,true,true};
 return reinterpret_cast<OpenWrite>(LoadReadBlob(path,310,0x98e1e0,sites,retail,targets,relative,21));
}
typedef CWideString* (__fastcall* WidenProbe)(const CCharString*,void*,CWideString*);
static void* __fastcall CopyWiden(CWideString* dest,void*,const CWideString& src) {return new(dest) CWideString(src);}
static CWideString* __fastcall RebuiltWiden(const CCharString* src,void*,CWideString* dest) {return new(dest) CWideString(src->ToWideString());}
static WidenProbe LoadWiden(const char* path) {
 const unsigned sites[3]={9,25,34};const DWORD retail[3]={0x99b6a0,0x99b720,0x99b510};
 CWideString (__fastcall* factory)(const CCharString&)=CWideString::CreateFromCharString;
 const DWORD targets[3]={reinterpret_cast<DWORD>(factory),reinterpret_cast<DWORD>(CopyWiden),reinterpret_cast<DWORD>(DestroyWide)};const bool relative[3]={true,true,true};
 return reinterpret_cast<WidenProbe>(LoadReadBlob(path,45,0x99e480,sites,retail,targets,relative,3));
}
static void CheckNarrowPaths(const char* originalPath) {
 WidenProbe original=LoadWiden(originalPath);WidenProbe modes[2]={original,RebuiltWiden};
 const char high[]={char(0x80),char(0xff),0},embedded[]={'A',0,'B',0};
 const char* inputs[7]={"","abc",high,embedded,"D:","x.y","\\path"};const long sizes[7]={0,3,2,3,2,3,5};unsigned widen=0,fill=0,paths=0;
 for(unsigned n=0;n<7;++n)for(unsigned mode=0;mode<2;++mode) {
  {
   CCharString input(inputs[n],sizes[n]);CCharStringData* originalData=CharData(input);unsigned long storage;CWideString* output=reinterpret_cast<CWideString*>(&storage);
   CHECK_WRITE(modes[mode](&input,0,output)==output && output->GetLength()==sizes[n]);
   for(long i=0;i<sizes[n];++i)CHECK_WRITE(output->GetAsCharArray()[i]==static_cast<unsigned char>(inputs[n][i]));
   CHECK_WRITE(output->GetAsCharArray()[sizes[n]]==0 && CharData(input)==originalData && input.GetLength()==sizes[n]);
   if(originalData)CHECK_WRITE(originalData->owners==1 && memcmp(originalData->Data.PData,inputs[n],sizes[n])==0);
   CHECK_WRITE(DataOf(*output) && DataOf(*output)->owners==1);output->~CWideString();++widen;
  }
  CHECK_WRITE(g_CCharStringInstanceCount_013BD800==0 && g_charAllocated==g_charReleased && g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
 }
 const unsigned counts[8]={0,1,2,3,7,8,31,64};const wchar_t characters[4]={0,L' ',0x80,0xffff};
 for(unsigned count=0;count<8;++count)for(unsigned ch=0;ch<4;++ch) {
  _STL::basic_string<wchar_t> value(counts[count],characters[ch]);
  CHECK_WRITE(value.size()==counts[count] && value.capacity()==counts[count] && value._M_start[counts[count]]==0);
  for(unsigned i=0;i<counts[count];++i)CHECK_WRITE(value._M_start[i]==characters[ch]);
  value._M_deallocate_block();++fill;
 }
 char absolute[320];CHECK_WRITE(wcstombs(absolute,g_realPath,319)!=static_cast<size_t>(-1));const char* base=strrchr(absolute,'\\')+1;
 char directory[320];strcpy(directory,absolute);char* last=strrchr(directory,'\\');CHECK_WRITE(last);last[1]=0;
 const char* names[4]={absolute,base,absolute+2,""};
 for(unsigned n=0;n<4;++n) {
  {
   CCharString name(names[n],-1);CCharString result=CAFile::ConvertPathToFullPath(name);
   CHECK_WRITE(strcmp(result.GetConstCharPtr(),n==3?directory:absolute)==0 && strcmp(name.GetConstCharPtr(),names[n])==0);++paths;
  }
  CHECK_WRITE(g_CCharStringInstanceCount_013BD800==0 && g_charAllocated==g_charReleased && g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
 }
 CHECK_WRITE(widen==14 && fill==32 && paths==4 && VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE));
 printf("NATIVE_NARROW_PATH_DETAIL widen=14 fill=32 paths=4 char_allocations=%u char_releases=%u array_allocations=%u array_releases=%u\n",g_charAllocated,g_charReleased,g_arrayAllocated,g_arrayReleased);
}

static void CheckReadConversions(NarrowRead originalNarrow,UpperRead originalUpper) {
 const wchar_t high[]={0x141,0x17a,0},embedded[]={L'A',0x100,L'B',0},nonAscii[]={0xe9,0x80,0xff,0};
 const wchar_t* inputs[7]={L"",L"d:",L"D:",L"aBc-09",high,embedded,nonAscii};
 NarrowRead narrow[2]={RebuiltNarrowRead,originalNarrow};UpperRead upper[2]={RebuiltUpperRead,originalUpper};unsigned cases=0;
 for(unsigned n=0;n<7;++n)for(unsigned nm=0;nm<2;++nm)for(unsigned um=0;um<2;++um) {
  {
   CWideString wide(inputs[n]);char expected[32],capital[32];unsigned length=0;
   while(inputs[n][length] && static_cast<unsigned char>(inputs[n][length])!=0) {expected[length]=static_cast<char>(inputs[n][length]);capital[length]=expected[length]>='a' && expected[length]<='z'?expected[length]-32:expected[length];++length;}
   expected[length]=capital[length]=0;
   unsigned long a,b;CCharString* value=reinterpret_cast<CCharString*>(&a);CCharString* result=reinterpret_cast<CCharString*>(&b);
   CHECK_WRITE(narrow[nm](&wide,0,value)==value);CCharStringData* originalData=CharData(*value);
   CHECK_WRITE(value->GetLength()==static_cast<long>(length) && strcmp(originalData?originalData->Data.PData:"",expected)==0);
   CHECK_WRITE(upper[um](value,0,result)==result);CCharStringData* resultData=CharData(*result);
   CHECK_WRITE(result->GetLength()==static_cast<long>(length) && strcmp(resultData?resultData->Data.PData:"",capital)==0);
   CHECK_WRITE(CharData(*value)==originalData && strcmp(originalData?originalData->Data.PData:"",expected)==0);
   if(originalData)CHECK_WRITE(originalData->owners==1 && resultData!=originalData && resultData->owners==1);
   CHECK_WRITE(wcscmp(wide,inputs[n])==0);result->~CCharString();value->~CCharString();
  }
  CHECK_WRITE(g_CCharStringInstanceCount_013BD800==0 && g_charAllocated==g_charReleased && g_arrayAllocated==g_arrayReleased && g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);++cases;
 }
 CHECK_WRITE(cases==28);
}
static void CheckReadPath(const char* readPath,const char* narrowPath,const char* upperPath,ConstructLife originalCtor,CountedReset originalReset,DWORD sectorBytes) {
 NarrowRead originalNarrow=LoadReadNarrow(narrowPath);UpperRead originalUpper=LoadReadUpper(upperPath);CheckReadConversions(originalNarrow,originalUpper);
 OpenWrite originalRead=LoadReadOpen(readPath);OpenWrite modes[2]={CompiledRead,originalRead};CountedReset resets[2]={RebuiltReset,originalReset};
 const wchar_t* base=wcsrchr(g_realPath,L'\\');CHECK_WRITE(base);++base;
 wchar_t missing[320],relativeMissing[320];wcscpy(missing,g_realPath);wcscat(missing,L".missing-parent\\child.bin");wcscpy(relativeMissing,base);wcscat(relativeMissing,L".missing-parent\\child.bin");
 unsigned cases=0,opens=0,failures=0,reads=0;DWORD payloadSize=2*sectorBytes+17;
 unsigned char* readBuffer=static_cast<unsigned char*>(VirtualAlloc(0,sectorBytes,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK_WRITE(readBuffer);
 const unsigned copies[3]={0,1,3};
 for(unsigned mode=0;mode<2;++mode) {
  unsigned key=0;
  for(unsigned noCaching=0;noCaching<2;++noCaching)for(unsigned state=0;state<4;++state)for(unsigned form=0;form<3;++form)for(unsigned share=0;share<3;++share) {
   PrepareWrite(false);
   if(state==0) {FILE* f=_wfopen(g_realPath,L"wb");CHECK_WRITE(f);for(DWORD i=0;i<payloadSize;++i)CHECK_WRITE(fputc((i*37+23)&255,f)!=EOF);CHECK_WRITE(fclose(f)==0);}
   {
    const wchar_t* input=state==3?L"":state==2?(form==2?missing+2:form==1?relativeMissing:missing):(form==2?g_realPath+2:form==1?base:g_realPath);
    CWideString name(input);unsigned long aliases[3];for(unsigned a=0;a<copies[share];++a)new(aliases+a) CWideString(name);
    CWideStringData* nameData=DataOf(name);g_fileAllocation=static_cast<unsigned char*>(malloc(96));CHECK_WRITE(g_fileAllocation);memset(g_fileAllocation,0xa7,96);
    unsigned char expected[96];memcpy(expected,g_fileAllocation,96);g_fileObject=reinterpret_cast<CThreadedFile*>(g_fileAllocation+32);g_fileFreed=g_managerCloses=0;
    CHECK_WRITE((mode?originalCtor:RebuiltConstructLife)(g_fileObject,0)==g_fileObject);
    PutWrite(expected+32,0,reinterpret_cast<DWORD>(LifetimeFileVtable));PutWrite(expected+32,8,0xffffffff);PutWrite(expected+32,12,0);expected[32+24]=expected[32+25]=0;
    CHECK_WRITE(memcmp(expected,g_fileAllocation,96)==0);
    CCountedPointer<CThreadedFile> owner;resets[mode](&owner,0,g_fileObject);CHECK_WRITE(owner.data_==g_fileObject && owner.control_==g_controlBlock && g_controlBlock->RefCount==1);
    bool result=modes[mode](g_fileObject,0,name,noCaching!=0);CHECK_WRITE(result==(state==0));
    PutWrite(expected+32,4,g_realPath[0]==L'D' || g_realPath[0]==L'd'?1:0);PutWrite(expected+32,8,reinterpret_cast<DWORD>(g_fileObject->fileHandle_));
    if(result) {PutWrite(expected+32,12,reinterpret_cast<DWORD>(nameData));PutWrite(expected+32,16,payloadSize);PutWrite(expected+32,20,++key);expected[32+25]=1;++opens;}
    else {CHECK_WRITE(g_fileObject->fileHandle_==INVALID_HANDLE_VALUE);++failures;}
    CHECK_WRITE(memcmp(expected,g_fileAllocation,96)==0 && g_CWideStringInstanceCount_013BCA20==int(3+copies[share]));
    CHECK_WRITE(g_CCharStringInstanceCount_013BD800==0 && g_charAllocated==g_charReleased && g_arrayAllocated==g_arrayReleased && DataOf(name)==nameData);
    if(nameData)CHECK_WRITE(nameData->owners==int(1+copies[share]+unsigned(result)));
    CheckFinish(g_fileObject);
    HANDLE handle=g_fileObject->fileHandle_;
    if(result) {
     OVERLAPPED overlapped;memset(&overlapped,0,sizeof(overlapped));overlapped.hEvent=CreateEventA(0,1,0,0);CHECK_WRITE(overlapped.hEvent);DWORD count=0;
     BOOL started=ReadFile(handle,readBuffer,sectorBytes,&count,&overlapped);CHECK_WRITE(started || GetLastError()==997);
     CHECK_WRITE(GetOverlappedResult(handle,&overlapped,&count,1) && count==sectorBytes);
     for(DWORD i=0;i<count;++i)CHECK_WRITE(readBuffer[i]==static_cast<unsigned char>(i*37+23));CHECK_WRITE(CloseHandle(overlapped.hEvent));++reads;
    }
    resets[mode](&owner,0,0);CHECK_WRITE(!owner.data_ && !owner.control_ && g_fileFreed && !g_controlBlock && g_managerCloses==unsigned(result));
    PutWrite(expected+32,0,reinterpret_cast<DWORD>(LifetimeBaseVtable));PutWrite(expected+32,8,0xffffffff);PutWrite(expected+32,12,0);expected[32+25]=0;
    CHECK_WRITE(memcmp(expected,g_fileSnapshot,96)==0);
    if(result) {DWORD handleFlags;CHECK_WRITE(!GetHandleInformation(handle,&handleFlags) && GetLastError()==6);}
    g_fileObject=0;for(unsigned a=copies[share];a>0;--a)reinterpret_cast<CWideString*>(aliases+a-1)->~CWideString();
   }
   CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1 && g_controlAllocated==g_controlReleased);++cases;
  }
 }
 CHECK_WRITE(cases==144 && opens==36 && failures==108 && reads==36);
 CHECK_WRITE(VirtualFree(readBuffer,0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalRead),0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalNarrow),0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalUpper),0,MEM_RELEASE));
 printf("NATIVE_READ_PATH_DETAIL cases=144 opens=36 failures=108 reads=36 conversion_cases=28 char_allocations=%u char_releases=%u array_allocations=%u array_releases=%u\n",g_charAllocated,g_charReleased,g_arrayAllocated,g_arrayReleased);
}
int main(int argc,char** argv) {
 CHECK_WRITE(argc==14 && mbstowcs(g_realPath,argv[2],259)!=static_cast<size_t>(-1));
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1);
 wchar_t working[260];CHECK_WRITE(_wgetcwd(working,260)==working);
 wchar_t directory[264];wcscpy(directory,working);wcscat(directory,L"\\");
 {CWideString initial=CAFile::GetCurrentPath();CHECK_WRITE(wcscmp(initial,working)==0);}
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
 g_originalFinish=LoadFinish(argv[11]);
 CharacterAppend originalCharacter=LoadCharacterRooted(argv[3]);CheckCharacterRooted(originalCharacter);CheckReserveRooted();CheckFillRooted();
 wchar_t missing[320];wcscpy(missing,g_realPath);wcscat(missing,L".missing-parent\\child.bin");
 OpenWrite original=LoadWrite(argv[1]);OpenWrite modes[2]={CompiledWrite,original};unsigned cases=0,opens=0,failures=0;
 ConstructLife originalCtor=reinterpret_cast<ConstructLife>(LoadLife(argv[4],41,0x98dfd0,true));
 DeleteLife originalDelete=reinterpret_cast<DeleteLife>(LoadLife(argv[5],88,0x991450,false));
 unsigned virtualDeletes=0,heapDeletes=0,managerCalls=0,resizeCalls=0,unalignedFailures=0,invalidTrue=0,countedCases=0,sharedReleases=0;
 CountedReset originalReset=LoadCountedReset(argv[7]);CountedReset resetModes[2]={RebuiltReset,originalReset};
 Resize originalResize=LoadResize(argv[6]);Resize resizeModes[2]={CompiledResize,originalResize};
 wchar_t volume[4]={working[0],L':',L'\\',0};DWORD sectors,sectorBytes,freeClusters,totalClusters;
 CHECK_WRITE(GetDiskFreeSpaceW(volume,&sectors,&sectorBytes,&freeClusters,&totalClusters));
 CHECK_WRITE(sectorBytes>=128 && sectorBytes<=65536 && (sectorBytes&(sectorBytes-1))==0);
 memset(LifetimeManagerStorage,0x6d,sizeof(LifetimeManagerStorage));
 const unsigned copies[3]={0,1,3};unsigned modeAllocations[2];
 const wchar_t* basename=wcsrchr(g_realPath,L'\\');CHECK_WRITE(basename);++basename;
 wchar_t relativeMissing[320];wcscpy(relativeMissing,basename);wcscat(relativeMissing,L".missing-parent\\child.bin");
 for(unsigned mode=0;mode<2;++mode) {
  unsigned key=0,startAllocations=g_allocated;
  for(unsigned flags=0;flags<2;++flags)for(unsigned overwrite=0;overwrite<2;++overwrite)for(unsigned state=0;state<4;++state)for(unsigned share=0;share<3;++share)for(unsigned form=0;form<3;++form)for(unsigned lifetime=0;lifetime<(mode==0?4:3);++lifetime) {
   CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1);PrepareWrite(state==0);g_copies=copies[share];
   {
    const wchar_t* input=state==3?L"":state==2?(form==2?missing+2:form==1?relativeMissing:missing):(form==2?g_realPath+2:form==1?basename:g_realPath);
    CWideString name(input);g_expectedName=&name;
    unsigned long aliasStorage[3];
    for(unsigned a=0;a<g_copies;++a) {
     if(a==1) {CWideString* alias=new(aliasStorage+a) CWideString;*alias=CWideString(L"replace this alias");*alias=name;}
     else new(aliasStorage+a) CWideString(name);
    }
    CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==int(2+g_copies));CWideStringData* data=DataOf(name);
    if(data)CHECK_WRITE(data->owners==int(1+g_copies));
    unsigned char expected[96];g_fileAllocation=static_cast<unsigned char*>(malloc(96));CHECK_WRITE(g_fileAllocation);
    unsigned char* storage=g_fileAllocation;memset(storage,0xa7,96);memcpy(expected,storage,96);
    g_fileObject=reinterpret_cast<CThreadedFile*>(storage+32);g_fileFreed=g_managerCloses=0;
    ConstructLife construct=mode==0?RebuiltConstructLife:originalCtor;
    CHECK_WRITE(construct(g_fileObject,0)==g_fileObject);
    PutWrite(expected+32,0,reinterpret_cast<DWORD>(LifetimeFileVtable));PutWrite(expected+32,8,0xffffffff);PutWrite(expected+32,0xc,0);
    expected[32+0x18]=expected[32+0x19]=0;CHECK_WRITE(memcmp(storage,expected,96)==0);
    bool counted=lifetime==(mode==0?3:2);unsigned long ownerStorage[10],ownerExpected[10];
    memset(ownerStorage,0x39,sizeof(ownerStorage));memcpy(ownerExpected,ownerStorage,sizeof(ownerStorage));
    CCountedPointer<CThreadedFile>* owner=0;
    if(counted) {
     owner=new(ownerStorage+4) CCountedPointer<CThreadedFile>;
     resetModes[mode](owner,0,g_fileObject);CHECK_WRITE(owner->data_==g_fileObject && owner->control_==g_controlBlock && g_controlBlock);
     CHECK_WRITE(g_controlBlock->RefCount==1 && g_controlBlock->Data==g_fileObject && g_controlBlock->DeleteFunc==&CCountedPointer<CThreadedFile>::DeleteData);
     ownerExpected[4]=reinterpret_cast<DWORD>(g_fileObject);ownerExpected[5]=reinterpret_cast<DWORD>(g_controlBlock);
     CHECK_WRITE(memcmp(ownerStorage,ownerExpected,sizeof(ownerStorage))==0);
    }
    g_fileObject->filenameStorage_=name;
    storage[32+0x18]=storage[32+0x19]=static_cast<unsigned char>(flags);
    PutWrite(expected+32,0xc,reinterpret_cast<DWORD>(data));expected[32+0x18]=expected[32+0x19]=static_cast<unsigned char>(flags);
    CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==int(3+g_copies));
    {
     CWideString resolved=CAFile::ConvertPathToFullPath(name);
     CHECK_WRITE(wcscmp(resolved,state==3?directory:state==2?missing:g_realPath)==0);
    }
    CThreadedFile* file=reinterpret_cast<CThreadedFile*>(storage+32);bool result=modes[mode](file,0,name,overwrite!=0);
    CHECK_WRITE(result==(state<2) && g_CWideStringInstanceCount_013BCA20==int(3+g_copies));
    CHECK_WRITE(DataOf(name)==data);if(data)CHECK_WRITE(data->owners==int(2+g_copies));
    PutWrite(expected+32,4,0);PutWrite(expected+32,8,reinterpret_cast<DWORD>(file->fileHandle_));
    if(result) {
     unsigned size=state==0 && !overwrite?1234:0;CHECK_WRITE(file->fileHandle_!=INVALID_HANDLE_VALUE && GetFileSize(file->fileHandle_,0)==size);
     PutWrite(expected+32,0x10,size);PutWrite(expected+32,0x14,++key);expected[32+0x18]=expected[32+0x19]=1;
     ++opens;
    } else {CHECK_WRITE(file->fileHandle_==INVALID_HANDLE_VALUE);++failures;}
    CHECK_WRITE(memcmp(storage,expected,96)==0);
    unsigned resizeIndex=((((flags*2+overwrite)*4+state)*3+share)*3+form);
    DWORD requests[6]={sectorBytes,sectorBytes*4,sectorBytes*4+1,sectorBytes*2,sectorBytes*2,flags?(result?0:0xffffffff):sectorBytes};
    DWORD preserved=state==0 && !overwrite?1234:0;
    for(unsigned step=0;step<6;++step) {
     SetLastError(0);bool resized=resizeModes[mode](file,0,requests[step]);DWORD error=GetLastError();
     DWORD actual=GetFileSize(file->fileHandle_,0),position=SetFilePointer(file->fileHandle_,0,0,1);
     CHECK_WRITE(resized==(requests[step]==actual) && file->length_==actual);
     PutWrite(expected+32,0x10,actual);CHECK_WRITE(memcmp(storage,expected,96)==0);
     CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==int(3+g_copies) && g_managerCloses==0);
     if(data)CHECK_WRITE(data->owners==int(2+g_copies));
     ResizeOutcome current={resized?1:0,actual,position,error};
     if(mode==0 && lifetime==0)g_resizeOutcomes[resizeIndex][step]=current;
     else CHECK_WRITE(memcmp(&current,&g_resizeOutcomes[resizeIndex][step],sizeof(current))==0);
     if(result) {
      CHECK_WRITE(actual==position);if(actual<preserved)preserved=actual;
      if(step!=2)CHECK_WRITE(resized && position==requests[step]);
      else if(!resized) {CHECK_WRITE(actual==requests[1]);++unalignedFailures;}
     } else {CHECK_WRITE(actual==0xffffffff && position==0xffffffff);if(resized)++invalidTrue;}
     ++resizeCalls;
    }
    CheckFinish(file);
    HANDLE handle=file->fileHandle_;bool freed=lifetime!=0;
    if(counted) {
     CHECK_WRITE(memcmp(ownerStorage,ownerExpected,sizeof(ownerStorage))==0 && g_controlBlock->RefCount==1);
     if(share) {
      // Explicit shared-owner setup; the specialized class has no recovered copy API.
      CCountedPointer<CThreadedFile> alias;alias.data_=owner->data_;alias.control_=owner->control_;++alias.control_->RefCount;
      resetModes[mode](&alias,0,0);CHECK_WRITE(!alias.data_ && !alias.control_ && !g_fileFreed && g_managerCloses==0);
      CHECK_WRITE(owner->control_==g_controlBlock && g_controlBlock->RefCount==1);++sharedReleases;
      if(result) {DWORD handleFlags;CHECK_WRITE(GetHandleInformation(handle,&handleFlags));}
     }
     resetModes[mode](owner,0,0);ownerExpected[4]=ownerExpected[5]=0;
     CHECK_WRITE(memcmp(ownerStorage,ownerExpected,sizeof(ownerStorage))==0 && !g_controlBlock && g_controlAllocated==g_controlReleased);
     owner->~CCountedPointer<CThreadedFile>();++countedCases;
    } else if(lifetime==2) {delete file;++virtualDeletes;}
    else CHECK_WRITE((mode==0?LifetimeDeleting:originalDelete)(file,0,lifetime)==file);
    CHECK_WRITE(g_fileFreed==unsigned(freed) && g_managerCloses==unsigned(result));
    PutWrite(expected+32,0,reinterpret_cast<DWORD>(LifetimeBaseVtable));PutWrite(expected+32,8,0xffffffff);PutWrite(expected+32,0xc,0);
    if(result)expected[32+0x18]=expected[32+0x19]=0;
    CHECK_WRITE(memcmp(freed?g_fileSnapshot:storage,expected,96)==0);
    if(result) {DWORD handleFlags;CHECK_WRITE(!GetHandleInformation(handle,&handleFlags) && GetLastError()==6);}
    if(result) {
     DWORD size=requests[5];FILE* f=_wfopen(g_realPath,L"rb");CHECK_WRITE(f);
     for(DWORD i=0;i<size;++i) {int value=fgetc(f);CHECK_WRITE(value!=EOF);if(i<preserved)CHECK_WRITE(value==int((i*37+23)&255));}
     CHECK_WRITE(fgetc(f)==EOF && fclose(f)==0);
    }
    CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==int(2+g_copies));
    if(data)CHECK_WRITE(data->owners==int(1+g_copies));
    if(!freed) {free(g_fileAllocation);g_fileAllocation=0;}else ++heapDeletes;
    g_fileObject=0;managerCalls+=g_managerCloses;++cases;
    for(unsigned a=g_copies;a>0;--a)reinterpret_cast<CWideString*>(aliasStorage+a-1)->~CWideString();
    CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==2);if(data)CHECK_WRITE(data->owners==1);
   }
   CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
   unsigned live=0;for(unsigned b=0;b<16;++b)if(g_blocks[b]) {++live;CHECK_WRITE(g_blocks[b]==DataOf(g_CAFileCurrentPath_013BCA10));}
   CHECK_WRITE(live==1 && DataOf(g_CAFileCurrentPath_013BCA10)->owners==1);
  }
  modeAllocations[mode]=g_allocated-startAllocations;
 }
 CHECK_WRITE(modeAllocations[0]>0 && modeAllocations[0]*3==modeAllocations[1]*4);
 CheckReadPath(argv[8],argv[9],argv[10],originalCtor,originalReset,sectorBytes);
 CheckNarrowPaths(argv[13]);
 CheckFileQueries();
 CheckPathExists(argv[12]);
 CHECK_WRITE(g_finishCalls==4608 && g_finishCallbacks==2160 && g_finishMutations==1080);
 printf("NATIVE_FINISH_DETAIL calls=%u callbacks=%u mutations=%u\n",g_finishCalls,g_finishCallbacks,g_finishMutations);
 CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(g_originalFinish),0,MEM_RELEASE));
 g_CAFileCurrentPath_013BCA10=CWideString();
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released);
 for(unsigned b=0;b<16;++b)CHECK_WRITE(!g_blocks[b]);
 _wremove(g_realPath);CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalCharacter),0,MEM_RELEASE));
 CHECK_WRITE(cases==1008 && opens==504 && failures==504 && virtualDeletes==144 && heapDeletes==720 && managerCalls==504);
 for(unsigned m=0;m<sizeof(LifetimeManagerStorage);++m)CHECK_WRITE(LifetimeManagerStorage[m]==0x6d);
 CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(originalCtor),0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalDelete),0,MEM_RELEASE));
 CHECK_WRITE(resizeCalls==6048 && invalidTrue==252 && (unalignedFailures==0 || unalignedFailures==504));
 CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(originalResize),0,MEM_RELEASE));
 CHECK_WRITE(countedCases==288 && sharedReleases==192 && g_controlAllocated==432 && g_controlReleased==432 && !g_controlBlock);
 CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(originalReset),0,MEM_RELEASE));
 printf("NATIVE_COUNTED_OWNER_DETAIL counted=%u shared_releases=%u control_allocations=%u control_releases=%u\n",countedCases,sharedReleases,g_controlAllocated,g_controlReleased);
 printf("NATIVE_OPEN_RESIZE_DETAIL calls=%u unaligned_failures=%u invalid_true=%u sector_bytes=%lu\n",resizeCalls,unalignedFailures,invalidTrue,sectorBytes);
 printf("NATIVE_FILE_READ_PATH_PASS cases=%u opens=%u failures=%u virtual_deletes=144 heap_deletes=720 manager_calls=504 path_forms=3 cache_instances=1 char_cases=128 reserve_checks=6 fill_cases=16 data_allocations=%u data_releases=%u\n",cases,opens,failures,g_allocated,g_released);return 0;
}
