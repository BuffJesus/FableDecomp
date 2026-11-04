#include <stdlib.h>
#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"

// Never-run link-only typed boundaries; no production storage/layout credit.
CThreadedFileManager g_FableThreadedFileManager_013BC870;
unsigned long CThreadedFileManager::AddObject(const CThreadedFileObjectDesc&, CThreadedFile&)
{
    abort();
    return 0;
}
