#include "fable_language.h"
#include "fable_resource_directories.h"

CWideString FABLE_FASTCALL
FableGetLanguageDirectoryA_0041A120()
{
    return (
        L"Data\\lang\\" +
        NLocalisation::GetLanguageFolder().ToWideString()) +
        L"\\";
}
