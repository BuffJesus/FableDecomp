#include "fable_threaded_file.h"
int main(int argc, char** argv)
{
    reinterpret_cast<CThreadedFile*>(argv)->CThreadedFile::WaitForObjectToFinish(static_cast<unsigned long>(argc));
    return 0;
}
