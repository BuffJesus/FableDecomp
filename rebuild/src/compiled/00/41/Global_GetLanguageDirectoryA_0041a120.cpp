#include "fable_language.h"
#include "fable_resource_directories.h"

CWideString FABLE_FASTCALL
FableGetLanguageDirectoryA_0041A120()
{
    return (
        L"Data\\lang\\" +
        FableGetDefaultLanguageName_00415530().ToWideString()) +
        L"\\";
}
