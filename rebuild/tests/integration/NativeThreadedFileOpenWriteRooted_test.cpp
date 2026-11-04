// Recovered absolute/plain-relative resolution and cache around owned write opens.
#include "fable_filesystem.h"
#include "fable_threaded_file.h"
#include "NativeThreadedReadAPC_win32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <new>
#include <direct.h>
__declspec(noreturn) static void FailWrite(const char* expr,int line) {
 fprintf(stderr,"NATIVE_OPEN_WRITE_ROOTED_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK_WRITE(x) do {if(!(x)) FailWrite(#x,__LINE__);}while(0)
static wchar_t g_realPath[260];
static const CWideString* g_expectedName;
static unsigned g_copies,g_allocated,g_released;
static CWideStringData* g_blocks[16];
static CWideStringData* DataOf(const CWideString& value) {
 CWideStringData* data;CHECK_WRITE(sizeof(value)==sizeof(data));memcpy(&data,&value,sizeof(data));return data;
}
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
void* __cdecl operator new(size_t size) {
 CHECK_WRITE(size==sizeof(CWideStringData));void* p=malloc(size);CHECK_WRITE(p);
 unsigned i=0;while(i<16 && g_blocks[i])++i;CHECK_WRITE(i<16);g_blocks[i]=static_cast<CWideStringData*>(p);++g_allocated;return p;
}
void __cdecl operator delete(void* p) {
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

int main(int argc,char** argv) {
 CHECK_WRITE(argc==4 && mbstowcs(g_realPath,argv[2],259)!=static_cast<size_t>(-1));
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1);
 wchar_t working[260];CHECK_WRITE(_wgetcwd(working,260)==working);
 wchar_t directory[264];wcscpy(directory,working);wcscat(directory,L"\\");
 {CWideString initial=CAFile::GetCurrentPath();CHECK_WRITE(wcscmp(initial,working)==0);}
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
 CharacterAppend originalCharacter=LoadCharacterRooted(argv[3]);CheckCharacterRooted(originalCharacter);CheckReserveRooted();CheckFillRooted();
 wchar_t missing[320];wcscpy(missing,g_realPath);wcscat(missing,L".missing-parent\\child.bin");
 OpenWrite original=LoadWrite(argv[1]);OpenWrite modes[2]={CompiledWrite,original};unsigned cases=0,opens=0,failures=0;
 const unsigned copies[3]={0,1,3};unsigned modeAllocations[2];
 const wchar_t* basename=wcsrchr(g_realPath,L'\\');CHECK_WRITE(basename);++basename;
 wchar_t relativeMissing[320];wcscpy(relativeMissing,basename);wcscat(relativeMissing,L".missing-parent\\child.bin");
 for(unsigned mode=0;mode<2;++mode) {
  unsigned key=0,startAllocations=g_allocated;
  for(unsigned flags=0;flags<2;++flags)for(unsigned overwrite=0;overwrite<2;++overwrite)for(unsigned state=0;state<4;++state)for(unsigned share=0;share<3;++share)for(unsigned form=0;form<3;++form) {
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
    unsigned char storage[96],expected[96];memset(storage,0xa7,96);storage[32+0x18]=storage[32+0x19]=static_cast<unsigned char>(flags);PutWrite(storage+32,0xc,reinterpret_cast<DWORD>(data));memcpy(expected,storage,96);
    {
     CWideString resolved=CAFile::ConvertPathToFullPath(name);
     CHECK_WRITE(wcscmp(resolved,state==3?directory:state==2?missing:g_realPath)==0);
    }
    CThreadedFile* file=reinterpret_cast<CThreadedFile*>(storage+32);bool result=modes[mode](file,0,name,overwrite!=0);
    CHECK_WRITE(result==(state<2) && g_CWideStringInstanceCount_013BCA20==int(2+g_copies));
    CHECK_WRITE(DataOf(name)==data);if(data)CHECK_WRITE(data->owners==int(1+g_copies));
    PutWrite(expected+32,4,0);PutWrite(expected+32,8,reinterpret_cast<DWORD>(file->fileHandle_));
    if(result) {
     unsigned size=state==0 && !overwrite?1234:0;CHECK_WRITE(file->fileHandle_!=INVALID_HANDLE_VALUE && GetFileSize(file->fileHandle_,0)==size);
     PutWrite(expected+32,0x10,size);PutWrite(expected+32,0x14,++key);expected[32+0x18]=expected[32+0x19]=1;
     CHECK_WRITE(CloseHandle(file->fileHandle_));FILE* f=_wfopen(g_realPath,L"rb");CHECK_WRITE(f);
     for(unsigned i=0;i<size;++i)CHECK_WRITE(fgetc(f)==int((i*37+23)&255));CHECK_WRITE(fgetc(f)==EOF && fclose(f)==0);++opens;
    } else {CHECK_WRITE(file->fileHandle_==INVALID_HANDLE_VALUE);++failures;}
    CHECK_WRITE(memcmp(storage,expected,96)==0);++cases;
    for(unsigned a=g_copies;a>0;--a)reinterpret_cast<CWideString*>(aliasStorage+a-1)->~CWideString();
    CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==2);if(data)CHECK_WRITE(data->owners==1);
   }
   CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released+1);
   unsigned live=0;for(unsigned b=0;b<16;++b)if(g_blocks[b]) {++live;CHECK_WRITE(g_blocks[b]==DataOf(g_CAFileCurrentPath_013BCA10));}
   CHECK_WRITE(live==1 && DataOf(g_CAFileCurrentPath_013BCA10)->owners==1);
  }
  modeAllocations[mode]=g_allocated-startAllocations;
 }
 CHECK_WRITE(modeAllocations[0]>0 && modeAllocations[0]==modeAllocations[1]);
 g_CAFileCurrentPath_013BCA10=CWideString();
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1 && g_allocated==g_released);
 for(unsigned b=0;b<16;++b)CHECK_WRITE(!g_blocks[b]);
 _wremove(g_realPath);CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE) && VirtualFree(reinterpret_cast<void*>(originalCharacter),0,MEM_RELEASE));
 CHECK_WRITE(cases==288 && opens==144 && failures==144);
 printf("NATIVE_OPEN_WRITE_ROOTED_PASS cases=%u opens=%u failures=%u path_forms=3 cache_instances=1 char_cases=128 reserve_checks=6 fill_cases=16 data_allocations=%u data_releases=%u\n",cases,opens,failures,g_allocated,g_released);return 0;
}
