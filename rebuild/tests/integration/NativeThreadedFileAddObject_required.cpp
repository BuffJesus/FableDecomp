#include "fable_threaded_file.h"

int main(int argc, char** argv)
{
    CThreadedFile* file = reinterpret_cast<CThreadedFile*>(argv);
    const CThreadedFileObjectDesc& desc = *reinterpret_cast<const CThreadedFileObjectDesc*>(argc);
    return static_cast<int>(file->AddObject(desc));
}
