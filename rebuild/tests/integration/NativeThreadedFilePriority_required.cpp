// Never-run link caller; actual machine caller can be independently emulated.
#include "fable_threaded_file.h"
int main(int argc, char** argv)
{
    CThreadedFile* file = reinterpret_cast<CThreadedFile*>(argv);
    file->SetObjectPriority(static_cast<unsigned long>(argc),
                            static_cast<EThreadedFilePriority>(argc));
    return 0;
}
