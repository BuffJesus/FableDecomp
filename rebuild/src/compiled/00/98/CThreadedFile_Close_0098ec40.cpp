#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"

extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);

void CThreadedFile::Close()
{
    if (fileHandle_ != reinterpret_cast<void*>(-1))
    {
        g_FableThreadedFileManager_013BC870.CloseAllStreamsForFile(this);
        CloseHandle(fileHandle_);
        fileHandle_ = reinterpret_cast<void*>(-1);
        openFlag_ = false;
        openedForWrite_ = false;
    }
}

CThreadedFile::~CThreadedFile()
{
    Close();
}
