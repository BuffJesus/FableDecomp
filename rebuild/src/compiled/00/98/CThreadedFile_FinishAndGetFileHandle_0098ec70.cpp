// CThreadedFile::FinishAndGetFileHandle - 0x0098EC70, thiscall, zero explicit args.
// Exact25B verified root; CloseAllStreamsForFile remains a child boundary.
#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"

void* CThreadedFile::FinishAndGetFileHandle()
{
    if (fileHandle_ != reinterpret_cast<void*>(-1))
    {
        g_FableThreadedFileManager_013BC870.CloseAllStreamsForFile(this);
    }
    return fileHandle_;
}
