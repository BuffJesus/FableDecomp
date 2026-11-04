// Genuine CThreadedFile::WaitForObjectToFinish(unsigned long), retail0098F370.
#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"
void CThreadedFile::WaitForObjectToFinish(unsigned long uid)
{
    g_FableThreadedFileManager_013BC870.WaitForObjectToFinish(uid);
}
