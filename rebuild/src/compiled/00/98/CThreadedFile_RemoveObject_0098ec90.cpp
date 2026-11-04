#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"

// Existing real singleton mapping; no duplicate static/global storage.
bool CThreadedFile::RemoveObject(unsigned long uid)
{
    return g_FableThreadedFileManager_013BC870.RemoveObject(uid);
}
