#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "fable_filesystem.h"
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_memory_file.h"
#include "fable_resource_directories.h"
#include "fable_sound_manager.h"
#include "test_support/base_class_stubs.h"

// The fixture links the candidate object and defines the engine members it
// calls as recorders: a fake directory scan, string concatenation, a memory
// file that captures what the sound manager writes through the writer, and
// the sound manager's lip-sync generator.

class CFileInformation
{
};

namespace
{
    const wchar_t* const* g_scanFiles;
    int g_scanIndex;
    int g_scanCalls;
    bool g_patternValid;
    long g_liveWide;

    int g_filesCreated;
    int g_filesDestroyed;
    int g_dumps;
    int g_generateCalls;
    bool g_argumentsValid;
    wchar_t g_created[4][64];
    wchar_t g_generated[4][64];
    char g_dumped[4][32];
    CMemoryFile* g_openFile;
    char g_payload[32];
    long g_payloadSize;

    wchar_t* Copy(const wchar_t* text)
    {
        const size_t length = wcslen(text);
        wchar_t* copy = static_cast<wchar_t*>(malloc((length + 1) * 2));
        memcpy(copy, text, (length + 1) * 2);
        return copy;
    }

    void Reset(const wchar_t* const* files)
    {
        g_scanFiles = files;
        g_scanIndex = 0;
        g_scanCalls = 0;
        g_patternValid = true;
        g_liveWide = 0;
        g_filesCreated = 0;
        g_filesDestroyed = 0;
        g_dumps = 0;
        g_generateCalls = 0;
        g_argumentsValid = true;
        memset(g_created, 0, sizeof(g_created));
        memset(g_generated, 0, sizeof(g_generated));
        memset(g_dumped, 0, sizeof(g_dumped));
        g_openFile = 0;
    }

    bool NextFile(
        const CWideString& pattern,
        CAFile::CFileScanInfo& info,
        wchar_t* pathname)
    {
        ++g_scanCalls;
        g_patternValid =
            g_patternValid &&
            wcscmp(pattern, L"Data\\Sound\\*.lug") == 0;
        if (g_scanFiles[g_scanIndex] == 0)
        {
            info.Stopped = true;
            return false;
        }
        wcscpy(pathname, g_scanFiles[g_scanIndex++]);
        info.Stopped = false;
        return true;
    }
}

CWideString::CWideString()
{
    ++g_liveWide;
    storage_ = 0;
}

CWideString::CWideString(const wchar_t* text)
{
    ++g_liveWide;
    storage_ = static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
    storage_->text = Copy(text);
}

// Only reached through the inline CMemoryFile::GetPathname in the vtable.
CWideString::CWideString(const CWideString& other)
{
    ++g_liveWide;
    storage_ = static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
    storage_->text = Copy(other);
}

CWideString::~CWideString()
{
    --g_liveWide;
}

CWideString::operator const wchar_t*() const
{
    return storage_ ? storage_->text : L"";
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right)
{
    wchar_t joined[128];
    wcscpy(joined, left);
    wcscat(joined, right);
    return CWideString(joined);
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right)
{
    wchar_t joined[128];
    wcscpy(joined, left);
    wcscat(joined, right);
    return CWideString(joined);
}

CWideString FABLE_FASTCALL NResourceDirectoryNames::GetSoundDir()
{
    return CWideString(L"Data\\Sound\\");
}

bool FABLE_FASTCALL CAFile::StartFileScan(
    const CWideString& pattern,
    CFileScanInfo& info,
    wchar_t* pathname)
{
    return NextFile(pattern, info, pathname);
}

bool FABLE_FASTCALL CAFile::ScanNextFile(
    const CWideString& pattern,
    CFileScanInfo& info,
    wchar_t* pathname)
{
    return NextFile(pattern, info, pathname);
}

// Splits "Data\\Sound\\name.ext" into "name" and ".ext".
void FABLE_FASTCALL CAFile::SplitPath(
    const CWideString& path,
    CWideString* drive,
    CWideString* directory,
    CWideString* filename,
    CWideString* extension)
{
    g_argumentsValid =
        g_argumentsValid &&
        drive == 0 && directory == 0 && filename != 0 && extension != 0;
    const wchar_t* text = path;
    const wchar_t* name = wcsrchr(text, L'\\');
    name = name ? name + 1 : text;
    const wchar_t* dot = wcsrchr(name, L'.');
    wchar_t stem[64];
    wcsncpy(stem, name, dot - name);
    stem[dot - name] = 0;
    // Written through the storage the way the real SplitPath fills outputs.
    CWideString stemString(stem);
    CWideString extensionString(dot);
    memcpy(filename, &stemString, sizeof(CWideString));
    memcpy(extension, &extensionString, sizeof(CWideString));
}

CBaseClassNonCopyable::CBaseClassNonCopyable()
{
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
}

CAFile::CAFile()
{
}



CFileInformation CAFile::GetFileInformation()
{
    return CFileInformation();
}

CMemoryFile::CMemoryFile(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    g_argumentsValid =
        g_argumentsValid &&
        accessType == 3 && flags == 2 && g_openFile == 0;
    wcscpy(g_created[g_filesCreated++ & 3], pathname);
    g_openFile = this;
    g_payloadSize = 0;
}

// ~CMemoryFile is inline (retail COMDAT 0x412020) and calls Close().
void CMemoryFile::Close()
{
    ++g_filesDestroyed;
    g_openFile = 0;
}

void CMemoryFile::Write(
    const void* data,
    long size,
    CASuspendableProcess* process)
{
    g_argumentsValid = g_argumentsValid && this == g_openFile && process == 0;
    memcpy(g_payload + g_payloadSize, data, size);
    g_payloadSize += size;
}

void CMemoryFile::DumpToFile()
{
    g_argumentsValid = g_argumentsValid && this == g_openFile;
    memcpy(g_dumped[g_dumps++ & 3], g_payload, g_payloadSize);
}

void CMemoryFile::Open(const CWideString&, FileAccessType, unsigned long) {}
void CMemoryFile::Read(void*, long, CASuspendableProcess*) {}
void CMemoryFile::SetPosition(long) {}
void CMemoryFile::MovePosition(long) {}
long CMemoryFile::GetPosition() const { return 0; }
void CMemoryFile::SetLength(long) {}
long CMemoryFile::GetLength() const { return 0; }
bool CMemoryFile::IsOpen() const { return true; }
CAFile* CMemoryFile::Clone() const { return 0; }

// The lip-sync generator writes the stem through the writer it is handed.
void CASoundManager::GenerateLipSyncData(
    const CWideString& lugPathname,
    LHA::CBinaryDataWriter& writer)
{
    wcscpy(g_generated[g_generateCalls++ & 3], lugPathname);
    const wchar_t* name = wcsrchr(lugPathname, L'\\') + 1;
    char narrow[16];
    int length = 0;
    while (name[length] != L'.')
    {
        narrow[length] = static_cast<char>(name[length]);
        ++length;
    }
    narrow[length++] = 0;
    g_argumentsValid = g_argumentsValid && writer.Write(narrow, length);
}

#define FABLE_UNKNOWN_SLOT(n) void CASoundManager::UnknownSlot##n() {}
FABLE_UNKNOWN_SLOT(01) FABLE_UNKNOWN_SLOT(02) FABLE_UNKNOWN_SLOT(03)
FABLE_UNKNOWN_SLOT(04) FABLE_UNKNOWN_SLOT(05) FABLE_UNKNOWN_SLOT(06)
FABLE_UNKNOWN_SLOT(07) FABLE_UNKNOWN_SLOT(08) FABLE_UNKNOWN_SLOT(09)
FABLE_UNKNOWN_SLOT(10) FABLE_UNKNOWN_SLOT(11) FABLE_UNKNOWN_SLOT(12)
FABLE_UNKNOWN_SLOT(13) FABLE_UNKNOWN_SLOT(14) FABLE_UNKNOWN_SLOT(15)
FABLE_UNKNOWN_SLOT(16) FABLE_UNKNOWN_SLOT(17) void CASoundManager::UnknownSlot18(long) {}
FABLE_UNKNOWN_SLOT(19) FABLE_UNKNOWN_SLOT(20) FABLE_UNKNOWN_SLOT(21)
FABLE_UNKNOWN_SLOT(22) FABLE_UNKNOWN_SLOT(23) FABLE_UNKNOWN_SLOT(24)
FABLE_UNKNOWN_SLOT(25) FABLE_UNKNOWN_SLOT(26) FABLE_UNKNOWN_SLOT(27)
FABLE_UNKNOWN_SLOT(28) FABLE_UNKNOWN_SLOT(29) FABLE_UNKNOWN_SLOT(30)
FABLE_UNKNOWN_SLOT(31) FABLE_UNKNOWN_SLOT(32) FABLE_UNKNOWN_SLOT(33)
FABLE_UNKNOWN_SLOT(34) FABLE_UNKNOWN_SLOT(35) FABLE_UNKNOWN_SLOT(36)
FABLE_UNKNOWN_SLOT(37) FABLE_UNKNOWN_SLOT(38) FABLE_UNKNOWN_SLOT(39)
FABLE_UNKNOWN_SLOT(40) FABLE_UNKNOWN_SLOT(41) FABLE_UNKNOWN_SLOT(42)
FABLE_UNKNOWN_SLOT(43) FABLE_UNKNOWN_SLOT(44) FABLE_UNKNOWN_SLOT(45)
FABLE_UNKNOWN_SLOT(48) FABLE_UNKNOWN_SLOT(50)
void CASoundManager::UnknownSlot46(unsigned long, float, float) {}
float CASoundManager::UnknownSlot47(unsigned long) { return 0.0f; }
bool CASoundManager::UnknownSlot49(float&) { return false; }

namespace
{
    CASoundManager* g_soundManager;
    // CSystemManager has string members whose constructors live elsewhere;
    // the leaf only needs the object's address and fields.
    fable_u32 g_systemManagerStorage[sizeof(CSystemManager) / 4];
    CSystemManager& g_systemManager =
        *reinterpret_cast<CSystemManager*>(g_systemManagerStorage);

    bool CheckNoFiles()
    {
        static const wchar_t* const files[] = { 0 };
        Reset(files);
        CMainGameComponent::GenerateMetFilesFromLugFiles();
        return
            g_patternValid &&
            g_argumentsValid &&
            g_scanCalls == 1 &&
            g_filesCreated == 0 &&
            g_generateCalls == 0 &&
            g_liveWide == 0;
    }

    bool CheckTwoFiles()
    {
        static const wchar_t* const files[] =
        {
            L"Data\\Sound\\hero.lug",
            L"Data\\Sound\\guildmaster.lug",
            0
        };
        Reset(files);
        CMainGameComponent::GenerateMetFilesFromLugFiles();
        return
            g_patternValid &&
            g_argumentsValid &&
            g_scanCalls == 3 &&
            g_filesCreated == 2 &&
            g_filesDestroyed == 2 &&
            g_generateCalls == 2 &&
            g_dumps == 2 &&
            wcscmp(g_created[0], L"Data\\Sound\\hero.met") == 0 &&
            wcscmp(g_created[1], L"Data\\Sound\\guildmaster.met") == 0 &&
            wcscmp(g_generated[0], L"Data\\Sound\\hero.lug") == 0 &&
            wcscmp(g_generated[1], L"Data\\Sound\\guildmaster.lug") == 0 &&
            strcmp(g_dumped[0], "hero") == 0 &&
            strcmp(g_dumped[1], "guildmaster") == 0 &&
            g_liveWide == 0;
    }
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    return &g_systemManager;
}

int main()
{
    g_soundManager = new CASoundManager;
    memset(&g_systemManager, 0, sizeof(g_systemManager));
    g_systemManager.PSoundManager.Data = g_soundManager;

    if (!CheckNoFiles() || !CheckTwoFiles())
    {
        printf(
            "scan=%d created=%d destroyed=%d generate=%d dumps=%d "
            "live=%ld pattern=%d args=%d\n",
            g_scanCalls,
            g_filesCreated,
            g_filesDestroyed,
            g_generateCalls,
            g_dumps,
            g_liveWide,
            g_patternValid ? 1 : 0,
            g_argumentsValid ? 1 : 0);
        puts("FABLETLC_GENERATE_MET_FILES_BEHAVIOR FAIL");
        return 1;
    }

    puts("FABLETLC_GENERATE_MET_FILES_BEHAVIOR PASS");
    return 0;
}
