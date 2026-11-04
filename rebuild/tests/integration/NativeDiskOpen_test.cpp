// Behaviour fixture for CDiskFileWin32::Open (0x0099A6A0), against real
// temporary files through kernel32. Strings are heap-text stand-ins.
#define private public
#include "fable_disk_file.h"
#include "fable_file_exception.h"
#undef private

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef void* HANDLE;
typedef unsigned long DWORD;
typedef int BOOL;
#define MAX_PATH 260
#define GENERIC_READ 0x80000000UL
#define GENERIC_WRITE 0x40000000UL
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_NORMAL 0x80
extern "C"
{
    __declspec(dllimport) DWORD __stdcall GetTempPathW(DWORD, wchar_t*);
    __declspec(dllimport) unsigned int __stdcall GetTempFileNameW(const wchar_t*, const wchar_t*, unsigned int, wchar_t*);
    __declspec(dllimport) HANDLE __stdcall CreateFileW(const wchar_t*, DWORD, DWORD, void*, DWORD, DWORD, HANDLE);
    __declspec(dllimport) BOOL __stdcall CloseHandle(HANDLE);
    __declspec(dllimport) BOOL __stdcall DeleteFileW(const wchar_t*);
    __declspec(dllimport) BOOL __stdcall WriteFile(HANDLE, const void*, DWORD, DWORD*, void*);
}

namespace
{
    wchar_t*& WSlot(const CWideString& s) { return *reinterpret_cast<wchar_t**>(const_cast<CWideString*>(&s)); }
    char*& CSlot(const CCharString& s) { return *reinterpret_cast<char**>(const_cast<CCharString*>(&s)); }
    const wchar_t* WText(const CWideString& s) { return WSlot(s) ? WSlot(s) : L""; }
    const char* CText(const CCharString& s) { return CSlot(s) ? CSlot(s) : ""; }
    wchar_t* WDup(const wchar_t* t) { size_t n = wcslen(t) + 1; wchar_t* p = (wchar_t*)malloc(n * 2); memcpy(p, t, n * 2); return p; }
    char* CDup(const char* t) { size_t n = strlen(t) + 1; char* p = (char*)malloc(n); memcpy(p, t, n); return p; }
    CCharString Make(const char* a, const char* b)
    {
        CCharString r((const char*)0, 0);
        char* p = (char*)malloc(strlen(a) + strlen(b) + 1);
        strcpy(p, a); strcat(p, b);
        CSlot(r) = p;
        return r;
    }

    int g_validated;
    int g_directories;
    char g_lastMessage[512];
}

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const wchar_t* text) : storage_(0) { WSlot(*this) = WDup(text); }
CWideString::~CWideString() { free(WSlot(*this)); WSlot(*this) = 0; }
CWideString& CWideString::operator=(const CWideString& other)
{
    wchar_t* copy = WDup(WText(other));
    free(WSlot(*this));
    WSlot(*this) = copy;
    return *this;
}
long CWideString::GetLength() const { return (long)wcslen(WText(*this)); }
CWideString::operator const wchar_t*() const { return WText(*this); }
CCharString CWideString::ToCharString() const
{
    char buffer[MAX_PATH * 2];
    size_t i = 0;
    for (const wchar_t* w = WText(*this); *w; ++w) buffer[i++] = (char)*w;
    buffer[i] = 0;
    return Make(buffer, "");
}

CCharString::CCharString(const char* text, long) : storage_(0) { if (text) CSlot(*this) = CDup(text); }
CCharString::CCharString(const CCharString& other) : storage_(0) { CSlot(*this) = CDup(CText(other)); }
CCharString::~CCharString() { free(CSlot(*this)); CSlot(*this) = 0; }
CCharString::operator const char*() const
{
    strncpy(g_lastMessage, CText(*this), sizeof(g_lastMessage) - 1);
    return CText(*this);
}
CCharString FABLE_FASTCALL operator+(const char* left, const CCharString& right) { return Make(left, CText(right)); }
CCharString FABLE_FASTCALL operator+(const CCharString& left, const char* right) { return Make(CText(left), right); }
CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right) { return Make(CText(left), CText(right)); }
CCharString FABLE_FASTCALL GFIntToCharString(long value)
{
    char buffer[16];
    sprintf(buffer, "%ld", value);
    return Make(buffer, "");
}
CCharString FABLE_FASTCALL GFGetLastWindowsError() { return Make("ERR", ""); }

bool FABLE_FASTCALL CAFile::ConvertPathToFullPath(const CWideString& path, CWideString& fullPath)
{
    fullPath = path;
    return true;
}
void FABLE_FASTCALL CAFile::ValidatePathnameIsOnWriteableDrive(const CWideString&) { ++g_validated; }
void FABLE_FASTCALL CAFile::CreateDirectoryForFile(const CWideString&) { ++g_directories; }

namespace
{
    struct RawFile
    {
        unsigned long words[7];
        CDiskFileWin32* operator->() { return reinterpret_cast<CDiskFileWin32*>(words); }
    };

    void Setup(RawFile& f)
    {
        for (int i = 0; i < 7; ++i)
            f.words[i] = 0xCDCDCDCD;
        f.words[6] = 0;  // Pathname: an empty wide string
        g_validated = g_directories = 0;
        g_lastMessage[0] = 0;
    }

    void Release(RawFile& f)
    {
        if (f->Opened && f->WinFileHandle && f->WinFileHandle != (void*)0xCDCDCDCD)
            CloseHandle(f->WinFileHandle);
        free(WSlot(f->Pathname));
    }

    wchar_t g_temp[MAX_PATH];

    void MakeTemp(const char* contents)
    {
        // Caller supplies a new build-owned path; keep every output for inspection.
        HANDLE h = CreateFileW(g_temp, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        DWORD done = 0;
        WriteFile(h, contents, (DWORD)strlen(contents), &done, 0);
        CloseHandle(h);
    }

    bool Opened(RawFile& f, FileAccessType access, long length, int validated, int directories)
    {
        return f->Opened && f->AccessType == access && f->Length == length && f->Position == 0 &&
               f->WinFileHandle != 0 && f->WinFileHandle != (void*)-1 &&
               wcscmp(WText(f->Pathname), g_temp) == 0 &&
               g_validated == validated && g_directories == directories;
    }
}

int main(int argc,char** argv)
{
    if(argc!=2 || mbstowcs(g_temp,argv[1],MAX_PATH-1)==size_t(-1))return 80;
    int fail = 0;
    RawFile f;

    // Read an existing file (flag 2: sequential scan).
    MakeTemp("hello");
    Setup(f);
    f->CDiskFileWin32::Open(CWideString(g_temp), FILE_ACCESS_READ, 2);
    if (!Opened(f, FILE_ACCESS_READ, 5, 0, 0)) {printf("DISK_OPEN_HOST_FAIL case=1\n");return 80;}
    Release(f);

    // Read/write an existing file: the drive is checked.
    Setup(f);
    f->CDiskFileWin32::Open(CWideString(g_temp), FILE_ACCESS_READ_WRITE, 0);
    if (!Opened(f, FILE_ACCESS_READ_WRITE, 5, 1, 0)) {printf("DISK_OPEN_HOST_FAIL case=2\n");return 80;}
    Release(f);

    // Read/write/create keeps the contents and counts as read/write.
    Setup(f);
    f->CDiskFileWin32::Open(CWideString(g_temp), FILE_ACCESS_READ_WRITE_CREATE, 0);
    if (!Opened(f, FILE_ACCESS_READ_WRITE, 5, 1, 0)) {printf("DISK_OPEN_HOST_FAIL case=4\n");return 80;}
    Release(f);

    // Create truncates, makes the directory and counts as read/write.
    Setup(f);
    f->CDiskFileWin32::Open(CWideString(g_temp), FILE_ACCESS_CREATE, 0);
    if (!Opened(f, FILE_ACCESS_READ_WRITE, 0, 1, 1)) {printf("DISK_OPEN_HOST_FAIL case=8\n");return 80;}
    Release(f);

    // A missing file: FILE_NOT_FOUND with the path, access type and Windows error.
    wcscat(g_temp,L".missing"); // Absent sibling; no deletion.
    Setup(f);
    bool threw = false;
    try { f->CDiskFileWin32::Open(CWideString(g_temp), FILE_ACCESS_READ, 0); }
    catch (CBBBFileException& e) { threw = e.Type == CBBBFileException::FILE_NOT_FOUND; }
    char expected[600];
    char narrow[MAX_PATH * 2];
    size_t i = 0;
    for (const wchar_t* w = g_temp; *w; ++w) narrow[i++] = (char)*w;
    narrow[i] = 0;
    sprintf(expected, "Cannot open file '%s' (access type 1) - ERR", narrow);
    if (!threw || strcmp(g_lastMessage, expected) != 0 || reinterpret_cast<unsigned char*>(f.words)[0x14] != 0xCD) fail |= 16;
    free(WSlot(f->Pathname));

    // An empty path: BAD_PARAMETERS before any open.
    Setup(f);
    threw = false;
    try { f->CDiskFileWin32::Open(CWideString(L""), FILE_ACCESS_READ, 0); }
    catch (CBBBFileException& e) { threw = e.Type == CBBBFileException::BAD_PARAMETERS; }
    if (!threw || strcmp(g_lastMessage, "Bad filename ") != 0 || f->WinFileHandle != 0) fail |= 32;
    free(WSlot(f->Pathname));

    // An unknown access type opens nothing but still marks the file open.
    Setup(f);
    f->CDiskFileWin32::Open(CWideString(L"x"), (FileAccessType)7, 0);
    if (!f->Opened || f->WinFileHandle != 0 || f->AccessType != (FileAccessType)0xCDCDCDCD || f->Position != 0) fail |= 64;
    free(WSlot(f->Pathname));

    if (fail == 0)
        printf("FABLETLC_CDISKFILEWIN32_OPEN_0099A6A0_PASS\n");
    else
        printf("FABLETLC_CDISKFILEWIN32_OPEN_0099A6A0 FAIL %d\n", fail);
    return fail?80:0;
}