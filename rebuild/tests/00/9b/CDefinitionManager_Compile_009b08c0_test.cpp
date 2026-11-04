#include <stdio.h>
#include <string.h>

#include "fable_definition_manager.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
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
        kTime,
        kCheckDependencies,
        kPathExists,
        kFileConstruct,
        kPathnameConstruct,
        kOpenRead,
        kOpenCreate,
        kInputConstruct,
        kLoad,
        kInputDestroy,
        kClose,
        kPathnameDestroy,
        kFileDestroy,
        kCreateSymbols,
        kPrefixConstruct,
        kCompilePathList,
        kPrefixDestroy,
        kRemoveUnusedMemory,
        kOutputConstruct,
        kSave,
        kGetLength,
        kPeekData,
        kWrite,
        kOutputDestroy
    };

    const unsigned int kMaxEvents = 64;
    Event g_events[kMaxEvents];
    unsigned int g_eventCount;
    bool g_recording;
    bool g_valid;
    bool g_pathExists;
    bool g_loadResult;
    CDefinitionManager* g_manager;
    void* g_file;
    void* g_input;
    void* g_output;
    void* g_prefix;
    void* g_diskFileVtable;
    char g_outputData[4];

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
            g_valid = false;
    }

    void* Vtable(const void* object)
    {
        return *static_cast<void* const*>(object);
    }
}

// Protected member addresses, through a derived view.
struct ManagerView : CDefinitionManager
{
    static const void* Symbols(CDefinitionManager* m) { return &static_cast<ManagerView*>(m)->SymbolPathList; }
    static const void* Sources(CDefinitionManager* m) { return &static_cast<ManagerView*>(m)->CompilePathList; }
    static const void* FileName(CDefinitionManager* m) { return &static_cast<ManagerView*>(m)->CompiledFileName; }
    static void Switches(bool usingCompiled, bool creating)
    {
        UsingCompiledDefs = usingCompiled;
        CreatingCompiledDefs = creating;
    }
};

bool CDefinitionManager::UsingCompiledDefs;

double FABLE_FASTCALL GFGetTime()
{
    Record(kTime);
    return 1.0;
}

void CDefinitionManager::CheckDependencies(const _STL::vector<CWideString>& pathList)
{
    Record(kCheckDependencies);
    Check(this == g_manager && &pathList == ManagerView::Symbols(this));
}

bool CDefinitionManager::LoadBinaryDefinitions(CDataInputStream& stream)
{
    Record(kLoad);
    Check(this == g_manager && &stream == g_input);
    return g_loadResult;
}

void CDefinitionManager::SaveBinaryDefinitions(CDataOutputStream& stream)
{
    Record(kSave);
    Check(this == g_manager && &stream == g_output);
}

void CDefinitionManager::CreateSymbolsFromPathList(const _STL::vector<CWideString>& pathList, bool flag)
{
    Record(kCreateSymbols);
    Check(this == g_manager && &pathList == ManagerView::Symbols(this) && flag);
}

void CDefinitionManager::DoCompilePathList(const _STL::vector<CWideString>& pathList, const CCharString& prefix)
{
    Record(kCompilePathList);
    Check(this == g_manager && &pathList == ManagerView::Sources(this) && &prefix == g_prefix);
}

void CDefinitionManager::RemoveUnusedMemory()
{
    Record(kRemoveUnusedMemory);
    Check(this == g_manager);
}

CCharString::CCharString(const char* text, long length)
{
    // Only the empty compile prefix is built with a recording on; the output
    // stream's base builds its "\r\n" newline through here too.
    if (text[0] == 0)
    {
        Record(kPrefixConstruct);
        Check(length == -1);
        g_prefix = this;
    }
}

CCharString::~CCharString()
{
    if (this == g_prefix)
        Record(kPrefixDestroy);
}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { if (this == g_file) Record(kFileDestroy); }

CAFile::CAFile()
{
    Record(kFileConstruct);
    g_file = this;
}



CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }

bool FABLE_FASTCALL CAFile::PathExists(const CWideString& pathname)
{
    Record(kPathExists);
    Check(&pathname == ManagerView::FileName(g_manager));
    return g_pathExists;
}

CWideString::CWideString() : storage_(0) { Record(kPathnameConstruct); }
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() { Record(kPathnameDestroy); }

void CDiskFileWin32::Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    Record(accessType == FILE_ACCESS_READ ? kOpenRead : kOpenCreate);
    Check(this == g_file && Vtable(this) == g_diskFileVtable && !IsOpen());
    Check(&pathname == ManagerView::FileName(g_manager));
    Check((accessType == FILE_ACCESS_READ || accessType == FILE_ACCESS_CREATE) && flags == 4);
    Opened = true;
}

void CDiskFileWin32::Close()
{
    Record(kClose);
    Check(this == g_file && Vtable(this) == g_diskFileVtable);
    Opened = false;
}

void CDiskFileWin32::Write(const void* data, long size, CASuspendableProcess* process)
{
    Record(kWrite);
    Check(this == g_file && IsOpen() && data == g_outputData && size == 3 && process == 0);
}

void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { return CFileInformation(); }

CDataInputStream::CDataInputStream() {}
void CDataInputStream::SetPosition(unsigned long) {}

CFileDataInputStream::CFileDataInputStream(CAFile* file, unsigned long bufferSize)
{
    Record(kInputConstruct);
    g_input = this;
    Check(file == g_file && bufferSize == 0x4000 && static_cast<CDiskFileWin32*>(file)->IsOpen());
    File = file;
}

CFileDataInputStream::~CFileDataInputStream()
{
    Record(kInputDestroy);
    Check(this == g_input);
}



CMemoryDataOutputStream::CMemoryDataOutputStream()
{
    Record(kOutputConstruct);
    g_output = this;
}

CMemoryDataOutputStream::~CMemoryDataOutputStream()
{
    Record(kOutputDestroy);
    Check(this == g_output);
}

void CMemoryDataOutputStream::SetPosition(unsigned long) {}
unsigned long CMemoryDataOutputStream::GetPosition() const { return 0; }
void CMemoryDataOutputStream::Close() {}
void CMemoryDataOutputStream::Write(const void*, long) {}

unsigned long CMemoryDataOutputStream::GetLength() const
{
    Record(kGetLength);
    Check(this == g_output);
    return 3;
}

const void* CMemoryDataOutputStream::PeekData() const
{
    Record(kPeekData);
    Check(this == g_output);
    return g_outputData;
}

namespace
{
    bool Run(bool usingCompiled, bool pathExists, bool loadResult, bool creating,
             const Event* expected, unsigned int count)
    {
        static fable_u8 managerBytes[sizeof(CDefinitionManager)];
        memset(managerBytes, 0xA5, sizeof(managerBytes));
        g_manager = reinterpret_cast<CDefinitionManager*>(managerBytes);
        ManagerView::Switches(usingCompiled, creating);
        g_pathExists = pathExists;
        g_loadResult = loadResult;
        g_eventCount = 0;
        g_valid = true;
        g_file = g_input = g_output = g_prefix = 0;
        g_recording = true;
        g_manager->Compile();
        g_recording = false;
        return g_valid && g_eventCount == count &&
               memcmp(g_events, expected, count * sizeof(Event)) == 0;
    }

    // Compile from the path lists: symbols, sources with an empty prefix, trim.
#define COMPILE_STEPS \
    kTime, kCreateSymbols, kPrefixConstruct, kCompilePathList, kPrefixDestroy, kTime, kRemoveUnusedMemory
#define OPEN_COMPILED \
    kPathExists, kFileConstruct, kPathnameConstruct, kOpenRead, kInputConstruct, kTime, kLoad
#define CLOSE_COMPILED \
    kInputDestroy, kClose, kPathnameDestroy, kFileDestroy
}

int main()
{
    {
        CDiskFileWin32 reference;
        g_diskFileVtable = Vtable(&reference);
    }

    static const Event textOnly[] = { kTime, kCheckDependencies, kTime, COMPILE_STEPS };
    static const Event noFile[] = { kTime, kCheckDependencies, kTime, kPathExists, COMPILE_STEPS };
    static const Event loaded[] = { kTime, kCheckDependencies, kTime, OPEN_COMPILED, kTime, CLOSE_COMPILED };
    static const Event loadFailed[] = { kTime, kCheckDependencies, kTime, OPEN_COMPILED, CLOSE_COMPILED, COMPILE_STEPS };
    static const Event created[] = { kTime, kCheckDependencies, kTime, COMPILE_STEPS,
        kFileConstruct, kPathnameConstruct, kOpenCreate, kOutputConstruct, kTime, kSave,
        kGetLength, kPeekData, kWrite, kTime, kClose, kOutputDestroy, kClose, kPathnameDestroy, kFileDestroy };

    const bool passed =
        Run(false, true, true, false, textOnly, sizeof(textOnly) / sizeof(Event)) &&
        Run(true, false, true, false, noFile, sizeof(noFile) / sizeof(Event)) &&
        Run(true, true, true, false, loaded, sizeof(loaded) / sizeof(Event)) &&
        Run(true, true, true, true, loaded, sizeof(loaded) / sizeof(Event)) &&
        Run(true, true, false, false, loadFailed, sizeof(loadFailed) / sizeof(Event)) &&
        Run(false, true, true, true, created, sizeof(created) / sizeof(Event));
    if (!passed)
    {
        printf("FABLETLC_CDEFINITIONMANAGER_COMPILE_009B08C0 FAIL (%u events)\n", g_eventCount);
        return 1;
    }
    printf("FABLETLC_CDEFINITIONMANAGER_COMPILE_009B08C0_PASS\n");
    return 0;
}