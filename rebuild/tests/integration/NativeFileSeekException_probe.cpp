#include "fable_disk_file.h"
#include "fable_file_exception.h"
#include <stdio.h>
#include <string.h>

extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);

static void Check(bool condition, int line)
{
    if (!condition)
    {
        fprintf(stderr, "FILE_SEEK_EXCEPTION_FAILURE line=%d\n", line);
        fflush(stderr);
        ExitProcess(80);
    }
}
#define VERIFY(condition) Check((condition) != 0, __LINE__)

// Runs in a separate process: the original throw sites retain their outer
// CCharString temporary. Do not hide that behavior by adding EH to providers.
extern "C" void ProbeNativeFileSeekExceptions(CDiskFileWin32* source, const char* archive)
{
    VERIFY(source && source->IsOpen());
    long sourcePosition = source->GetPosition();
    CAFile* file = source->Clone();
    VERIFY(file && file->GetPosition() == 0);
    file->MovePosition(4);
    for (unsigned mode = 0; mode < 2; ++mode)
    {
        long beforePosition = file->GetPosition();
        long beforeStrings = g_CCharStringInstanceCount_013BD800;
        bool caught = false;
        try
        {
            file->MovePosition(mode ? file->GetLength() : -beforePosition - 1);
        }
        catch (const CExceptionBase& error)
        {
            caught = true;
            const CBBBFileException& typed = static_cast<const CBBBFileException&>(error);
            VERIFY(typed.Type == CBBBFileException::BAD_SEEK);
        }
        catch (...)
        {
            fprintf(stderr, "FILE_SEEK_EXCEPTION_FAILURE base_not_caught\n");
            fflush(stderr);
            ExitProcess(80);
        }
        VERIFY(caught && file->GetPosition() == beforePosition);
        VERIFY(source->GetPosition() == sourcePosition);
        VERIFY(g_CCharStringInstanceCount_013BD800 == beforeStrings + 1);
        unsigned char actual[4], expected[4];
        file->Read(actual, sizeof(actual), 0);
        FILE* oracle = fopen(archive, "rb");
        VERIFY(oracle && !fseek(oracle, beforePosition, SEEK_SET));
        VERIFY(fread(expected, 1, sizeof(expected), oracle) == sizeof(expected));
        VERIFY(!fclose(oracle) && !memcmp(actual, expected, sizeof(actual)));
        VERIFY(file->GetPosition() == beforePosition + 4);
    }
    printf("FILE_SEEK_EXCEPTION_PASS throws=2 readable_after=2 retained_strings=2 source_unchanged=1 cleanup_unclaimed=1\n");
    fflush(stdout);
    ExitProcess(0);
}
