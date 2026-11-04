#include <stdio.h>
#include <string.h>

#include "fable_data_input_stream.h"
#include "fable_definition_table.h"
#include "fable_disk_file.h"
#include "test_support/base_class_stubs.h"
#include "test_support/data_input_stream_stubs.h"

class CFileInformation
{
};

namespace
{
    enum Event
    {
        kPathExists,
        kBaseConstruct,
        kPathnameConstruct,
        kOpen,
        kStreamConstruct,
        kRead,
        kStreamDestroy,
        kClose,
        kPathnameDestroy,
        kBaseDestroy
    };

    const unsigned int kMaxEvents = 16;
    Event g_events[kMaxEvents];
    unsigned int g_eventCount;
    bool g_recording;
    bool g_pathExists;
    CDefStringTable* g_expectedTable;
    void* g_file;
    void* g_diskFileVtable;
    CFileDataInputStream* g_stream;
    bool g_argumentsValid;
    bool g_flagsVisibleAtDestruction;
    bool g_expectedReadOnly;

    void Record(Event event)
    {
        if (!g_recording)
            return;
        if (g_eventCount < kMaxEvents)
            g_events[g_eventCount] = event;
        ++g_eventCount;
    }

    void Check(bool condition)
    {
        if (g_recording && !condition)
            g_argumentsValid = false;
    }

    void* Vtable(const void* object)
    {
        return *static_cast<void* const*>(object);
    }

    // Leaves non-zero garbage where LoadTable's locals will live, so the
    // inline CDiskFileWin32() must really clear Opened.
    void __declspec(noinline) DirtyStack()
    {
        volatile fable_u8 bytes[512];
        for (unsigned int i = 0; i < sizeof(bytes); ++i)
            bytes[i] = 0xA5;
    }
}

// The real engine members the LoadTable object links against, as small
// recording stand-ins.
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { if (this == g_file) Record(kBaseDestroy); }

CAFile::CAFile()
{
    Record(kBaseConstruct);
    g_file = this;
}



CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }

bool FABLE_FASTCALL CAFile::PathExists(const CWideString& pathname)
{
    Record(kPathExists);
    Check(&pathname == &g_expectedTable->TableFilename);
    return g_pathExists;
}

CWideString::CWideString() : storage_(0) { Record(kPathnameConstruct); }
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() { Record(kPathnameDestroy); }

void CDiskFileWin32::Open(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    Record(kOpen);
    Check(this == g_file);
    Check(Vtable(this) == g_diskFileVtable);
    Check(!IsOpen());
    Check(&pathname == &g_expectedTable->TableFilename);
    Check(accessType == 1);
    Check(flags == 4);
    Opened = true;
}

void CDiskFileWin32::Close()
{
    Record(kClose);
    Check(this == g_file);
    Check(Vtable(this) == g_diskFileVtable);
    Check(IsOpen());
    Opened = false;
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

void CDataInputStream::SetPosition(unsigned long position)
{
    StreamPos = position;
}

// The base stream constructor is out of line in retail (0x00993B20); the
// stream stub below sets every field itself.
CDataInputStream::CDataInputStream() {}

CFileDataInputStream::CFileDataInputStream(
    CAFile* file,
    unsigned long bufferSize)
{
    Record(kStreamConstruct);
    g_stream = this;
    Check(file == g_file);
    Check(bufferSize == 0x4000);
    File = file;
}

CFileDataInputStream::~CFileDataInputStream()
{
    Record(kStreamDestroy);
    Check(this == g_stream);
    if (g_recording)
    {
        g_flagsVisibleAtDestruction =
            g_expectedTable->Locked &&
            g_expectedTable->ReadOnly == g_expectedReadOnly;
    }
}

void CDefStringTable::ReadTable(CDataInputStream& stream)
{
    Record(kRead);
    Check(this == g_expectedTable);
    Check(&stream == g_stream);
    Check(g_stream != 0 && g_stream->File == g_file);
    Check(!Locked);
}

namespace
{
    bool CheckMissingPath()
    {
        fable_u8 tableBytes[sizeof(CDefStringTable)];
        memset(tableBytes, 0xA5, sizeof(tableBytes));
        CDefStringTable& table =
            *reinterpret_cast<CDefStringTable*>(tableBytes);
        table.Locked = false;
        table.ReadOnly = true;
        g_expectedTable = &table;
        g_pathExists = false;
        g_eventCount = 0;
        g_argumentsValid = true;
        g_recording = true;

        DirtyStack();
        table.LoadTable(false);
        g_recording = false;
        return
            g_eventCount == 1 &&
            g_events[0] == kPathExists &&
            g_argumentsValid &&
            !table.Locked &&
            table.ReadOnly;
    }

    bool CheckLoadedPath(bool readOnly)
    {
        fable_u8 tableBytes[sizeof(CDefStringTable)];
        memset(tableBytes, 0, sizeof(tableBytes));
        CDefStringTable& table =
            *reinterpret_cast<CDefStringTable*>(tableBytes);
        table.ReadOnly = !readOnly;
        g_expectedTable = &table;
        g_expectedReadOnly = readOnly;
        g_pathExists = true;
        g_eventCount = 0;
        g_file = 0;
        g_stream = 0;
        g_argumentsValid = true;
        g_flagsVisibleAtDestruction = false;
        g_recording = true;

        DirtyStack();
        table.LoadTable(readOnly);
        g_recording = false;

        static const Event expected[10] =
        {
            kPathExists,
            kBaseConstruct,
            kPathnameConstruct,
            kOpen,
            kStreamConstruct,
            kRead,
            kStreamDestroy,
            kClose,
            kPathnameDestroy,
            kBaseDestroy
        };
        return
            g_eventCount == 10 &&
            memcmp(g_events, expected, sizeof(expected)) == 0 &&
            g_argumentsValid &&
            g_flagsVisibleAtDestruction &&
            table.Locked &&
            table.ReadOnly == readOnly;
    }
}

int main()
{
    {
        // The real CDiskFileWin32 vftable, as any complete object carries it.
        CDiskFileWin32 reference;
        g_diskFileVtable = Vtable(&reference);
    }

    const bool passed =
        CheckMissingPath() &&
        CheckLoadedPath(false) &&
        CheckLoadedPath(true);
    if (!passed)
    {
        printf("FABLETLC_DEF_STRING_TABLE_LOAD_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_DEF_STRING_TABLE_LOAD_BEHAVIOR PASS\n");
    return 0;
}
