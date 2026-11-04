// Never-run typed boundaries ONLY; no physical production singleton claim.
#include "fable_gfuninitialise.h"
#include <stdlib.h>
CThreadedFileManager g_FableThreadedFileManager_013BC870;
void CThreadedFileManager::SetObjectPriority(unsigned long, EThreadedFilePriority)
{
    abort();
}
