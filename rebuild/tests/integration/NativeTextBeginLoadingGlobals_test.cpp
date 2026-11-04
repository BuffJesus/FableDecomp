#include <stdlib.h>
#include "fable_gfuninitialise.h"
#include "fable_file_installer.h"
CThreadedFileManager g_FableThreadedFileManager_013BC870;
CCountedFileInstaller g_CFileInstallerSingleton_013CA818={0,0};
extern "C" void ColdBoundary();
void* __cdecl FableFileInstallerAllocate(fable_u32){ColdBoundary();return 0;}
CFileInstaller::CFileInstaller(){ColdBoundary();}
CFileInstaller::~CFileInstaller(){ColdBoundary();}
void CCountedFileInstaller::operator=(CFileInstaller*){ColdBoundary();}
extern "C" void* __cdecl FixtureBankInitMalloc(size_t n){return malloc(n);}
extern "C" void __cdecl FixtureBankInitFree(void* p){free(p);}
extern "C" void* __cdecl FixtureBankTreeMalloc(size_t n){return malloc(n);}
extern "C" void __cdecl FixtureBankTreeFree(void* p){free(p);}
#pragma comment(linker,"/alternatename:__imp__FixtureDeleteCriticalSection@4=__imp__DeleteCriticalSection@4")
fable_i32 g_CWideStringInstanceCount_013BCA20=0;
fable_i32 g_CCharStringInstanceCount_013BD800=0;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
char g_FableEmptyNarrowString_0129AAF4[1]={0};

#include "fable_progress_display.h"
namespace NProgressDisplay { CCountedPointer<CAProgressDisplay> PProgressDisplay; }

class CMemoryAllocator;


unsigned int g_CBankFileAsyncFailureHandlingMode_013CA868=0;
// Caller-owned buffers and null pools must not enter allocation paths.
#pragma comment(linker,"/alternatename:?EnterSynchronisation@CMemoryAllocator@@KIXXZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?LeaveSynchronisation@CMemoryAllocator@@KIXXZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?Create@CMemoryAllocator@@KIXXZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?AllocateResource@CMovableResourceMemoryPool@@QAE_NPAJPAVCMovableResource@@KK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?CanRelocateData@CMovableResource@@UBE_NJ@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??3CMemoryAllocator@@KIXPAX@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??1CMemoryAllocator@@IAE@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetNoAllocations@CMemoryAllocator@@QAEKXZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z=_ColdBoundary")
