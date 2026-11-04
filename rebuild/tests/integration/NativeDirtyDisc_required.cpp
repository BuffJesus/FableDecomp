#include "fable_gfuninitialise.h"

void RequireDirtyDisc()
{
    CThreadedFileManager::ProcessDirtyDiscError();
}
int main() { RequireDirtyDisc(); return 0; }
