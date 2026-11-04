// Behaviour fixture for CDiskFileWin32::Clone (0x0099AE00).
// Links the compiled source object. Every other member the object's
// CDiskFileWin32 vftable references is a recording stand-in here.
#include "fable_disk_file.h"

#include <stdio.h>
#include <stdlib.h>
#include "test_support/base_class_stubs.h"

class CFileInformation
{
public:
    unsigned char bytes[0x58];
};

static int g_seq;
static int g_newCalls, g_failNew;
static unsigned int g_newSize;
static int g_baseCtorSeq, g_pathCtorSeq, g_openSeq, g_closeCalls;
static const void* g_openThis;
static const CWideString* g_openPath;
static FileAccessType g_openAccess;
static unsigned long g_openFlags;
static bool g_openedAtOpen;
static void* g_vptrAtOpen;

void* operator new(unsigned int size)
{
    ++g_newCalls;
    g_newSize = size;
    if (g_failNew)
        return 0;
    return malloc(size);
}

void operator delete(void* p)
{
    free(p);
}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CAFile::CAFile() { g_baseCtorSeq = ++g_seq; }

CFileInformation CAFile::GetFileInformation() { CFileInformation info; return info; }

CWideString::CWideString() : storage_(0) { g_pathCtorSeq = ++g_seq; }
CWideString::~CWideString() {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}

void CDiskFileWin32::Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    g_openSeq = ++g_seq;
    g_openThis = this;
    g_openPath = &pathname;
    g_openAccess = accessType;
    g_openFlags = flags;
    g_openedAtOpen = Opened;
    g_vptrAtOpen = *reinterpret_cast<void**>(this);
    Opened = true;
}
void CDiskFileWin32::Close() { ++g_closeCalls; }
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CFileInformation CDiskFileWin32::GetFileInformation() { CFileInformation info; return info; }

int main()
{
    int fail = 0;

    // Source file object: a real CDiskFileWin32 (default ctor is inline).
    CDiskFileWin32 source;
    void* diskVptr = *reinterpret_cast<void**>(&source);
    const CDiskFileWin32& csource = source;
    const CWideString* sourcePath = reinterpret_cast<const CWideString*>(
        reinterpret_cast<const unsigned char*>(&source) + 0x18);

    g_seq = 0;
    g_newCalls = 0;
    CAFile* clone = csource.CDiskFileWin32::Clone();
    if (clone == 0 || g_newCalls != 1 || g_newSize != 0x1C) fail |= 1;
    // Construction order: CAFile base, Pathname, then Open on the new object.
    if (g_baseCtorSeq != 1 || g_pathCtorSeq != 2 || g_openSeq != 3) fail |= 2;
    if (g_openThis != clone || g_openPath != sourcePath) fail |= 4;
    if (g_openAccess != FILE_ACCESS_READ || g_openFlags != 2) fail |= 8;
    // Opened was cleared and the most-derived vptr installed before Open.
    if (g_openedAtOpen != false || g_vptrAtOpen != diskVptr) fail |= 16;
    if (clone != 0 && *reinterpret_cast<void**>(clone) != diskVptr) fail |= 32;
    if (clone != 0 && !clone->IsOpen()) fail |= 64;

    // Deleting through the base closes the clone (inline virtual dtor).
    g_closeCalls = 0;
    delete clone;
    if (g_closeCalls != 1) fail |= 128;

    // Allocation failure: null result, nothing constructed or opened.
    g_seq = 0;
    g_failNew = 1;
    g_openSeq = g_baseCtorSeq = 0;
    CAFile* none = csource.CDiskFileWin32::Clone();
    g_failNew = 0;
    if (none != 0 || g_openSeq != 0 || g_baseCtorSeq != 0) fail |= 256;

    if (fail == 0)
        printf("CDISKFILEWIN32_CLONE_PASS\n");
    else
        printf("CDISKFILEWIN32_CLONE_FAIL %d\n", fail);
    return 0;
}