#include <stdio.h>
#include <wchar.h>

#include "fable_filesystem.h"

namespace
{
    const wchar_t* g_fixturePath = L"C:\\FableTLC\\Project";
    const wchar_t* g_changedPath = 0;
    int g_changeDirectoryCalls = 0;
    int g_assignmentCalls = 0;
}

CWideString g_CAFileCurrentPath_013BCA10;

CWideString::CWideString()
    : storage_(0)
{
}

CWideString::~CWideString()
{
    storage_ = 0;
}

CWideString::operator const wchar_t*() const
{
    return reinterpret_cast<const wchar_t*>(storage_);
}

CWideString& CWideString::operator=(const CWideString& other)
{
    ++g_assignmentCalls;
    storage_ = other.storage_;
    return *this;
}

extern "C" int FABLE_CDECL _wchdir(const wchar_t* path)
{
    ++g_changeDirectoryCalls;
    g_changedPath = path;
    return 0;
}

static int VerifyCurrentPathUpdate()
{
    CWideString requestedPath;
    *reinterpret_cast<fable_u32*>(&requestedPath) =
        reinterpret_cast<fable_u32>(g_fixturePath);

    CAFile::SetCurrentPath(requestedPath);

    if (g_changeDirectoryCalls != 1)
        return 1;
    if (g_changedPath != g_fixturePath)
        return 2;
    if (g_assignmentCalls != 1)
        return 3;
    if (static_cast<const wchar_t*>(g_CAFileCurrentPath_013BCA10)
        != g_fixturePath)
        return 4;
    return 0;
}

int main()
{
    const int result = VerifyCurrentPathUpdate();
    if (result != 0)
    {
        printf("FABLETLC_SET_CURRENT_PATH_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_SET_CURRENT_PATH_BEHAVIOR PASS\n");
    return 0;
}
