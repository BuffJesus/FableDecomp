#include <stdio.h>
#include "fable_disk_file.h"

class CFileInformation {};

enum Event { WIDE_LITERAL, PATH_EXISTS, WIDE_DESTROY, DISK_CONSTRUCT,
             STRING_FROM_FILE, STRING_ASSIGN, STRING_DESTROY, DISK_CLOSE };
static Event events[20];
static int count;
static bool pathExists;
static void record(Event event) { events[count++] = event; }

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CAFile::CAFile() {}
CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const wchar_t*) : storage_(0) { record(WIDE_LITERAL); }
CWideString::CWideString(const CWideString&) : storage_(0) {}
CWideString::~CWideString() { record(WIDE_DESTROY); }

bool FABLE_FASTCALL CAFile::PathExists(const CWideString&)
{
    record(PATH_EXISTS);
    return pathExists;
}

CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long)
    : Opened(false)
{
    record(DISK_CONSTRUCT);
}
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() { record(DISK_CLOSE); }
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { return CFileInformation(); }

CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(CAFile*, long) : storage_(0) { record(STRING_FROM_FILE); }
CCharString::~CCharString() { record(STRING_DESTROY); }
CCharString& CCharString::operator=(const CCharString&)
{
    record(STRING_ASSIGN);
    return *this;
}
CCharString FableFullBuildVersion;

void FABLE_CDECL GFGetBuildNumber2();

int main()
{
    pathExists = false;
    GFGetBuildNumber2();
    const Event absent[] = { WIDE_LITERAL, PATH_EXISTS, WIDE_DESTROY };
    if (count != 3) return 1;
    for (int i = 0; i < 3; ++i) if (events[i] != absent[i]) return 2;
    count = 0;
    pathExists = true;
    GFGetBuildNumber2();
    const Event present[] = { WIDE_LITERAL, PATH_EXISTS, WIDE_DESTROY,
        WIDE_LITERAL, DISK_CONSTRUCT, WIDE_DESTROY, STRING_FROM_FILE,
        STRING_ASSIGN, STRING_DESTROY, DISK_CLOSE, WIDE_DESTROY };
    if (count != 11) return 3;
    for (int i = 0; i < 11; ++i) if (events[i] != present[i]) return 4;
    puts("GF_GET_BUILD_NUMBER_TYPED_PASS");
    return 0;
}
