#include <stdio.h>
#include <wchar.h>

#include "fable_resource_directories.h"

typedef long (FABLE_STDCALL *FableGetFolderPathW_0041A430)(
    void* window,
    int folder,
    void* token,
    fable_u32 flags,
    wchar_t* path);

namespace
{
    const wchar_t* g_folderPath;
    wchar_t g_returnStorage[260];
    unsigned int g_folderCalls;
    unsigned int g_appendCalls;
    unsigned int g_copyCalls;
    bool g_folderArgumentsValid;

    long FABLE_STDCALL FolderPathStub(
        void* window,
        int folder,
        void* token,
        fable_u32 flags,
        wchar_t* path)
    {
        ++g_folderCalls;
        g_folderArgumentsValid =
            window == 0 &&
            folder == 5 &&
            token == 0 &&
            flags == 0;
        wcscpy(path, g_folderPath);
        return 0;
    }

    bool CheckPath(
        const wchar_t* input,
        const wchar_t* expected,
        unsigned int expectedAppendCalls)
    {
        g_folderPath = input;
        g_folderCalls = 0;
        g_appendCalls = 0;
        g_copyCalls = 0;
        g_folderArgumentsValid = false;

        CWideString result =
            NResourceDirectoryNames::GetMyDocumentsDir();
        const wchar_t* actual = result;
        return
            g_folderCalls == 1 &&
            g_folderArgumentsValid &&
            g_appendCalls == expectedAppendCalls &&
            g_copyCalls == 1 &&
            wcscmp(actual, expected) == 0;
    }
}

extern "C" FableGetFolderPathW_0041A430
    g_FableGetFolderPathW_0041A430 = &FolderPathStub;

CWideString::CWideString(const wchar_t* text)
{
    storage_ = reinterpret_cast<CWideStringData*>(
        const_cast<wchar_t*>(text));
}

CWideString::CWideString(const CWideString& other)
{
    ++g_copyCalls;
    wcscpy(
        g_returnStorage,
        reinterpret_cast<const wchar_t*>(other.storage_));
    storage_ = reinterpret_cast<CWideStringData*>(g_returnStorage);
}

CWideString::~CWideString()
{
}

CWideString::operator const wchar_t*() const
{
    return reinterpret_cast<const wchar_t*>(storage_);
}

long CWideString::GetLength() const
{
    return static_cast<long>(wcslen(
        reinterpret_cast<const wchar_t*>(storage_)));
}

const CWideString& CWideString::operator+=(
    const wchar_t* text)
{
    ++g_appendCalls;
    wcscat(
        reinterpret_cast<wchar_t*>(storage_),
        text);
    return *this;
}

int main()
{
    const bool passed =
        CheckPath(
            L"C:\\Users\\Hero\\Documents",
            L"C:\\Users\\Hero\\Documents\\",
            1) &&
        CheckPath(
            L"D:\\Profiles\\Hero\\",
            L"D:\\Profiles\\Hero\\",
            0);

    if (!passed)
    {
        printf("FABLETLC_MY_DOCUMENTS_DIRECTORY_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_MY_DOCUMENTS_DIRECTORY_BEHAVIOR PASS\n");
    return 0;
}
