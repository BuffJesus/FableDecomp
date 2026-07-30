#include <stdio.h>
#include <wchar.h>

#include "fable_resource_directories.h"

namespace
{
    const wchar_t* WideText(const CWideString& value)
    {
        return *reinterpret_cast<const wchar_t* const*>(&value);
    }
}

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

CWideString::CWideString(const wchar_t* text)
{
    storage_ = reinterpret_cast<CWideStringData*>(
        const_cast<wchar_t*>(text));
}

CWideString::~CWideString()
{
}

CWideString FABLE_FASTCALL
FableGetMyDocumentsDirectory_0041A430()
{
    return CWideString(L"C:\\Users\\Hero\\Documents\\");
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right)
{
    static wchar_t combined[96];
    wcscpy(combined, WideText(left));
    wcscat(combined, right);
    return CWideString(combined);
}

int main()
{
    CWideString value =
        FableGetAutoSaveCheckpointDirectory_0041A4C0();
    if (wcscmp(
            WideText(value),
            L"C:\\Users\\Hero\\Documents\\My Games\\Fable\\") != 0)
    {
        printf("FABLETLC_CHECKPOINT_DIRECTORY_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_CHECKPOINT_DIRECTORY_BEHAVIOR PASS\n");
    return 0;
}
