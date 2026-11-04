#include <stdlib.h>
#include "fable_bank_async_entry.h"
extern "C" void UnusedAutoAllocBoundary(){abort();}
int main(int argc,char**argv){((CBankFileAsyncEntry*)argv)->BeginReadDataAutoAlloc(argc,0xffffffff,(EThreadedFilePriority)0);return 0;}
#pragma comment(linker,"/alternatename:?Align@CBankFile@@QBEKK@Z=_UnusedAutoAllocBoundary")
#pragma comment(linker,"/alternatename:??0CBankFileAsyncData@@AAE@KKPAVCMovableResourceMemoryPool@@AAVCBankFileAsync@@@Z=_UnusedAutoAllocBoundary")
#pragma comment(linker,"/alternatename:?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z=_UnusedAutoAllocBoundary")
