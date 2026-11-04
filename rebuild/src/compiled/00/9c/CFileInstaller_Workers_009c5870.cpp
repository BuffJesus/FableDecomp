#include "fable_file_installer.h"

extern "C" __declspec(dllimport) void* __stdcall GetCurrentThread();
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void*, int);

// Retail static cdecl entries passed to _beginthread by the constructor.
void FABLE_CDECL FableInstallerWorker_009C5870(void* object)
{
    SetThreadPriority(GetCurrentThread(), 1);
    static_cast<CFileInstaller*>(object)->ThreadMain();
}

void FABLE_CDECL FableInstallerDecompressionWorker_009C4420(void* object)
{
    SetThreadPriority(GetCurrentThread(), 0);
    static_cast<CFileInstaller*>(object)->DecompressionThreadMain();
}
