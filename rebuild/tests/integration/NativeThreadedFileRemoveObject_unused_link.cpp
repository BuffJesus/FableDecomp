// Never-run linkage boundary only: neither manager implementation nor production
// singleton storage/layout is recovered by this fixture.
#include <stdlib.h>
#include "fable_gfuninitialise.h"

CThreadedFileManager g_FableThreadedFileManager_013BC870;
bool CThreadedFileManager::RemoveObject(unsigned long)
{
    abort();
    return false;
}
