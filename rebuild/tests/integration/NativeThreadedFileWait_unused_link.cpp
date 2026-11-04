// Never-run typed link boundaries only: no production singleton/layout claim.
#include "fable_gfuninitialise.h"
#include <stdlib.h>
CThreadedFileManager g_FableThreadedFileManager_013BC870;
void CThreadedFileManager::WaitForObjectToFinish(unsigned long)
{
    abort();
}
