// Behaviour fixture for CDiskFileWin32::SetLength (0x0099A560).
// ---- CDiskFileWin32 fixture prelude --------------------------------------
// The member under test is linked from the compiled source object and called
// non-virtually on raw storage laid out as CDiskFileWin32 (no vtable needed).
// It runs against a REAL temporary file through kernel32; the CCharString
// members the inline CBBBFileException constructor uses are recording
// stand-ins.
#define private public
#include "fable_disk_file.h"
#include "fable_file_exception.h"
#undef private

#include <stdio.h>
#include <string.h>

// Minimal kernel32 surface (the VC7.1 tree has no Platform SDK headers).
typedef void* HANDLE;
typedef unsigned long DWORD;
typedef int BOOL;
#define MAX_PATH 260
#define GENERIC_READ 0x80000000UL
#define GENERIC_WRITE 0x40000000UL
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define FILE_ATTRIBUTE_NORMAL 0x80
#define FILE_BEGIN 0
#define FILE_CURRENT 1
extern "C"
{
    __declspec(dllimport) DWORD __stdcall GetTempPathW(DWORD, wchar_t*);
    __declspec(dllimport) unsigned int __stdcall GetTempFileNameW(const wchar_t*, const wchar_t*, unsigned int, wchar_t*);
    __declspec(dllimport) HANDLE __stdcall CreateFileW(const wchar_t*, DWORD, DWORD, void*, DWORD, DWORD, HANDLE);
    __declspec(dllimport) BOOL __stdcall CloseHandle(HANDLE);
    __declspec(dllimport) BOOL __stdcall DeleteFileW(const wchar_t*);
    __declspec(dllimport) DWORD __stdcall SetFilePointer(HANDLE, long, long*, DWORD);
    __declspec(dllimport) BOOL __stdcall WriteFile(HANDLE, const void*, DWORD, DWORD*, void*);
    __declspec(dllimport) BOOL __stdcall ReadFile(HANDLE, void*, DWORD, DWORD*, void*);
    __declspec(dllimport) DWORD __stdcall GetFileSize(HANDLE, DWORD*);
}

static int g_strCtor, g_strCopy, g_strText, g_strDtor;
static const char* g_strLiteral;
static long g_strLength;

CCharString::CCharString(const char* text, long length)
{
    storage_ = reinterpret_cast<CCharStringData*>(const_cast<char*>(text));
    g_strLiteral = text;
    g_strLength = length;
    ++g_strCtor;
}

CCharString::CCharString(const CCharString& other)
{
    storage_ = other.storage_;
    ++g_strCopy;
}

CCharString::operator const char*() const
{
    ++g_strText;
    return reinterpret_cast<const char*>(storage_);
}

CCharString::~CCharString()
{
    ++g_strDtor;
}

static void ResetStrings()
{
    g_strCtor = g_strCopy = g_strText = g_strDtor = 0;
    g_strLiteral = 0;
    g_strLength = 0;
}

static bool StringsThrew(const char* text)
{
    return g_strCtor == 1 && g_strCopy == 1 && g_strText == 1 && g_strDtor == 1 &&
        g_strLength == -1 && g_strLiteral != 0 && strcmp(g_strLiteral, text) == 0;
}

static bool StringsUntouched()
{
    return g_strCtor == 0 && g_strCopy == 0 && g_strText == 0 && g_strDtor == 0;
}

struct RawFile
{
    unsigned long words[7];
    CDiskFileWin32* operator->() { return reinterpret_cast<CDiskFileWin32*>(words); }
    CDiskFileWin32& operator*() { return *reinterpret_cast<CDiskFileWin32*>(words); }
};

static wchar_t g_tempPath[MAX_PATH];

static HANDLE OpenTemp(const char* contents, bool writable)
{
    wchar_t dir[MAX_PATH];
    GetTempPathW(MAX_PATH, dir);
    GetTempFileNameW(dir, L"dfw", 0, g_tempPath);
    HANDLE h = CreateFileW(g_tempPath, GENERIC_READ | GENERIC_WRITE, 0, 0,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD done = 0;
    WriteFile(h, contents, (DWORD)strlen(contents), &done, 0);
    if (!writable)
    {
        CloseHandle(h);
        h = CreateFileW(g_tempPath, GENERIC_READ, 0, 0, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, 0);
    }
    return h;
}

static void CloseTemp(HANDLE h)
{
    CloseHandle(h);
    DeleteFileW(g_tempPath);
}

static long Pointer(HANDLE h)
{
    return (long)SetFilePointer(h, 0, 0, FILE_CURRENT);
}

static void Setup(RawFile& f, HANDLE h, FileAccessType access, long position, long length)
{
    for (int i = 0; i < 7; ++i)
        f.words[i] = 0xCDCDCDCD;
    f->AccessType = access;
    f->Position = position;
    f->Length = length;
    f->WinFileHandle = h;
    f->Opened = true;
}

// The bytes the member must never touch: the vptr slot, Opened and Pathname.
static bool NeighboursIntact(RawFile& f)
{
    return f.words[0] == 0xCDCDCDCD && f->Opened == true &&
        f.words[6] == 0xCDCDCDCD;
}
// ---------------------------------------------------------------------------


int main()
{
    int fail = 0;
    RawFile f;
    HANDLE h = OpenTemp("0123456789", true);

    // Shrinking or equal: no-op (pointer untouched).
    SetFilePointer(h, 2, 0, FILE_BEGIN);
    Setup(f, h, FILE_ACCESS_READ_WRITE, 4, 10);
    f->CDiskFileWin32::SetLength(10);
    f->CDiskFileWin32::SetLength(3);
    if (f->Length != 10 || f->Position != 4 || Pointer(h) != 2) fail |= 1;

    // Growing: cached length updated, the real pointer ends at the cached position.
    f->CDiskFileWin32::SetLength(64);
    if (f->Length != 64 || f->Position != 4 || Pointer(h) != 4) fail |= 2;
    if (!NeighboursIntact(f) || f->WinFileHandle != h || f->AccessType != FILE_ACCESS_READ_WRITE) fail |= 4;
    // A bare pointer move does not extend the real file.
    if (GetFileSize(h, 0) != 10) fail |= 8;

    CloseTemp(h);
    if (fail == 0)
        printf("CDISKFILEWIN32_SETLENGTH_PASS\n");
    else
        printf("CDISKFILEWIN32_SETLENGTH_FAIL %d\n", fail);
    return 0;
}