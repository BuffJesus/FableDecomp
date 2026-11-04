#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_base_class.h"
#include "test_support/base_class_stubs.h"

// Same PDB shapes as the source file.
class CWideString;
class CASuspendableProcess;

enum FileAccessType
{
};

class CAFile : public CBaseClassNonCopyable
{
public:
    virtual void Open(
        const CWideString& pathname,
        FileAccessType accessType,
        unsigned long flags) = 0;
    virtual void Close() = 0;
    virtual void Read(
        void* data,
        long length,
        CASuspendableProcess* process) = 0;
    virtual void Write(
        const void* data,
        long length,
        CASuspendableProcess* process) = 0;
    virtual void SetPosition(long position) = 0;
    virtual void MovePosition(long offset) = 0;
    virtual long GetPosition() const = 0;
    virtual void SetLength(long length) = 0;
    virtual long GetLength() const = 0;
};

#pragma pack(push, 1)
template<typename T>
class CBasicString
{
public:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;

protected:
    void SetString(unsigned long length, T fill);
};

struct CCharStringData
{
    CBasicString<char> Data;
    long NoRefs;
};
#pragma pack(pop)

class CCharString
{
public:
    void LoadFromFile(
        CAFile& file,
        CASuspendableProcess* process,
        long length);

    CCharStringData* PStringData;

private:
    void UnassignString();
};

namespace
{
    unsigned char g_storage[sizeof(CCharStringData)];
    char g_buffer[128];
    bool g_recordNew;
    bool g_failAllocation;
    unsigned int g_releaseCalls;
    unsigned int g_allocateCalls;
    unsigned int g_setStringCalls;
    unsigned int g_sizeCalls;
    unsigned int g_positionCalls;
    unsigned int g_readCalls;
    unsigned int g_allocateSize;
    unsigned long g_setStringLength;
    char g_setStringFill;
    bool g_setStringSawFreshData;
    void* g_readBuffer;
    long g_readLength;
    CASuspendableProcess* g_readProcess;
    long g_fileSize;
    long g_filePosition;

    class TestFile : public CAFile
    {
    public:
        virtual void Open(const CWideString&, FileAccessType, unsigned long) {}
        virtual void Close() {}
        virtual void Read(
            void* data,
            long length,
            CASuspendableProcess* process)
        {
            ++g_readCalls;
            g_readBuffer = data;
            g_readLength = length;
            g_readProcess = process;
        }
        virtual void Write(const void*, long, CASuspendableProcess*) {}
        virtual void SetPosition(long) {}
        virtual void MovePosition(long) {}
        virtual long GetPosition() const
        {
            ++g_positionCalls;
            return g_filePosition;
        }
        virtual void SetLength(long) {}
        virtual long GetLength() const
        {
            ++g_sizeCalls;
            return g_fileSize;
        }
    };

    void Reset()
    {
        memset(g_storage, 0xFF, sizeof(g_storage));
        memset(g_buffer, 0, sizeof(g_buffer));
        g_failAllocation = false;
        g_releaseCalls = 0;
        g_allocateCalls = 0;
        g_setStringCalls = 0;
        g_sizeCalls = 0;
        g_positionCalls = 0;
        g_readCalls = 0;
        g_allocateSize = 0;
        g_setStringLength = 0;
        g_setStringFill = 0;
        g_setStringSawFreshData = false;
        g_readBuffer = 0;
        g_readLength = 0;
        g_readProcess = 0;
        g_fileSize = 91;
        g_filePosition = 27;
    }
}

CBaseClassNonCopyable::CBaseClassNonCopyable()
{
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
}

void* operator new(unsigned int size)
{
    if (!g_recordNew)
        return malloc(size);
    ++g_allocateCalls;
    g_allocateSize = size;
    return g_failAllocation ? 0 : g_storage;
}

void CCharString::UnassignString()
{
    ++g_releaseCalls;
    PStringData = 0;
}

template<>
void CBasicString<char>::SetString(unsigned long length, char fill)
{
    ++g_setStringCalls;
    g_setStringLength = length;
    g_setStringFill = fill;
    // The inline constructor clears the header before SetString runs.
    g_setStringSawFreshData =
        PData == 0 &&
        StringLength == 0 &&
        DataLength == 0 &&
        UseFastExtend;
    PData = g_buffer;
    StringLength = length;
}

int main()
{
    TestFile file;
    CASuspendableProcess* process =
        reinterpret_cast<CASuspendableProcess*>(&file + 1);
    CCharString text;
    CCharStringData* data =
        reinterpret_cast<CCharStringData*>(g_storage);

    Reset();
    text.PStringData = reinterpret_cast<CCharStringData*>(0x11111111);
    g_recordNew = true;
    text.LoadFromFile(file, process, -1);
    g_recordNew = false;
    if (
        g_releaseCalls != 1 ||
        g_allocateCalls != 1 ||
        g_allocateSize != 17 ||
        g_sizeCalls != 1 ||
        g_positionCalls != 1 ||
        g_setStringCalls != 1 ||
        g_setStringLength != 64 ||
        g_setStringFill != ' ' ||
        !g_setStringSawFreshData ||
        g_readCalls != 1 ||
        g_readBuffer != g_buffer ||
        g_readLength != 64 ||
        g_readProcess != process ||
        text.PStringData != data)
    {
        return 1;
    }
    // The bit-field neighbours of DataLength / UseFastExtend survive.
    if (
        g_storage[11] != 0x80 ||
        g_storage[12] != 0xFF ||
        data->NoRefs != 1)
    {
        return 2;
    }

    Reset();
    text.PStringData = 0;
    g_recordNew = true;
    text.LoadFromFile(file, process, 12);
    g_recordNew = false;
    if (
        g_releaseCalls != 0 ||
        g_sizeCalls != 0 ||
        g_positionCalls != 0 ||
        g_setStringLength != 12 ||
        g_readCalls != 1 ||
        g_readLength != 12)
    {
        return 3;
    }

    Reset();
    text.PStringData = 0;
    g_recordNew = true;
    text.LoadFromFile(file, process, 0);
    g_recordNew = false;
    if (
        text.PStringData != data ||
        g_setStringCalls != 1 ||
        g_readCalls != 0)
    {
        return 4;
    }

    Reset();
    g_failAllocation = true;
    text.PStringData = 0;
    g_recordNew = true;
    text.LoadFromFile(file, process, 12);
    g_recordNew = false;
    if (
        text.PStringData != 0 ||
        g_setStringCalls != 0 ||
        g_readCalls != 0)
    {
        return 5;
    }

    printf("FABLETLC_CHAR_STRING_LOAD_FILE_0099F2E0 PASS\n");
    return 0;
}
