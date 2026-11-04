#include "fable_threaded_file.h"
int main(int argc, char** argv)
{
    CThreadedFile* file = reinterpret_cast<CThreadedFile*>(argv);
    return file->RemoveObject(static_cast<unsigned long>(argc));
}
