// Only the gate-created private file is opened for writing.
#include "fable_threaded_file.h"
#include "NativeThreadedReadAPC_win32.h"
#include <stdio.h>
#include <string.h>
extern "C" {
__declspec(dllimport) DWORD __stdcall SetFilePointer(HANDLE,LONG,LONG*,DWORD);
__declspec(dllimport) BOOL __stdcall SetEndOfFile(HANDLE);
__declspec(dllimport) DWORD __stdcall GetFileSize(HANDLE,DWORD*);
__declspec(dllimport) BOOL __stdcall ReadFile(HANDLE,void*,DWORD,DWORD*,OVERLAPPED*);
__declspec(dllimport) BOOL __stdcall WriteFile(HANDLE,const void*,DWORD,DWORD*,OVERLAPPED*);
}
static void Fail(const char* expr,int line) {
 fprintf(stderr,"NATIVE_RESIZE_FAILURE line=%d expr=%s error=%lu\n",line,expr,GetLastError());fflush(stderr);ExitProcess(80);
}
#define CHECK(x) do {if(!(x)) Fail(#x,__LINE__);}while(0)
typedef bool (__fastcall* Resize)(CThreadedFile*,void*,DWORD);
static bool __fastcall Compiled(CThreadedFile* file,void*,DWORD size) {return file->ResizeFile(size);}
static DWORD (__stdcall* pPointer)(HANDLE,LONG,LONG*,DWORD)=SetFilePointer;
static BOOL (__stdcall* pEnd)(HANDLE)=SetEndOfFile;
static DWORD (__stdcall* pSize)(HANDLE,DWORD*)=GetFileSize;
static Resize Load(const char* path) {
 unsigned char original[58];FILE* f=fopen(path,"rb");CHECK(f);CHECK(fread(original,1,58,f)==58 && fgetc(f)==EOF);CHECK(fclose(f)==0);
 unsigned char* code=static_cast<unsigned char*>(VirtualAlloc(0,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));CHECK(code);memcpy(code,original,58);
 const DWORD sites[3]={19,29,41},retail[3]={0x143fde8,0x143fdec,0x143fdf0};
 const DWORD targets[3]={reinterpret_cast<DWORD>(&pPointer),reinterpret_cast<DWORD>(&pEnd),reinterpret_cast<DWORD>(&pSize)};
 for(unsigned i=0;i<3;++i) {DWORD value;memcpy(&value,original+sites[i],4);CHECK(value==retail[i]);memcpy(code+sites[i],targets+i,4);}
 DWORD old;CHECK(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&old));CHECK(FlushInstructionCache(GetCurrentProcess(),code,58));return reinterpret_cast<Resize>(code);
}
static void Run(Resize resize,HANDLE handle,DWORD length,bool expected) {
 unsigned char storage[96],wanted[96];memset(storage,0xa7,96);memcpy(storage+32+8,&handle,4);memcpy(wanted,storage,96);
 CThreadedFile* file=reinterpret_cast<CThreadedFile*>(storage+32);
 CHECK(resize(file,0,length)==expected);
 DWORD actual=GetFileSize(handle,0);memcpy(wanted+32+0x10,&actual,4);
 CHECK(memcmp(storage,wanted,96)==0);
}
int main(int argc,char** argv) {
 CHECK(argc==3);Resize original=Load(argv[1]);Resize modes[2]={Compiled,original};
 HANDLE file=CreateFileA(argv[2],0xc0000000UL,0,0,OPEN_EXISTING,0,0);CHECK(file!=INVALID_HANDLE_VALUE);
 unsigned char payload[4096],readback[4096];for(unsigned i=0;i<4096;++i)payload[i]=static_cast<unsigned char>(i*37+23);
 const DWORD sizes[6]={0,1,17,4096,8192,31};unsigned cases=0;
 for(unsigned mode=0;mode<2;++mode) {
  for(unsigned j=0;j<6;++j) {
   CHECK(SetFilePointer(file,0,0,0)==0 && SetEndOfFile(file));DWORD count=0;
   CHECK(WriteFile(file,payload,4096,&count,0) && count==4096);
   Run(modes[mode],file,sizes[j],true);
   CHECK(GetFileSize(file,0)==sizes[j] && SetFilePointer(file,0,0,1)==sizes[j]);
   CHECK(SetFilePointer(file,0,0,0)==0);DWORD prefix=sizes[j]<4096?sizes[j]:4096;
   CHECK(ReadFile(file,readback,prefix,&count,0) && count==prefix && memcmp(readback,payload,prefix)==0);++cases;
  }
  for(unsigned h=0;h<2;++h)for(unsigned n=0;n<2;++n) {
   Run(modes[mode],h?INVALID_HANDLE_VALUE:0,n?0xffffffff:0,n!=0);++cases;
  }
 }
 CHECK(CloseHandle(file));CHECK(VirtualFree(reinterpret_cast<void*>(original),0,MEM_RELEASE));
 printf("NATIVE_RESIZE_IO_PASS cases=%u resizes=12 invalid_handles=8 original_rebuilt=PASS\n",cases);return 0;
}
