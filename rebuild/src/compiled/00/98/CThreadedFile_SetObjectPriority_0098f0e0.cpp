// Retail 0098F0E0: forward to the global manager. The 10-byte body and both
// relocations match the original with the canonical declarations and enum.
#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"

void CThreadedFile::SetObjectPriority(unsigned long uid, EThreadedFilePriority priority)
{
    g_FableThreadedFileManager_013BC870.SetObjectPriority(uid, priority);
}
