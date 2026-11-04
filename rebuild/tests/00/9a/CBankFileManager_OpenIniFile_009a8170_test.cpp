#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "fable_data_input_stream.h"
#include "fable_memory_file.h"
#include "fable_retail_banks.h"
#include "test_support/base_class_stubs.h"
#include "test_support/data_input_stream_stubs.h"

// The fixture links the OpenIniFile object and defines the real engine
// members it calls as small working stand-ins: a memory file whose contents
// are a test INI, line streams over it, quoted-string reads, narrow/wide
// strings with live-instance counts, SplitPath, and the bank map's erase and
// operator[]. The checks read back the bank table and BasePath it builds.

class CFileInformation
{
};

namespace
{
    const char* g_iniText;
    const wchar_t* g_iniPathname;

    long g_liveChar;
    long g_liveWide;
    int g_fileBaseConstructs;
    int g_fileBaseDestroys;
    int g_opens;
    int g_closes;
    int g_fileStreams;
    int g_fileStreamDestroys;
    int g_lineStreams;
    int g_lineStreamDestroys;
    int g_trims;
    int g_overflowReads;
    int g_erases;
    void* g_erasedRoot;
    bool g_argumentsValid;
    CMemoryFile* g_file;

    const int kMaxEntries = 8;
    struct Entry
    {
        char key[32];
        fable_u8 info[sizeof(CBankFileManager::CBankInfo)];
    };
    Entry g_entries[kMaxEntries];
    int g_entryCount;

    void Check(bool condition)
    {
        if (!condition)
            g_argumentsValid = false;
    }

    wchar_t* CopyWide(const wchar_t* text)
    {
        const size_t length = wcslen(text);
        wchar_t* copy =
            static_cast<wchar_t*>(malloc((length + 1) * sizeof(wchar_t)));
        memcpy(copy, text, (length + 1) * sizeof(wchar_t));
        return copy;
    }

    CBankFileManager::CBankInfo* FindFixtureEntry(const char* key)
    {
        for (int i = 0; i < g_entryCount; ++i)
        {
            if (strcmp(g_entries[i].key, key) == 0)
                return reinterpret_cast<CBankFileManager::CBankInfo*>(
                    g_entries[i].info);
        }
        return 0;
    }

    bool Same(const CWideString& value, const wchar_t* expected)
    {
        return wcscmp(value, expected) == 0;
    }
}

// ---------------------------------------------------------------- strings

char g_FableEmptyNarrowString_0129AAF4[1] = "";

CCharString::CCharString()
{
    storage_ = 0;
    ++g_liveChar;
}

CCharString::CCharString(const char* text, long length)
{
    ++g_liveChar;
    storage_ = 0;
    if (text == 0 || text[0] == 0)
        return;
    if (length < 0)
        length = static_cast<long>(strlen(text));
    storage_ = static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));
    memset(storage_, 0, sizeof(CCharStringData));
    storage_->text = static_cast<char*>(malloc(length + 1));
    memcpy(storage_->text, text, length);
    storage_->text[length] = 0;
    storage_->unknown04 = length;
}

CCharString::~CCharString()
{
    --g_liveChar;
    if (storage_ != 0)
    {
        free(storage_->text);
        free(storage_);
    }
}

void CCharString::RemoveWhiteSpaceBeginEnd()
{
    ++g_trims;
    if (storage_ == 0)
        return;
    char* text = storage_->text;
    long length = storage_->unknown04;
    long first = 0;
    while (first < length && (text[first] == ' ' || text[first] == '\t'))
        ++first;
    while (length > first &&
           (text[length - 1] == ' ' || text[length - 1] == '\t'))
        --length;
    memmove(text, text + first, length - first);
    text[length - first] = 0;
    storage_->unknown04 = length - first;
}

CWideString CCharString::ToWideString() const
{
    const char* text = GetConstCharPtr();
    wchar_t buffer[128];
    size_t i = 0;
    for (; text[i] != 0 && i < 127; ++i)
        buffer[i] = static_cast<unsigned char>(text[i]);
    buffer[i] = 0;
    return CWideString(buffer);
}

CWideString::CWideString()
{
    storage_ = 0;
    ++g_liveWide;
}

CWideString::CWideString(const wchar_t* text)
{
    ++g_liveWide;
    storage_ = 0;
    *this = text;
}

CWideString::CWideString(const CWideString& other)
{
    ++g_liveWide;
    storage_ = 0;
    *this = other;
}

CWideString::~CWideString()
{
    --g_liveWide;
    UnassignString();
}

void CWideString::UnassignString()
{
    if (storage_ != 0)
    {
        free(storage_->text);
        free(storage_);
        storage_ = 0;
    }
}

CWideString::operator const wchar_t*() const
{
    return storage_ == 0 ? L"" : storage_->text;
}

const CWideString& CWideString::operator=(const wchar_t* text)
{
    wchar_t* copy = CopyWide(text);
    UnassignString();
    if (copy[0] == 0)
    {
        free(copy);
        return *this;
    }
    storage_ = static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
    memset(storage_, 0, sizeof(CWideStringData));
    storage_->text = copy;
    storage_->unknown04 =
        static_cast<fable_u32>(reinterpret_cast<size_t>(copy + wcslen(copy)));
    return *this;
}

CWideString& CWideString::operator=(const CWideString& other)
{
    *this = static_cast<const wchar_t*>(other);
    return *this;
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right)
{
    wchar_t buffer[256];
    wcscpy(buffer, left);
    wcscat(buffer, right);
    return CWideString(buffer);
}

// ------------------------------------------------------------------ files

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { if (this == g_file) ++g_fileBaseDestroys; }

CAFile::CAFile()
{
    ++g_fileBaseConstructs;
}



CFileInformation CAFile::GetFileInformation()
{
    return CFileInformation();
}

void FABLE_FASTCALL CAFile::SplitPath(
    const CWideString& path,
    CWideString* drive,
    CWideString* directory,
    CWideString* filename,
    CWideString* extension)
{
    Check(drive == 0 && filename == 0 && extension == 0 && directory != 0);
    wchar_t buffer[128];
    wcscpy(buffer, path);
    wchar_t* slash = wcsrchr(buffer, L'\\');
    if (slash != 0)
        slash[1] = 0;
    else
        buffer[0] = 0;
    *directory = buffer;
}

void CMemoryFile::Open(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    ++g_opens;
    g_file = this;
    Check(accessType == 1 && flags == 2 && !Opened);
    Check(DataBuffer._M_start == 0 && DataBuffer._M_finish == 0 &&
          DataBuffer._M_end_of_storage._M_data == 0);
    Pathname = pathname;
    // Freed by the inline ~CMemoryFile.
    DataBuffer._M_start = static_cast<unsigned char*>(malloc(16));
    Opened = true;
}

void CMemoryFile::Close()
{
    ++g_closes;
    Check(this == g_file && Opened);
    Opened = false;
}

void CMemoryFile::Read(void*, long, CASuspendableProcess*) {}
void CMemoryFile::Write(const void*, long, CASuspendableProcess*) {}
void CMemoryFile::SetPosition(long) {}
void CMemoryFile::MovePosition(long) {}
long CMemoryFile::GetPosition() const { return 0; }
void CMemoryFile::SetLength(long) {}
long CMemoryFile::GetLength() const { return 0; }
bool CMemoryFile::IsOpen() const { return Opened; }
CAFile* CMemoryFile::Clone() const { return 0; }

// ---------------------------------------------------------------- streams


void CDataInputStream::SetPosition(unsigned long position)
{
    CurrentSourcePtr -= StreamPos - position;
    SourceChunkBytesRemaining += static_cast<long>(StreamPos - position);
    StreamPos = position;
}

// The base stream constructor is out of line in retail (0x00993B20); the
// stream stub below sets every field itself.
CDataInputStream::CDataInputStream() {}

// The file stream reads the test INI text; each line stream reads its line.
CFileDataInputStream::CFileDataInputStream(
    CAFile* file,
    unsigned long bufferSize)
{
    ++g_fileStreams;
    Check(file == g_file && bufferSize == 0x4000);
    File = file;
    Buffer = 0;
    BufferSize = static_cast<long>(bufferSize);
    StreamPos = 0;
    StreamSize = static_cast<long>(strlen(g_iniText));
    CurrentSourcePtr = reinterpret_cast<fable_u8*>(const_cast<char*>(g_iniText));
    SourceChunkStreamPos = 0;
    SourceChunkBytesRemaining = StreamSize;
}

CFileDataInputStream::~CFileDataInputStream()
{
    ++g_fileStreamDestroys;
}

CCharString CDataInputStream::ReadLine()
{
    const char* text = reinterpret_cast<const char*>(CurrentSourcePtr);
    long length = 0;
    while (StreamPos + length < static_cast<unsigned long>(StreamSize) &&
           text[length] != '\n')
        ++length;
    long consumed = length;
    if (StreamPos + consumed < static_cast<unsigned long>(StreamSize))
        ++consumed;
    StreamPos += consumed;
    CurrentSourcePtr += consumed;
    SourceChunkBytesRemaining -= consumed;
    return CCharString(text, length);
}

CMemoryDataInputStream::CMemoryDataInputStream(const CCharString& source)
{
    ++g_lineStreams;
    const char* text = source.GetConstCharPtr();
    const long length = static_cast<long>(strlen(text));
    StreamSource = static_cast<fable_u8*>(malloc(length + 1));
    memcpy(StreamSource, text, length + 1);
    StreamPos = 0;
    StreamSize = length;
    CurrentSourcePtr = StreamSource;
    SourceChunkStreamPos = 0;
    SourceChunkBytesRemaining = length;
    // A "Chunked" line ends its first source chunk at the '=', so the byte
    // after the bank name must come through ReadWithSrcChunkOverflow.
    if (strncmp(text, "Chunked", 7) == 0)
        SourceChunkBytesRemaining =
            static_cast<long>(strchr(text, '=') - text);
}

CMemoryDataInputStream::~CMemoryDataInputStream()
{
    ++g_lineStreamDestroys;
    free(StreamSource);
}

void CDataInputStream::ReadWithSrcChunkOverflow(void* data, long size)
{
    ++g_overflowReads;
    Check(SourceChunkBytesRemaining == 0 && size == 1);
    memcpy(data, CurrentSourcePtr, size);
    CurrentSourcePtr += size;
    StreamPos += size;
    SourceChunkBytesRemaining = StreamSize - static_cast<long>(StreamPos);
}

CCharString CDataInputStream::ReadUntilCharacter(char character, bool& found)
{
    const char* text = reinterpret_cast<const char*>(CurrentSourcePtr);
    long length = 0;
    while (StreamPos + length < static_cast<unsigned long>(StreamSize) &&
           text[length] != character)
        ++length;
    found = StreamPos + length < static_cast<unsigned long>(StreamSize);
    StreamPos += length;
    CurrentSourcePtr += length;
    SourceChunkBytesRemaining -= length;
    long end = length;
    while (end > 0 && text[end - 1] == ' ')
        --end;
    return CCharString(text, end);
}

CCharString CDataInputStream::ReadUntilWhiteSpace()
{
    const char* text = reinterpret_cast<const char*>(CurrentSourcePtr);
    long length = 0;
    while (StreamPos + length < static_cast<unsigned long>(StreamSize) &&
           text[length] != ' ')
        ++length;
    StreamPos += length;
    CurrentSourcePtr += length;
    SourceChunkBytesRemaining -= length;
    return CCharString(text, length);
}

bool CBankFileManager::ReadQuotedString(
    CCharString& text,
    CDataInputStream& stream)
{
    const char* cursor = reinterpret_cast<const char*>(stream.CurrentSourcePtr);
    const char* end = cursor + (stream.StreamSize - stream.StreamPos);
    while (cursor < end && *cursor == ' ')
        ++cursor;
    if (cursor >= end || *cursor != '"')
        return false;
    const char* close = static_cast<const char*>(
        memchr(cursor + 1, '"', end - cursor - 1));
    if (close == 0)
        return false;
    text.~CCharString();
    new (&text) CCharString(cursor + 1, static_cast<long>(close - cursor - 1));
    const long consumed = static_cast<long>(
        close + 1 - reinterpret_cast<const char*>(stream.CurrentSourcePtr));
    stream.StreamPos += consumed;
    stream.CurrentSourcePtr += consumed;
    stream.SourceChunkBytesRemaining -= consumed;
    return true;
}

// -------------------------------------------------------------- bank map

void CBankFileManager::CBankInfoMap::Erase(CNode* node)
{
    ++g_erases;
    g_erasedRoot = node;
    g_entryCount = 0;
}

CBankFileManager::CBankInfo&
CBankFileManager::CBankInfoMap::operator[](const CCharString& key)
{
    CBankInfo* info = FindFixtureEntry(key.GetConstCharPtr());
    if (info != 0)
        return *info;
    Entry& entry = g_entries[g_entryCount++];
    strcpy(entry.key, key.GetConstCharPtr());
    // Null CWideStrings are empty; a stale RegisteredBank must be overwritten.
    memset(entry.info, 0, sizeof(entry.info));
    info = reinterpret_cast<CBankInfo*>(entry.info);
    info->RegisteredBank = reinterpret_cast<CBankFile*>(0xDEAD);
    ++size_;
    return *info;
}

// ------------------------------------------------------------------ tests

namespace
{
    struct Manager
    {
        fable_u8 bytes[sizeof(CBankFileManager)];
        CBankFileManager::CBankInfoMap::CNode head;

        CBankFileManager* Get()
        {
            return reinterpret_cast<CBankFileManager*>(bytes);
        }
    };

    void Reset(const char* text)
    {
        g_iniText = text;
        g_liveChar = 0;
        g_liveWide = 0;
        g_fileBaseConstructs = 0;
        g_fileBaseDestroys = 0;
        g_opens = 0;
        g_closes = 0;
        g_fileStreams = 0;
        g_fileStreamDestroys = 0;
        g_lineStreams = 0;
        g_lineStreamDestroys = 0;
        g_trims = 0;
        g_overflowReads = 0;
        g_erases = 0;
        g_erasedRoot = 0;
        g_argumentsValid = true;
        g_file = 0;
    }

    bool Lifetimes(int lines)
    {
        return
            g_liveChar == 0 &&
            g_liveWide == 0 &&
            g_fileBaseConstructs == 1 &&
            g_fileBaseDestroys == 1 &&
            g_opens == 1 &&
            g_closes == 1 &&
            g_fileStreams == 1 &&
            g_fileStreamDestroys == 1 &&
            g_lineStreams == lines &&
            g_lineStreamDestroys == lines &&
            g_trims == lines;
    }

    bool Bank(
        const char* key,
        const wchar_t* dev,
        const wchar_t* retail,
        const wchar_t* devHeader,
        const wchar_t* retailHeader)
    {
        CBankFileManager::CBankInfo* info = FindFixtureEntry(key);
        return
            info != 0 &&
            Same(info->DevBankFilePath, dev) &&
            Same(info->RetailBankFilePath, retail) &&
            Same(info->DevHeaderFilePath, devHeader) &&
            Same(info->RetailHeaderFilePath, retailHeader) &&
            info->RegisteredBank == 0;
    }

    // An empty map is not erased; an empty INI only sets BasePath.
    bool CheckEmpty()
    {
        Reset("");
        Manager manager;
        memset(&manager, 0, sizeof(manager));
        CBankFileManager* banks = manager.Get();
        banks->Banks.head_ = &manager.head;
        manager.head.parent_ = reinterpret_cast<CBankFileManager::
            CBankInfoMap::CNode*>(0x1234);

        CWideString pathname(L"Data\\Banks\\empty.ini");
        g_liveWide = 0;
        banks->OpenIniFile(pathname);

        const bool ok =
            g_argumentsValid &&
            Lifetimes(0) &&
            g_erases == 0 &&
            manager.head.parent_ == reinterpret_cast<CBankFileManager::
                CBankInfoMap::CNode*>(0x1234) &&
            banks->Banks.size_ == 0 &&
            Same(banks->BasePath(), L"Data\\Banks\\");
        return ok;
    }

    bool CheckParse()
    {
        static const char kIni[] =
            "  BeginRetailFile \"Data\\Retail\\\"  \n"
            "Frontend = \"Data\\Frontend.big\" \"frontend.bnk\"\n"
            "devheaderdirectory \"Headers\\Dev\\\"\n"
            "RetailHeaderDirectory \"Headers\\Retail\\\"\n"
            "Textures = \"Data\\Textures.big\" \"textures.bnk\"\n"
            "Chunked = \"Data\\Chunked.big\" \"chunked.bnk\"\n"
            "NoHeader = \"Data\\NoHeader.big\"\n"
            "BadLine = unquoted\n"
            "Unknown \"ignored\"\n"
            "EndRetailFile\n"
            "Late = \"Data\\Late.big\" \"\"\n"
            "Frontend = \"Data\\Frontend2.big\" \"frontend.bnk\"";
        Reset(kIni);
        Manager manager;
        memset(&manager, 0, sizeof(manager));
        CBankFileManager* banks = manager.Get();
        banks->Banks.head_ = &manager.head;
        banks->Banks.size_ = 3;
        manager.head.parent_ = reinterpret_cast<CBankFileManager::
            CBankInfoMap::CNode*>(0x2468);
        manager.head.left_ = 0;
        manager.head.right_ = 0;
        g_entryCount = 1;
        strcpy(g_entries[0].key, "Stale");

        CWideString pathname(L"Data\\Banks\\banks.ini");
        g_liveWide = 0;
        banks->OpenIniFile(pathname);

        return
            g_argumentsValid &&
            Lifetimes(12) &&
            g_overflowReads == 1 &&
            g_erases == 1 &&
            g_erasedRoot == reinterpret_cast<void*>(0x2468) &&
            manager.head.parent_ == 0 &&
            manager.head.left_ == &manager.head &&
            manager.head.right_ == &manager.head &&
            g_entryCount == 5 &&
            banks->Banks.size_ == 5 &&
            FindFixtureEntry("Stale") == 0 &&
            FindFixtureEntry("BadLine") == 0 &&
            FindFixtureEntry("Unknown") == 0 &&
            Bank("Frontend", L"Data\\Frontend2.big", L"",
                 L"Headers\\Dev\\frontend.bnk",
                 L"Headers\\Retail\\frontend.bnk") &&
            Bank("Textures", L"Data\\Textures.big", L"Data\\Retail\\",
                 L"Headers\\Dev\\textures.bnk",
                 L"Headers\\Retail\\textures.bnk") &&
            Bank("Chunked", L"Data\\Chunked.big", L"Data\\Retail\\",
                 L"Headers\\Dev\\chunked.bnk",
                 L"Headers\\Retail\\chunked.bnk") &&
            Bank("NoHeader", L"Data\\NoHeader.big", L"Data\\Retail\\",
                 L"", L"") &&
            Bank("Late", L"Data\\Late.big", L"", L"", L"") &&
            Same(banks->BasePath(), L"Data\\Banks\\");
    }

    // Before any header directory is set, the header names are used bare.
    bool CheckHeaderBeforeDirectories()
    {
        static const char kIni[] =
            "Frontend = \"Data\\Frontend.big\" \"frontend.bnk\"\n";
        Reset(kIni);
        Manager manager;
        memset(&manager, 0, sizeof(manager));
        CBankFileManager* banks = manager.Get();
        banks->Banks.head_ = &manager.head;
        g_entryCount = 0;

        CWideString pathname(L"banks.ini");
        g_liveWide = 0;
        banks->OpenIniFile(pathname);

        return
            g_argumentsValid &&
            Lifetimes(1) &&
            g_overflowReads == 0 &&
            g_entryCount == 1 &&
            Bank("Frontend", L"Data\\Frontend.big", L"", L"frontend.bnk",
                 L"frontend.bnk") &&
            Same(banks->BasePath(), L"");
    }
}

int main()
{
    const bool empty = CheckEmpty();
    const bool parse = CheckParse();
    const bool bare = CheckHeaderBeforeDirectories();
    if (!empty || !parse || !bare)
    {
        printf(
            "FABLETLC_OPEN_INI_FILE FAIL empty=%d parse=%d bare=%d "
            "args=%d char=%ld wide=%ld file=%d/%d open=%d/%d "
            "stream=%d/%d lines=%d/%d trims=%d overflow=%d erase=%d "
            "entries=%d\n",
            empty, parse, bare, g_argumentsValid, g_liveChar, g_liveWide,
            g_fileBaseConstructs, g_fileBaseDestroys, g_opens, g_closes,
            g_fileStreams, g_fileStreamDestroys, g_lineStreams,
            g_lineStreamDestroys, g_trims, g_overflowReads, g_erases,
            g_entryCount);
        return 1;
    }

    printf("FABLETLC_OPEN_INI_FILE PASS\n");
    return 0;
}
