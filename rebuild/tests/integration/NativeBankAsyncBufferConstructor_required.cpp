#include <stdlib.h>
#include <new>
#include "fable_bank_async_entry.h"
// Existing member of already-declared friend; no new access grant.
// This is linkage-only. Never execute this executable.
void CBankFileAsyncEntry::ReadDataNonAsync(unsigned long offset, void* data, unsigned long size)
{
    new (data) CBankFileAsyncData(data, size, BankFile);
}
extern "C" void UnusedConstructorBoundary() { abort(); }
int main(int argc,char**argv)
{
    reinterpret_cast<CBankFileAsyncEntry*>(argv)->ReadDataNonAsync(argc,argv,argc);
    return 0;
}
#pragma comment(linker,"/alternatename:__purecall=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?CanRelocateData@CMovableResource@@UBE_NJ@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?Verify@CMovableResource@@UBEKXZ=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:??0CBaseClassNonCopyable@@QAE@XZ=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?OnFileReadBegin@CThreadedFileCallback@@UAE_NPAPAXW4EThreadedFilePriority@@@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?OnFileReadCompletion@CThreadedFileCallback@@UAEX_N@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:??1CBaseClassNonCopyable@@UAE@XZ=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?insert_unique@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@_N@2@ABQAVCBankFileAsyncData@@@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:??1CCriticalSectionLock@@QAE@XZ=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?RelocateData@CBankFileAsyncData@@UAEXJPAX0J@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?CanRelocateData@CBankFileAsyncData@@UBE_NJ@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?OnFileReadBegin@CBankFileAsyncData@@UAE_NPAPAXW4EThreadedFilePriority@@@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:?OnFileReadCompletion@CBankFileAsyncData@@UAEX_N@Z=_UnusedConstructorBoundary")
#pragma comment(linker,"/alternatename:??1CBankFileAsyncData@@UAE@XZ=_UnusedConstructorBoundary")
