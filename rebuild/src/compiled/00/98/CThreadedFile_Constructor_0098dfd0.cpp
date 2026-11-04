#include "fable_threaded_file.h"

// CThreadedFile::CThreadedFile() @ 0x0098DFD0.
CThreadedFile::CThreadedFile()
{
    openedForWrite_ = false;
    openFlag_ = false;
    fileHandle_ = reinterpret_cast<void*>(-1);
}
