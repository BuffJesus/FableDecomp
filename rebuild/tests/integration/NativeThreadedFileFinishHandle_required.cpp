#include "fable_threaded_file.h"
int main(int, char** argv)
{
    CThreadedFile* file = reinterpret_cast<CThreadedFile*>(argv);
    return file->FinishAndGetFileHandle() != 0;
}
