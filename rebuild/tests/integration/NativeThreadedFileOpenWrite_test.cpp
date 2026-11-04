// Reuse the established string/path recorders; its legacy main is never called.
#define main LegacyReadOpenMain
#include "../00/98/CThreadedFile_Open_0098e1e0_test.cpp"
#undef main
#include "NativeThreadedReadAPC_win32.h"
#include <stdlib.h>
static void FailWrite(const char* expr,int line) {
 fprintf(stderr,"NATIVE_OPEN_WRITE_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK_WRITE(x) do {if(!(x)) FailWrite(#x,__LINE__);}while(0)
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
 wchar_t missing[320];wcscpy(missing,g_realPath);wcscat(missing,L".missing-parent\\child.bin");
 OpenWrite original=LoadWrite(argv[1]);OpenWrite modes[2]={CompiledWrite,original};unsigned cases=0,opens=0,failures=0;
 for(unsigned mode=0;mode<2;++mode) {
  unsigned key=0;
  for(unsigned flags=0;flags<2;++flags)for(unsigned overwrite=0;overwrite<2;++overwrite)for(unsigned state=0;state<3;++state) {
   PrepareWrite(state==0);CWideString name(L"Data\\owned-write.big");Reset(state==2?missing:g_realPath,0);g_expectedName=&name;
   unsigned char storage[96],expected[96];memset(storage,0xa7,96);storage[32+0x18]=storage[32+0x19]=static_cast<unsigned char>(flags);memcpy(expected,storage,96);
   CThreadedFile* file=reinterpret_cast<CThreadedFile*>(storage+32);bool result=modes[mode](file,0,name,overwrite!=0);
   CHECK_WRITE(result==(state!=2) && g_argumentsValid && g_wideDestructs==1 && g_charDestructs==0 && g_assigns==0 && g_upperCalls==0);
   PutWrite(expected+32,4,0);PutWrite(expected+32,8,reinterpret_cast<DWORD>(file->fileHandle_));
   if(result) {
    unsigned size=state==0 && !overwrite?1234:0;
    CHECK_WRITE(file->fileHandle_!=INVALID_HANDLE_VALUE && GetFileSize(file->fileHandle_,0)==size);
    PutWrite(expected+32,0x10,size);PutWrite(expected+32,0x14,++key);expected[32+0x18]=expected[32+0x19]=1;
    CHECK_WRITE(CloseHandle(file->fileHandle_));FILE* f=_wfopen(g_realPath,L"rb");CHECK_WRITE(f);
    for(unsigned i=0;i<size;++i)CHECK_WRITE(fgetc(f)==int((i*37+23)&255));CHECK_WRITE(fgetc(f)==EOF && fclose(f)==0);++opens;
   } else {CHECK_WRITE(file->fileHandle_==INVALID_HANDLE_VALUE);++failures;}
   CHECK_WRITE(memcmp(storage,expected,96)==0);++cases;
  }
 }
 _wremove(g_realPath);CHECK_WRITE(VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE));
 printf("NATIVE_OPEN_WRITE_IO_PASS cases=%u opens=%u failures=%u original_rebuilt=PASS\n",cases,opens,failures);return 0;
}
