#include "fable_language.h"
#include "fable_resource_directories.h"

CWideString FABLE_FASTCALL
FableGetLanguageDirectoryB_0041A330()
{
    return (
        L"Data\\lang\\" +
        FableGetDefaultLanguageName_00415530().ToWideString()) +
        L"\\";
}
