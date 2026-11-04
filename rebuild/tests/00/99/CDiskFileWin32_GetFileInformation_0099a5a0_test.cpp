// Behaviour fixture for CDiskFileWin32::GetFileInformation (0x0099A5A0).
// Links the compiled source object and runs against a REAL temporary file
// through kernel32; CDateAndTime's constructor and Initialise are recording
// stand-ins (Initialise stores the FILETIME halves it was given).
#define private public
#include "fable_disk_file.h"
#include "fable_file_information.h"
#undef private

#include <stdio.h>
#include <string.h>

typedef void* HANDLE;
typedef unsigned long DWORD;
typedef int BOOL;
extern "C"
{
    __declspec(dllimport) DWORD __stdcall GetTempPathW(DWORD, wchar_t*);
    __declspec(dllimport) unsigned int __stdcall GetTempFileNameW(const wchar_t*, const wchar_t*, unsigned int, wchar_t*);
    __declspec(dllimport) HANDLE __stdcall CreateFileW(const wchar_t*, DWORD, DWORD, void*, DWORD, DWORD, HANDLE);
    __declspec(dllimport) BOOL __stdcall CloseHandle(HANDLE);
    __declspec(dllimport) BOOL __stdcall DeleteFileW(const wchar_t*);
    __declspec(dllimport) BOOL __stdcall SetFileAttributesW(const wchar_t*, DWORD);
    __declspec(dllimport) BOOL __stdcall GetFileTime(HANDLE, FILETIME*, FILETIME*, FILETIME*);
}

static int g_ctors;
static int g_inits;

CDateAndTime::CDateAndTime()
{
    ++g_ctors;
    Year = 0;
    Month = Day = Hour = Minute = Second = Millisecond = 0;
}

void CDateAndTime::Initialise(const FILETIME& fileTime)
{
    ++g_inits;
    Year = 1;
    Month = fileTime.dwLowDateTime;
    Day = fileTime.dwHighDateTime;
    Hour = Minute = Second = Millisecond = 7;
}

struct RawFile
{
    unsigned long words[7];
    CDiskFileWin32* operator->() { return reinterpret_cast<CDiskFileWin32*>(words); }
};

static bool Stamp(const CDateAndTime& t, const FILETIME& ft)
{
    return t.Year == 1 && t.Month == ft.dwLowDateTime && t.Day == ft.dwHighDateTime && t.Hour == 7;
}

static bool Zero(const CDateAndTime& t)
{
    return t.Year == 0 && t.Month == 0 && t.Day == 0 && t.Millisecond == 0;
}

int main()
{
    int fail = 0;
    wchar_t dir[260], path[260];
    GetTempPathW(260, dir);
    GetTempFileNameW(dir, L"dfi", 0, path);
    RawFile f;
    memset(f.words, 0xCD, sizeof(f.words));

    // Writable file: the three stamps come from the handle, ReadOnly false.
    HANDLE h = CreateFileW(path, 0x80000000UL, 1, 0, 3, 0x80, 0);
    FILETIME c, a, w;
    GetFileTime(h, &c, &a, &w);
    f->WinFileHandle = h;
    g_ctors = g_inits = 0;
    CFileInformation info = f->CDiskFileWin32::GetFileInformation();
    if (!Stamp(info.CreationTime, c) || !Stamp(info.LastAccessTime, a) || !Stamp(info.LastWriteTime, w)) fail |= 1;
    if (info.ReadOnly != false || g_ctors != 3 || g_inits != 3) fail |= 2;
    CloseHandle(h);

    // Read-only attribute: reported.
    SetFileAttributesW(path, 0x1);
    h = CreateFileW(path, 0x80000000UL, 1, 0, 3, 0x80, 0);
    f->WinFileHandle = h;
    CFileInformation ro = f->CDiskFileWin32::GetFileInformation();
    if (ro.ReadOnly != true) fail |= 4;
    CloseHandle(h);
    SetFileAttributesW(path, 0x80);
    DeleteFileW(path);

    // Query failure (invalid handle): zeroed stamps from fresh dates, writable.
    f->WinFileHandle = (HANDLE)0;
    g_ctors = g_inits = 0;
    CFileInformation bad = f->CDiskFileWin32::GetFileInformation();
    if (!Zero(bad.CreationTime) || !Zero(bad.LastAccessTime) || !Zero(bad.LastWriteTime)) fail |= 8;
    if (bad.ReadOnly != false || g_inits != 0 || g_ctors != 6) fail |= 16;
    // The file object itself is untouched.
    if (f.words[0] != 0xCDCDCDCD || f.words[2] != 0xCDCDCDCD || f.words[6] != 0xCDCDCDCD) fail |= 32;

    if (fail == 0)
        printf("CDISKFILEWIN32_GETFILEINFORMATION_PASS\n");
    else
        printf("CDISKFILEWIN32_GETFILEINFORMATION_FAIL %d\n", fail);
    return 0;
}