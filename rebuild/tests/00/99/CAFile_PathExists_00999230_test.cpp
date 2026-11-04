#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_filesystem.h"

typedef int FableBool;
typedef unsigned long FableDword;
typedef void* FableHandle;

extern "C" __declspec(dllimport) FableHandle FABLE_STDCALL CreateFileW(
    const wchar_t* name,
    FableDword access,
    FableDword share,
    void* security,
    FableDword disposition,
    FableDword flags,
    FableHandle templateFile);
extern "C" __declspec(dllimport) FableBool FABLE_STDCALL CloseHandle(
    FableHandle object);
extern "C" __declspec(dllimport) FableBool FABLE_STDCALL DeleteFileW(
    const wchar_t* name);
extern "C" __declspec(dllimport) FableDword FABLE_STDCALL GetTempPathW(
    FableDword size,
    wchar_t* buffer);

namespace
{
    const unsigned int kTextCharacters = 300;
    wchar_t g_tempDirectory[kTextCharacters];
    int g_liveStrings;
    unsigned int g_currentPathCalls;
    unsigned int g_expandCalls;
}

// Minimal owning CWideString: storage_ points at a heap copy of the text.
static CWideStringData* NewData(const wchar_t* text)
{
    // Raw block: CWideStringData has no default ctor.
    CWideStringData* data = static_cast<CWideStringData*>(operator new(sizeof(CWideStringData)));
    data->text = new wchar_t[wcslen(text) + 1];
    wcscpy(data->text, text);
    data->owners = 1;
    return data;
}

CWideString::CWideString() : storage_(0) { ++g_liveStrings; }
CWideString::CWideString(const wchar_t* text) : storage_(NewData(text))
{
    ++g_liveStrings;
}
CWideString::CWideString(const CWideString& other)
    : storage_(other.storage_ ? NewData(other.storage_->text) : 0)
{
    ++g_liveStrings;
}
CWideString::~CWideString()
{
    --g_liveStrings;
    if (storage_)
    {
        wchar_t* text = storage_->text;  // (a property in delete[] trips VC7.1)
        delete[] text;
        delete storage_;
    }
}
CWideString& CWideString::operator=(const CWideString& other)
{
    CWideStringData* copy = other.storage_ ? NewData(other.storage_->text) : 0;
    if (storage_)
    {
        wchar_t* text = storage_->text;  // (a property in delete[] trips VC7.1)
        delete[] text;
        delete storage_;
    }
    storage_ = copy;
    return *this;
}
CWideString::operator const wchar_t*() const
{
    return storage_ ? storage_->text : L"";
}

CWideString FABLE_FASTCALL CAFile::GetCurrentPath()
{
    ++g_currentPathCalls;
    return CWideString(g_tempDirectory);
}

// Relative names resolve against the context directory.
CWideString FABLE_FASTCALL CAFile::ExpandPath(
    const CWideString& pathname,
    const CWideString& context)
{
    ++g_expandCalls;
    wchar_t text[kTextCharacters];
    wcscpy(text, context);
    wcscat(text, pathname);
    return CWideString(text);
}

static int Run(const wchar_t* fileName)
{
    wchar_t fullPath[kTextCharacters];
    wcscpy(fullPath, g_tempDirectory);
    wcscat(fullPath, fileName);
    DeleteFileW(fullPath);

    const CWideString relative(fileName);
    if (CAFile::PathExists(relative))
        return 1;

    FableHandle file = CreateFileW(fullPath, 0x40000000ul, 0, 0, 2, 0x80, 0);
    if (file == reinterpret_cast<FableHandle>(-1))
        return 2;
    CloseHandle(file);

    const bool fileExists = CAFile::PathExists(relative);
    DeleteFileW(fullPath);
    if (!fileExists)
        return 3;

    // A directory is a path too.
    if (!CAFile::PathExists(CWideString(L"")))
        return 4;

    if (CAFile::PathExists(relative))
        return 5;
    if (g_currentPathCalls != 4 || g_expandCalls != 4)
        return 6;
    return 0;
}

int main()
{
    const FableDword length = GetTempPathW(260, g_tempDirectory);
    if (length == 0 || length > 260)
        return 10;

    const int result = Run(L"fabletlc_pathexists_00999230.tmp");
    // Every temporary PathExists built was destroyed.
    if (result != 0 || g_liveStrings != 0)
    {
        printf("FABLETLC_PATH_EXISTS_00999230 FAIL %d %d\n", result, g_liveStrings);
        return result ? result : 11;
    }
    printf("FABLETLC_PATH_EXISTS_00999230 PASS\n");
    return 0;
}
