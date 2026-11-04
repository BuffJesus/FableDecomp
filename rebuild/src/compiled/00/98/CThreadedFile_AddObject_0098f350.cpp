#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"

unsigned long CThreadedFile::AddObject(const CThreadedFileObjectDesc& desc)
{
    return g_FableThreadedFileManager_013BC870.AddObject(desc, *this);
}
