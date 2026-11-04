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
__declspec(noreturn) static void FailWrite(const char* expr,int line) {
 fprintf(stderr,"NATIVE_COUNTED_FILE_OWNER_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK_WRITE(x) do {if(!(x)) FailWrite(#x,__LINE__);}while(0)
static wchar_t g_realPath[260];
static const CWideString* g_expectedName;
static unsigned g_copies,g_allocated,g_released;
static CWideStringData* g_blocks[16];
static unsigned char* g_fileAllocation;
static CThreadedFile* g_fileObject;
static unsigned char g_fileSnapshot[96];
static unsigned g_fileFreed,g_managerCloses;
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
}
static CWideStringData* DataOf(const CWideString& value) {
 CWideStringData* data;CHECK_WRITE(sizeof(value)==sizeof(data));memcpy(&data,&value,sizeof(data));return data;
}
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
void* __cdecl operator new(size_t size) {
 if(size==sizeof(CCPPointerInfo)) {
  CHECK_WRITE(!g_controlAllocation && !g_controlBlock);
  g_controlAllocation=static_cast<unsigned char*>(malloc(44));CHECK_WRITE(g_controlAllocation);memset(g_controlAllocation,0x6b,44);
  g_controlBlock=reinterpret_cast<CCPPointerInfo*>(g_controlAllocation+16);++g_controlAllocated;return g_controlBlock;
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
 unsigned i=0;while(i<16 && g_blocks[i]!=p)++i;CHECK_WRITE(i<16 && g_blocks[i]->owners<=0);g_blocks[i]=0;++g_released;free(p);
}
CWideString CWideString::GetSubString(unsigned long,unsigned long) const {FailWrite("cold dot-path substring",__LINE__);}
CWideString __fastcall CAFile::GetAbsoluteFilePath(const CWideString&,const CWideString&) {FailWrite("cold parent-path normalization",__LINE__);}
void __fastcall CAFile::SplitPath(const CWideString&,CWideString*,CWideString*,CWideString*,CWideString*) {FailWrite("cold read-open SplitPath",__LINE__);}
CCharString CWideString::ToCharString() const {FailWrite("cold read-open ToCharString",__LINE__);}
CCharString CCharString::GetAsUpperCase() const {FailWrite("cold read-open uppercase",__LINE__);}
CCharString::~CCharString() {FailWrite("cold read-open char destructor",__LINE__);}
template<> bool CBasicString<char>::operator==(const char*) const {FailWrite("cold read-open char comparison",__LINE__);}
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
int main(int argc,char** argv) {
 CHECK_WRITE(argc==8 && mbstowcs(g_realPath,argv[2],259)!=static_cast<size_t>(-1));
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1);
 wchar_t working[260];CHECK_WRITE(_wgetcwd(working,260)==working);
 wchar_t directory[264];wcscpy(directory,working);wcscat(directory,L"\\");
 {CWideString initial=CAFile::GetCurrentPath();CHECK_WRITE(wcscmp(initial,working)==0);}
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
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
 g_CAFileCurrentPath_013BCA10=CWideString();
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released);
 for(unsigned b=0;b<16;++b)CHECK_WRITE(!g_blocks[b]);
 _wremove(g_realPath);CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalCharacter),0,MEM_RELEASE));
 CHECK_WRITE(cases==1008 && opens==504 && failures==504 && virtualDeletes==144 && heapDeletes==720 && managerCalls==504);
 for(unsigned m=0;m<sizeof(LifetimeManagerStorage);++m)CHECK_WRITE(LifetimeManagerStorage[m]==0x6d);
 CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(originalCtor),0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalDelete),0,MEM_RELEASE));
 CHECK_WRITE(resizeCalls==6048 && invalidTrue==252 && (unalignedFailures==0 || unalignedFailures==504));
 CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(originalResize),0,MEM_RELEASE));
 CHECK_WRITE(countedCases==288 && sharedReleases==192 && g_controlAllocated==288 && g_controlReleased==288 && !g_controlBlock);
 CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(originalReset),0,MEM_RELEASE));
 printf("NATIVE_COUNTED_OWNER_DETAIL counted=%u shared_releases=%u control_allocations=%u control_releases=%u\n",countedCases,sharedReleases,g_controlAllocated,g_controlReleased);
 printf("NATIVE_OPEN_RESIZE_DETAIL calls=%u unaligned_failures=%u invalid_true=%u sector_bytes=%lu\n",resizeCalls,unalignedFailures,invalidTrue,sectorBytes);
 printf("NATIVE_COUNTED_FILE_OWNER_PASS cases=%u opens=%u failures=%u virtual_deletes=144 heap_deletes=720 manager_calls=504 path_forms=3 cache_instances=1 char_cases=128 reserve_checks=6 fill_cases=16 data_allocations=%u data_releases=%u\n",cases,opens,failures,g_allocated,g_released);return 0;
}
