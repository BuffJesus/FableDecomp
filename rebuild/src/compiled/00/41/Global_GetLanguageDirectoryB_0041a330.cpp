#include "fable_language.h"
#include "fable_resource_directories.h"

CWideString FABLE_FASTCALL
FableGetLanguageDirectoryB_0041A330()
{
    return (
        L"Data\\lang\\" +
        NLocalisation::GetLanguageFolder().ToWideString()) +
        L"\\";
}
