#include <stdio.h>

#include "fable_disk_file.h"
#include "test_support/base_class_stubs.h"

class CFileInformation
{
};

namespace
{
    // Every recorded event gets the next sequence number.
    int g_sequence;
    int g_baseConstructedAt;
    int g_pathnameConstructedAt;
    int g_openedAt;
    int g_closedAt;
    int g_pathnameDestroyedAt;
    int g_baseDestroyedAt;
    const CDiskFileWin32* g_openSelf;
    const CWideString* g_openPathname;
    FileAccessType g_openAccessType;
    unsigned long g_openFlags;
    long g_positionCalls;
}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { g_baseDestroyedAt = ++g_sequence; }
CAFile::CAFile() { g_baseConstructedAt = ++g_sequence; }

CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }

CWideString::CWideString() : storage_(0)
{
    g_pathnameConstructedAt = ++g_sequence;
}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() { g_pathnameDestroyedAt = ++g_sequence; }

void CDiskFileWin32::Open(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    g_openedAt = ++g_sequence;
    g_openSelf = this;
    g_openPathname = &pathname;
    g_openAccessType = accessType;
    g_openFlags = flags;
    AccessType = accessType;
    Opened = true;
}

void CDiskFileWin32::Close()
{
    g_closedAt = ++g_sequence;
    Opened = false;
}

void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return ++g_positionCalls; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation()
{
    return CFileInformation();
}

int main()
{
    CWideString pathname;
    g_sequence = 0;

    CAFile* file = new CDiskFileWin32(
        pathname, static_cast<FileAccessType>(3), 0x240);
    if (
        g_baseConstructedAt != 1 ||
        g_pathnameConstructedAt != 2 ||
        g_openedAt != 3 ||
        g_openSelf != file ||
        g_openPathname != &pathname ||
        g_openAccessType != 3 ||
        g_openFlags != 0x240)
    {
        return 1;
    }

    // The object dispatches through the CDiskFileWin32 vtable.
    if (!file->IsOpen() || file->GetPosition() != 1 || file->GetAlignment() != 1)
        return 2;

    delete file;
    if (g_closedAt != 4 || g_pathnameDestroyedAt != 5 || g_baseDestroyedAt != 6)
        return 3;

    printf("FABLETLC_DISK_FILE_CTOR_0099AD80 PASS\n");
    return 0;
}
