// Actual recovered wide-string ownership around real owned write opens.
#include "fable_filesystem.h"
#include "fable_threaded_file.h"
#include "NativeThreadedReadAPC_win32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <new>
__declspec(noreturn) static void FailWrite(const char* expr,int line) {
 fprintf(stderr,"NATIVE_OPEN_WRITE_STRINGS_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK_WRITE(x) do {if(!(x)) FailWrite(#x,__LINE__);}while(0)
static wchar_t g_realPath[260];
static const CWideString* g_expectedName;
static unsigned g_copies,g_converts,g_allocated,g_released;
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
CWideString __fastcall CAFile::ConvertPathToFullPath(const CWideString& path) {
 CHECK_WRITE(&path==g_expectedName && g_CWideStringInstanceCount_013BCA20==int(1+g_copies));++g_converts;return path;
}
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
int main(int argc,char** argv) {
 CHECK_WRITE(argc==3 && mbstowcs(g_realPath,argv[2],259)!=static_cast<size_t>(-1));
 CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==0);
 wchar_t missing[320];wcscpy(missing,g_realPath);wcscat(missing,L".missing-parent\\child.bin");
 OpenWrite original=LoadWrite(argv[1]);OpenWrite modes[2]={CompiledWrite,original};unsigned cases=0,opens=0,failures=0;
 const unsigned copies[3]={0,1,3};
 for(unsigned mode=0;mode<2;++mode) {
  unsigned key=0;
  for(unsigned flags=0;flags<2;++flags)for(unsigned overwrite=0;overwrite<2;++overwrite)for(unsigned state=0;state<4;++state)for(unsigned share=0;share<3;++share) {
   CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==0);PrepareWrite(state==0);g_copies=copies[share];
   {
    CWideString name(state==3?L"":state==2?missing:g_realPath);g_expectedName=&name;
    unsigned long aliasStorage[3];
    for(unsigned a=0;a<g_copies;++a) {
     if(a==1) {CWideString* alias=new(aliasStorage+a) CWideString;*alias=CWideString(L"replace this alias");*alias=name;}
     else new(aliasStorage+a) CWideString(name);
    }
    CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==int(1+g_copies));CWideStringData* data=DataOf(name);
    if(data)CHECK_WRITE(data->owners==int(1+g_copies));
    unsigned char storage[96],expected[96];memset(storage,0xa7,96);storage[32+0x18]=storage[32+0x19]=static_cast<unsigned char>(flags);PutWrite(storage+32,0xc,reinterpret_cast<DWORD>(data));memcpy(expected,storage,96);
    CThreadedFile* file=reinterpret_cast<CThreadedFile*>(storage+32);unsigned converted=g_converts;bool result=modes[mode](file,0,name,overwrite!=0);
    CHECK_WRITE(result==(state<2) && g_converts==converted+1 && g_CWideStringInstanceCount_013BCA20==int(1+g_copies));
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
    CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==1);if(data)CHECK_WRITE(data->owners==1);
   }
   CHECK_WRITE(g_CWideStringInstanceCount_013BCA20==0 && g_allocated==g_released);
   for(unsigned b=0;b<16;++b)CHECK_WRITE(!g_blocks[b]);
  }
 }
 _wremove(g_realPath);CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE));
 CHECK_WRITE(g_converts==96 && g_allocated==104 && g_released==104);
 printf("NATIVE_OPEN_WRITE_STRINGS_PASS cases=%u opens=%u failures=%u data_allocations=%u data_releases=%u\n",cases,opens,failures,g_allocated,g_released);return 0;
}
