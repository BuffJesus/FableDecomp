typedef unsigned long DWORD;
typedef void* HANDLE;
enum {FALSE=0,WAIT_OBJECT_0=0,ERROR_INVALID_HANDLE=6};
extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
__declspec(dllimport) HANDLE __stdcall CreateEventA(void*,int,int,const char*);
__declspec(dllimport) HANDLE __stdcall CreateThread(void*,unsigned long,DWORD(__stdcall*)(void*),void*,unsigned long,unsigned long*);
__declspec(dllimport) DWORD __stdcall WaitForSingleObject(HANDLE,DWORD);
__declspec(dllimport) int __stdcall CloseHandle(HANDLE);
__declspec(dllimport) int __stdcall GetHandleInformation(HANDLE,DWORD*);
__declspec(dllimport) DWORD __stdcall GetLastError();
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_file_installer_active_files.h"
#include "fable_retail_banks.h"
static void require(bool ok){if(!ok)abort();}
static unsigned live,baseCalls,destroyed;
static void* __cdecl Allocate(void*,unsigned count,unsigned width){void* p=calloc(count,width);require(p!=0);++live;return p;}
static void __cdecl Release(void*,void* p){if(p){require(live!=0);--live;free(p);}}
void CCompressorZlib::Compress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CCompressorZlib::Decompress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CCompressorZlib::CompressAsImage(const void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){abort();}
void CACompressor::Compress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CACompressor::Decompress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CACompressor::CompressAsImage(void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){abort();}
void CACompressor::SetQuality(float){abort();}
extern "C" void __fastcall BDtor(void*){++baseCalls;}
char g_FableEmptyNarrowString_0129AAF4[1]={0};
extern "C" void* __cdecl AllocateInstallerBuffer(unsigned long);
extern "C" void __cdecl CheckInstallerAllocatorEmpty();
extern "C" __declspec(dllimport) void* __stdcall GlobalAlloc(unsigned,unsigned long);


extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
void CFileInstaller::ProcessActiveFileForDecompression(CActiveFile&){abort();}
void CFileInstaller::PostFileReadCompletion(CActiveRead*){abort();}
void CFileInstaller::ProcessActiveFileForWrite(CActiveFile&){abort();}
void CFileInstaller::ProcessReads(){abort();}
void CFileInstaller::ProcessAlways(){abort();}
int main(){
 for(unsigned cycle=0;cycle!=64;++cycle){
  unsigned long storage[sizeof(CFileInstaller)/4];memset(storage,0,sizeof(storage));
  unsigned char* bytes=(unsigned char*)storage;long baseline=g_CCharStringInstanceCount_013BD800;
  ::new((void*)storage)CFileInstaller;
  HANDLE events[4];for(unsigned i=0;i!=4;++i){events[i]=*(HANDLE*)(bytes+0x84+i*4);require(events[i]!=0);}
  // Let actual CRT workers enter their genuine idle waits on alternate cycles;
  // the immediate-destruction cycles exercise startup/shutdown races too.
  if(cycle&1)Sleep(10);
  typedef void*(__fastcall* ScalarDelete)(void*,void*,unsigned);
  ScalarDelete destroy=*(ScalarDelete*)*(void**)bytes;
  require(destroy(bytes,0,0)==bytes);
  require(bytes[0x78]&&bytes[0x79]&&bytes[0x7a]&&!bytes[0x7b]);
  require(g_CCharStringInstanceCount_013BD800==baseline);
  for(i=0;i!=4;++i){DWORD flags;require(!GetHandleInformation(events[i],&flags)&&GetLastError()==ERROR_INVALID_HANDLE);}
 }

 puts("INSTALLER_NATIVE_WORKERS_OK cases=64 workers=128 events=256");return 0;
}

