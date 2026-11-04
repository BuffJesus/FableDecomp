#include "fable_file_installer.h"
#include "fable_file_installer_active_files.h"
#include "fable_retail_banks.h"
#include "fable_critical_section_lock.h"
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
template<class List> static __forceinline void DestroyList(List& list){list.Clear();if(list.head)free(list.head);}
static __forceinline void DestroyCompleted(InstallerCompletedFileSet& set){
 if(set.size){set.Erase(set.head->parent);set.head->left=set.head;set.head->parent=0;set.head->right=set.head;set.size=0;}
 if(set.head)free(set.head);
}
CFileInstaller::~CFileInstaller(){
 unsigned char* bytes=reinterpret_cast<unsigned char*>(this);
 void** handles=reinterpret_cast<void**>(bytes+0x84);
 {
  CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bytes+4),false);
  ClearBuffers();bytes[0x7a]=1;
  typedef int(__stdcall* Event)(void*);Event signal=SetEvent;
  signal(handles[0]);signal(handles[1]);
  typedef void(__stdcall* Yield)(unsigned long);Yield yield=Sleep;
  while(!bytes[0x78]||!bytes[0x79]){lock.Release();yield(0);lock.Lock();}
  Event close=CloseHandle;close(handles[0]);close(handles[1]);close(handles[2]);close(handles[3]);
 }
 DeleteCriticalSection(bytes+4);
 CBasicRetailBankAliasMap& aliases=*reinterpret_cast<CBasicRetailBankAliasMap*>(bytes+0x94);
 aliases.clear();if(aliases.head_)free(aliases.head_);
 DestroyCompleted(*reinterpret_cast<InstallerCompletedFileSet*>(bytes+0x44));
 DestroyList(*reinterpret_cast<InstallerActiveFileList*>(bytes+0x40));
 DestroyList(*reinterpret_cast<InstallerCountedFileList*>(bytes+0x3c));
 DestroyList(*reinterpret_cast<InstallerCountedFileList*>(bytes+0x38));
 CFileInstallerBufferArray& buffers=*reinterpret_cast<CFileInstallerBufferArray*>(bytes+0x1c);
 unsigned char allocator;FableInstallerDestroyBufferRange(buffers.first,buffers.last,&allocator);
 if(buffers.first)free(buffers.first);
}
