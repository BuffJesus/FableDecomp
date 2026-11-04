#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_disk_file.h"
#include "test_support/base_class_stubs.h"

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
extern "C" __declspec(dllimport) FableBool FABLE_STDCALL WriteFile(
    FableHandle file,
    const void* data,
    FableDword size,
    FableDword* written,
    void* overlapped);
extern "C" __declspec(dllimport) FableBool FABLE_STDCALL GetHandleInformation(
    FableHandle object,
    FableDword* flags);
extern "C" __declspec(dllimport) FableDword FABLE_STDCALL GetTempPathW(
    FableDword size,
    wchar_t* buffer);
extern "C" __declspec(dllimport) FableDword FABLE_STDCALL GetFileAttributesW(
    const wchar_t* name);
extern "C" __declspec(dllimport) FableBool FABLE_STDCALL DeleteFileW(
    const wchar_t* name);

class CFileInformation
{
};

namespace
{
    const FableDword kGenericWrite = 0x40000000ul;
    const FableDword kCreateAlways = 2;
    const FableDword kFileAttributeNormal = 0x80;
    FableHandle g_openedHandle;
    wchar_t g_path[300];
}

// The real engine members the Close object links against, as small
// working stand-ins: Open creates a real Win32 file.
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CAFile::CAFile() {}

CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }
CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() {}

void CDiskFileWin32::Open(
    const CWideString&,
    FileAccessType accessType,
    unsigned long)
{
    AccessType = accessType;
    WinFileHandle = CreateFileW(
        g_path, kGenericWrite, 0, 0, kCreateAlways, kFileAttributeNormal, 0);
    g_openedHandle = WinFileHandle;
    Opened = WinFileHandle != reinterpret_cast<FableHandle>(-1);
}

void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation()
{
    return CFileInformation();
}

static int Run()
{
    CDiskFileWin32 file;
    CAFile& abstractFile = file;

    // Closing a file that was never opened only clears the flag.
    abstractFile.Close();
    if (file.IsOpen())
        return 1;

    abstractFile.Open(CWideString(), static_cast<FileAccessType>(2), 0);
    if (!file.IsOpen())
        return 2;
    FableDword written = 0;
    if (!WriteFile(g_openedHandle, "fable", 5, &written, 0) || written != 5)
        return 3;

    abstractFile.Close();
    FableDword handleFlags = 0;
    if (file.IsOpen())
        return 4;
    if (GetHandleInformation(g_openedHandle, &handleFlags))
        return 5;

    // A second Close must not touch the (now dead) handle again.
    abstractFile.Close();
    if (file.IsOpen())
        return 6;

    // The handle was really released: the file can be deleted.
    if (!DeleteFileW(g_path) || GetFileAttributesW(g_path) != 0xFFFFFFFFul)
        return 7;
    return 0;
}

int main()
{
    FableDword length = GetTempPathW(260, g_path);
    if (length == 0 || length > 260)
        return 10;
    wcscat(g_path, L"fabletlc_close_0099a920.tmp");

    const int result = Run();
    if (result != 0)
    {
        DeleteFileW(g_path);
        printf("FABLETLC_DISK_FILE_CLOSE_0099A920 FAIL %d\n", result);
        return result;
    }
    printf("FABLETLC_DISK_FILE_CLOSE_0099A920 PASS\n");
    return 0;
}
