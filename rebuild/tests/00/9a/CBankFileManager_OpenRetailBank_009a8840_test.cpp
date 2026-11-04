#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_data_input_stream.h"
#include "fable_disk_file.h"
#include "fable_retail_banks.h"
#include "fable_stlport_vector.h"
#include "test_support/base_class_stubs.h"
#define FABLE_STUBS_NO_MEMORY_DATA_INPUT_STREAM
#include "test_support/data_input_stream_stubs.h"

class CFileInformation
{
};

// Behaviour fixture for CBankFileManager::OpenRetailBank @ 0x009A8840.
// The real engine members the leaf calls are defined here as recorders over
// an in-memory BIGB image; the counted-pointer and list members are the
// genuine header code, observed through their reference counts.
namespace
{
    enum Event
    {
        kPathExists,
        kDiskConstruct,
        kDiskOpen,
        kStreamConstruct,
        kStreamOverflow,
        kStreamDestroy,
        kDiskClose,
        kDiskDelete,
        kBankConstruct,
        kToCharString,
        kVectorConstruct,
        kSetPosition,
        kMemoryConstruct,
        kReadName,
        kMapIndex,
        kMemoryDestroy,
        kThreadedConstruct,
        kThreadedReset,
        kThreadedOpen,
        kBankDelete
    };

    const unsigned int kMaxEvents = 128;
    Event g_events[kMaxEvents];
    unsigned int g_eventCount;
    bool g_ok;

    void Record(Event event)
    {
        if (g_eventCount < kMaxEvents)
            g_events[g_eventCount] = event;
        ++g_eventCount;
    }

    void Check(bool condition)
    {
        if (!condition)
            g_ok = false;
    }

    unsigned int Count(Event event)
    {
        unsigned int count = 0;
        for (unsigned int i = 0; i < g_eventCount && i < kMaxEvents; ++i)
            if (g_events[i] == event)
                ++count;
        return count;
    }

    // Index of the n-th occurrence of an event, or kMaxEvents.
    unsigned int At(Event event, unsigned int nth = 0)
    {
        for (unsigned int i = 0; i < g_eventCount && i < kMaxEvents; ++i)
            if (g_events[i] == event && nth-- == 0)
                return i;
        return kMaxEvents;
    }

    // The bank image the "disk file" serves, and the window a source chunk
    // exposes before ReadWithSrcChunkOverflow has to take over.
    fable_u8 g_image[256];
    unsigned long g_imageSize;
    long g_window;

    bool g_pathExists;
    const CWideString* g_pathname;
    CBankFileManager* g_manager;
    CDiskFileWin32* g_disk;
    CFileDataInputStream* g_fileStream;
    CMemoryDataInputStream* g_memoryStream;
    NBankFileManager::CRetailBank* g_bank;
    CThreadedFile* g_threaded;
    fable_u8* g_vectorData;
    unsigned long g_vectorSize;
    unsigned long g_setPosition;
    unsigned long g_memorySize;
    bool g_useDmaAtOpen;
    bool g_threadedNoCaching;

    // Interned contained-bank names (the header buffer dies with the call).
    char g_names[8][16];
    unsigned int g_nameCount;
    struct MapEntry
    {
        const char* name;
        NBankFileManager::CContainedBank value;
    };
    MapEntry g_map[8];
    unsigned int g_mapCount;

    const char g_pathToken[] = "bank path";

    const char* Intern(const char* text)
    {
        for (unsigned int i = 0; i < g_nameCount; ++i)
            if (strcmp(g_names[i], text) == 0)
                return g_names[i];
        strcpy(g_names[g_nameCount], text);
        return g_names[g_nameCount++];
    }

    void Refill(CDataInputStream& stream, const fable_u8* source, long size)
    {
        long left = size - (long)stream.StreamPos;
        stream.CurrentSourcePtr =
            const_cast<fable_u8*>(source) + stream.StreamPos;
        stream.SourceChunkStreamPos = stream.StreamPos;
        stream.SourceChunkBytesRemaining = left < g_window ? left : g_window;
    }

    const fable_u8* SourceOf(const CDataInputStream* stream, long* size)
    {
        if (stream == g_fileStream)
        {
            *size = (long)g_imageSize;
            return g_image;
        }
        *size = g_memorySize;
        return g_memoryStream->StreamSource;
    }
}

// ------------------------------------------------------------------ base
CBaseClassNonCopyable::CBaseClassNonCopyable()
{
    // The disk's empty CAFile constructor is inlined in the recovered root.
    // Observe its first base construction at the actual retail call boundary.
    if (!g_disk) {
        Record(kDiskConstruct);
        g_disk = reinterpret_cast<CDiskFileWin32*>(this);
    }
}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}


CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }

bool FABLE_FASTCALL CAFile::PathExists(const CWideString& pathname)
{
    Record(kPathExists);
    Check(&pathname == g_pathname);
    return g_pathExists;
}

// ------------------------------------------------------------------ strings
CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() {}

CCharString CWideString::ToCharString() const
{
    Record(kToCharString);
    Check(this == g_pathname);
    return CCharString(g_pathToken, -1);
}

// Storage holds the interned text pointer itself.
CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const char* text, long)
    : storage_(reinterpret_cast<CCharStringData*>(const_cast<char*>(text)))
{
}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) {}
CCharString::~CCharString() { storage_ = 0; }
CCharString& CCharString::operator=(const CCharString& other)
{
    storage_ = other.storage_;
    return *this;
}
CCharString::operator const char*() const
{
    return reinterpret_cast<const char*>(storage_);
}

// ------------------------------------------------------------------ disk file
void CDiskFileWin32::Open(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    Record(kDiskOpen);
    Check(this == g_disk);
    Check(!Opened);
    Check(&pathname == g_pathname);
    Check(accessType == 1);
    Check(flags == 2);
    Opened = true;
}

void CDiskFileWin32::Close()
{
    Record(kDiskClose);
    Check(this == g_disk);
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

void FABLE_FASTCALL CCountedPointer<CDiskFileWin32>::DeleteData(void* data)
{
    Record(kDiskDelete);
    Check(data == g_disk);
    delete static_cast<CDiskFileWin32*>(data);
}

// ------------------------------------------------------------------ streams

void CDataInputStream::SetPosition(unsigned long position)
{
    Record(kSetPosition);
    Check(this == g_fileStream);
    g_setPosition = position;
    StreamPos = position;
    long size;
    const fable_u8* source = SourceOf(this, &size);
    Refill(*this, source, size);
}

void CDataInputStream::ReadWithSrcChunkOverflow(void* data, long size)
{
    Record(kStreamOverflow);
    long total;
    const fable_u8* source = SourceOf(this, &total);
    Check(size > SourceChunkBytesRemaining);
    Check((long)StreamPos + size <= total);
    memcpy(data, source + StreamPos, size);
    StreamPos += size;
    Refill(*this, source, total);
}

// The base stream constructor is out of line in retail (0x00993B20); the
// stream stub below sets every field itself.
CDataInputStream::CDataInputStream() {}

CFileDataInputStream::CFileDataInputStream(
    CAFile* file,
    unsigned long bufferSize)
{
    Record(kStreamConstruct);
    g_fileStream = this;
    Check(file == g_disk);
    Check(g_disk != 0 && g_disk->IsOpen());
    Check(bufferSize == 0x4000);
    File = file;
    Buffer = 0;
    BufferSize = (long)bufferSize;
    StreamPos = 0;
    StreamSize = (long)g_imageSize;
    Refill(*this, g_image, (long)g_imageSize);
}

CFileDataInputStream::~CFileDataInputStream()
{
    Record(kStreamDestroy);
    Check(this == g_fileStream);
    g_fileStream = 0;
}

CMemoryDataInputStream::CMemoryDataInputStream(
    const void* data,
    unsigned long size)
{
    Record(kMemoryConstruct);
    g_memoryStream = this;
    g_memorySize = size;
    Check(data == g_vectorData);
    Check(size == g_vectorSize);
    StreamSource = static_cast<fable_u8*>(const_cast<void*>(data));
    StreamPos = 0;
    StreamSize = (long)size;
    Refill(*this, StreamSource, (long)size);
}

CMemoryDataInputStream::~CMemoryDataInputStream()
{
    Record(kMemoryDestroy);
    Check(this == g_memoryStream);
    g_memoryStream = 0;
}

CCharString CMemoryDataInputStream::ReadNullTerminatedString()
{
    Record(kReadName);
    Check(this == g_memoryStream);
    const char* text =
        reinterpret_cast<const char*>(StreamSource) + StreamPos;
    const long consumed = (long)strlen(text) + 1;
    CCharString name(Intern(text), -1);
    StreamPos += consumed;
    Refill(*this, StreamSource, g_memorySize);
    return name;
}

template<>
_STL::vector<fable_u8>::vector(size_t n)
    : _Vector_base<fable_u8, _STL::allocator<fable_u8> >(
          _STL::allocator<fable_u8>())
{
    Record(kVectorConstruct);
    _M_start = static_cast<fable_u8*>(malloc(n));
    memset(_M_start, 0xCD, n);
    memset(_M_start, 0, n);
    _M_finish = _M_start + n;
    _M_end_of_storage._M_data = _M_start + n;
    g_vectorData = _M_start;
    g_vectorSize = n;
}

// ------------------------------------------------------------------ banks
NBankFileManager::CRetailBank::CRetailBank()
{
    Record(kBankConstruct);
    g_bank = this;
    useDma_ = true;
}

NBankFileManager::CRetailBank::~CRetailBank() {}
void NBankFileManager::CContainedBankMap::Erase(CNode*) {}

void FABLE_FASTCALL
CCountedPointer<NBankFileManager::CRetailBank>::DeleteData(void* data)
{
    Record(kBankDelete);
    delete static_cast<NBankFileManager::CRetailBank*>(data);
}

NBankFileManager::CContainedBank&
NBankFileManager::CContainedBankMap::operator[](const CCharString& key)
{
    Record(kMapIndex);
    Check(g_bank != 0 && this == &g_bank->containedBanks_);
    const char* name = key;
    for (unsigned int i = 0; i < g_mapCount; ++i)
        if (g_map[i].name == name)
            return g_map[i].value;
    g_map[g_mapCount].name = name;
    memset(&g_map[g_mapCount].value, 0, sizeof(g_map[g_mapCount].value));
    return g_map[g_mapCount++].value;
}

CThreadedFile::CThreadedFile()
{
    Record(kThreadedConstruct);
    g_threaded = this;
}

CThreadedFile::~CThreadedFile() {}

bool CThreadedFile::Open(const CWideString& name, bool noCaching)
{
    Record(kThreadedOpen);
    Check(this == g_threaded);
    Check(&name == g_pathname);
    g_threadedNoCaching = noCaching;
    g_useDmaAtOpen = g_bank != 0 && g_bank->useDma_;
    return true;
}

void CCountedPointer<CThreadedFile>::Reset(CThreadedFile* data)
{
    Record(kThreadedReset);
    Check(g_bank != 0 && this == &g_bank->threadedFile_);
    Check(data == g_threaded);
    data_ = data;
    control_ = new CCPPointerInfo(&DeleteData, data);
}

void FABLE_FASTCALL CCountedPointer<CThreadedFile>::DeleteData(void* data)
{
    delete static_cast<CThreadedFile*>(data);
}

// ------------------------------------------------------------------ driver
namespace
{
    struct Contained
    {
        const char* name;
        fable_u32 values[5];
    };

    unsigned long g_cursor;

    void Put32(fable_u32 value)
    {
        memcpy(g_image + g_cursor, &value, 4);
        g_cursor += 4;
    }

    // BIGB, version, header offset, header size; the header (a count and
    // count x {name, five dwords}) sits at headerOffset.
    void Build(
        const char* magic,
        fable_u32 version,
        const Contained* banks,
        unsigned int count,
        bool emptyHeader)
    {
        memset(g_image, 0xEE, sizeof(g_image));
        g_cursor = 0;
        memcpy(g_image, magic, 4);
        g_cursor = 4;
        Put32(version);
        const fable_u32 headerOffset = 40;
        Put32(headerOffset);
        const unsigned long sizeAt = g_cursor;
        Put32(0);
        g_cursor = headerOffset;
        if (!emptyHeader)
        {
            Put32(count);
            for (unsigned int i = 0; i < count; ++i)
            {
                const unsigned long length = (unsigned long)strlen(banks[i].name) + 1;
                memcpy(g_image + g_cursor, banks[i].name, length);
                g_cursor += length;
                for (unsigned int v = 0; v < 5; ++v)
                    Put32(banks[i].values[v]);
            }
        }
        const fable_u32 headerSize = g_cursor - headerOffset;
        memcpy(g_image + sizeAt, &headerSize, 4);
        g_imageSize = g_cursor + 8;
    }

    struct Result
    {
        CBankFileManager::CRetailBankList::CNode* sentinel;
    };

    void Reset()
    {
        g_eventCount = 0;
        g_ok = true;
        g_disk = 0;
        g_fileStream = 0;
        g_memoryStream = 0;
        g_bank = 0;
        g_threaded = 0;
        g_vectorData = 0;
        g_vectorSize = 0;
        g_setPosition = 0xFFFFFFFF;
        g_memorySize = 0;
        g_useDmaAtOpen = true;
        g_threadedNoCaching = true;
        g_mapCount = 0;
        g_nameCount = 0;
    }

    // Runs OpenRetailBank on a fresh manager whose retail-bank list is an
    // empty sentinel; returns the list's first node (or the sentinel).
    CBankFileManager::CRetailBankList::CNode* Run(
        bool pathExists,
        fable_u32 managerVersion,
        CBankFileManager::CRetailBankList::CNode** sentinelOut)
    {
        static fable_u8 managerBytes[sizeof(CBankFileManager)];
        static fable_u8 pathnameBytes[sizeof(CWideString)];
        memset(managerBytes, 0xA5, sizeof(managerBytes));
        memset(pathnameBytes, 0, sizeof(pathnameBytes));
        CBankFileManager* manager =
            reinterpret_cast<CBankFileManager*>(managerBytes);
        CBankFileManager::CRetailBankList::CNode* sentinel =
            static_cast<CBankFileManager::CRetailBankList::CNode*>(
                malloc(sizeof(CBankFileManager::CRetailBankList::CNode)));
        sentinel->next_ = sentinel;
        sentinel->prev_ = sentinel;
        manager->RetailBanks.node_ = sentinel;
        manager->retailBankVersion_ = managerVersion;
        g_manager = manager;
        g_pathname = reinterpret_cast<const CWideString*>(pathnameBytes);
        g_pathExists = pathExists;

        manager->OpenRetailBank(*g_pathname, true);

        *sentinelOut = sentinel;
        return sentinel->next_;
    }

    bool NoBank(CBankFileManager::CRetailBankList::CNode* first,
                CBankFileManager::CRetailBankList::CNode* sentinel)
    {
        return first == sentinel && sentinel->prev_ == sentinel;
    }

    // The disk file is opened, wrapped, and released again with the stream.
    bool DiskOnlyLifetime()
    {
        return
            Count(kDiskConstruct) == 1 &&
            Count(kDiskOpen) == 1 &&
            Count(kStreamConstruct) == 1 &&
            Count(kStreamDestroy) == 1 &&
            Count(kDiskDelete) == 1 &&
            Count(kDiskClose) == 1 &&
            At(kDiskConstruct) < At(kDiskOpen) &&
            At(kDiskOpen) < At(kStreamConstruct) &&
            At(kStreamConstruct) < At(kStreamDestroy) &&
            At(kStreamDestroy) < At(kDiskDelete) &&
            Count(kBankConstruct) == 0 &&
            Count(kThreadedOpen) == 0;
    }

    bool CheckMissingPath()
    {
        Reset();
        g_window = 64;
        CBankFileManager::CRetailBankList::CNode* sentinel;
        CBankFileManager::CRetailBankList::CNode* first =
            Run(false, 7, &sentinel);
        const bool pass =
            g_ok && g_eventCount == 1 && g_events[0] == kPathExists &&
            NoBank(first, sentinel);
        free(sentinel);
        return pass;
    }

    bool CheckBadMagic()
    {
        Reset();
        g_window = 64;
        Build("BIGC", 7, 0, 0, false);
        CBankFileManager::CRetailBankList::CNode* sentinel;
        CBankFileManager::CRetailBankList::CNode* first =
            Run(true, 7, &sentinel);
        const bool pass =
            g_ok && DiskOnlyLifetime() && NoBank(first, sentinel) &&
            Count(kToCharString) == 0;
        free(sentinel);
        return pass;
    }

    bool CheckVersionMismatch()
    {
        Reset();
        g_window = 6;  // the magic fits the first window, the version overflows
        Build("BIGB", 8, 0, 0, false);
        CBankFileManager::CRetailBankList::CNode* sentinel;
        CBankFileManager::CRetailBankList::CNode* first =
            Run(true, 7, &sentinel);
        const bool pass =
            g_ok && DiskOnlyLifetime() && NoBank(first, sentinel) &&
            Count(kStreamOverflow) == 1;
        free(sentinel);
        return pass;
    }

    bool CheckOpen(long window, bool emptyHeader, bool* sawOverflow)
    {
        static const Contained banks[] =
        {
            { "text",   { 0x11, 0x12, 0x13, 0x14, 0x15 } },
            { "sounds", { 0x21, 0x22, 0x23, 0x24, 0x25 } },
            { "art",    { 0x31, 0x32, 0x33, 0x34, 0x35 } }
        };
        Reset();
        g_window = window;
        Build("BIGB", 7, banks, 3, emptyHeader);
        CBankFileManager::CRetailBankList::CNode* sentinel;
        CBankFileManager::CRetailBankList::CNode* first =
            Run(true, 7, &sentinel);
        *sawOverflow = Count(kStreamOverflow) != 0;

        bool pass =
            g_ok &&
            first != sentinel &&
            first->next_ == sentinel &&
            first->prev_ == sentinel &&
            sentinel->prev_ == first &&
            first->value_.Data == g_bank &&
            first->value_.Info != 0 &&
            first->value_.Info->RefCount == 1 &&
            first->value_.Info->Data == g_bank &&
            Count(kBankDelete) == 0 &&
            // The bank keeps the one remaining reference to the disk file.
            g_bank->diskFile_.Data == g_disk &&
            g_bank->diskFile_.Info != 0 &&
            g_bank->diskFile_.Info->RefCount == 1 &&
            Count(kDiskDelete) == 0 &&
            Count(kDiskClose) == 0 &&
            static_cast<const char*>(g_bank->filePath_) == g_pathToken &&
            g_bank->threadedFile_.data_ == g_threaded &&
            !g_bank->useDma_ &&
            !g_useDmaAtOpen &&
            !g_threadedNoCaching &&
            Count(kStreamDestroy) == 1 &&
            At(kThreadedOpen) < At(kStreamDestroy) &&
            At(kBankConstruct) < At(kToCharString) &&
            At(kThreadedConstruct) < At(kThreadedReset) &&
            At(kThreadedReset) < At(kThreadedOpen);

        if (emptyHeader)
        {
            pass = pass &&
                Count(kVectorConstruct) == 0 &&
                Count(kSetPosition) == 0 &&
                Count(kMemoryConstruct) == 0 &&
                g_mapCount == 0;
        }
        else
        {
            pass = pass &&
                Count(kVectorConstruct) == 1 &&
                g_setPosition == 40 &&
                At(kVectorConstruct) < At(kSetPosition) &&
                At(kSetPosition) < At(kMemoryConstruct) &&
                Count(kReadName) == 3 &&
                Count(kMapIndex) == 3 &&
                Count(kMemoryDestroy) == 1 &&
                At(kMemoryDestroy) < At(kThreadedConstruct) &&
                g_mapCount == 3;
            for (unsigned int i = 0; pass && i < 3; ++i)
            {
                const NBankFileManager::CContainedBank& value = g_map[i].value;
                pass =
                    strcmp(g_map[i].name, banks[i].name) == 0 &&
                    value.dataVersion_ == banks[i].values[0] &&
                    value.entryCount_ == banks[i].values[1] &&
                    value.infoOffset_ == banks[i].values[2] &&
                    value.infoSize_ == banks[i].values[3] &&
                    value.alignment_ == banks[i].values[4];
            }
        }

        free(first);
        free(sentinel);
        return pass;
    }
}

int main()
{
    bool overflowSmall = false;
    bool overflowLarge = true;
    bool overflowEmpty = true;
    const bool missing = CheckMissingPath();
    const bool badMagic = CheckBadMagic();
    const bool version = CheckVersionMismatch();
    const bool large = CheckOpen(256, false, &overflowLarge);
    const bool small = CheckOpen(5, false, &overflowSmall);
    const bool empty = CheckOpen(256, true, &overflowEmpty);

    if (!missing || !badMagic || !version || !large || !small || !empty ||
        overflowLarge || !overflowSmall || overflowEmpty)
    {
        printf(
            "FABLETLC_OPEN_RETAIL_BANK FAIL missing=%d magic=%d version=%d "
            "large=%d small=%d empty=%d overflow=%d/%d/%d events=%u\n",
            missing, badMagic, version, large, small, empty,
            overflowLarge, overflowSmall, overflowEmpty, g_eventCount);
        return 1;
    }

    printf("FABLETLC_OPEN_RETAIL_BANK PASS\n");
    return 0;
}
