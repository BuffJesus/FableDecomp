#pragma once

#include <stdlib.h>
#include <string.h>

#include "fable_filesystem.h"
#include "fable_carray.h"

// Retail's 12-byte malloc-backed vector (first/last/end); its dtor frees the
// block with the CRT, as the inline ~CMemoryFile COMDAT at 0x412020 shows.
template <class T>
class CMemoryFileBuffer
{
public:
    CMemoryFileBuffer() : First(0), Last(0), End(0) {}

    // The engine's sized constructor (FableWin ??0?$CArray@E@@QAE@I@Z):
    // malloc the elements and zero them. Inline in retail
    // (CAFile::CopyFileW 0x00998E70: malloc(0x1000) then `rep stosd`).
    explicit CMemoryFileBuffer(unsigned long count)
    {
        First = static_cast<T*>(malloc(count * sizeof(T)));
        memset(First, 0, count * sizeof(T));
        Last = First + count;
        End = Last;
    }
    ~CMemoryFileBuffer()
    {
        if (First != 0)
            free(First);
    }

    T* First;
    T* Last;
    T* End;
};

// RTTI: CMemoryFile : CAFile. Retail is 0x24 bytes: DataBuffer is a 12-byte
// vector (Ego_r's CArray is 16). The dtor and GetPathname are inline (retail
// COMDATs 0x412020 and 0x412070; GetPosition 0x412050 and GetLength 0x412060
// are inline too but stay declared here). OpenIniFile (0x9a8170) inlines the
// default ctor, the dtor and GetPathname.
class CMemoryFile : public CAFile
{
public:
    CMemoryFile() : Opened(false) {}
    CMemoryFile(
        const CWideString& pathname,
        FileAccessType accessType,
        unsigned long flags);
    // FableWin ??0CMemoryFile@@QAE@ABV0@@Z, retail 0x009E99C0 (Clone's copy).
    CMemoryFile(const CMemoryFile& other);
    virtual ~CMemoryFile() { Close(); }

    // Ego_r CMemoryFile::Open(void): an empty, open, nameless buffer
    // (retail 0x009E90A0).
    void Open();
    // Ego_r CMemoryFile::Open(void*, long): a copy of the caller's bytes
    // (retail 0x009E93C0).
    void Open(void* data, long size);
    // Ego_r CMemoryFile::OpenFromFile(CAFile&, long, CASuspendableProcess*),
    // retail 0x009E9420.
    void OpenFromFile(CAFile& file, long size, CASuspendableProcess* process);

    virtual void Open(
        const CWideString& pathname,
        FileAccessType accessType,
        unsigned long flags);
    virtual void Close();
    virtual void Read(
        void* data,
        long size,
        CASuspendableProcess* process);
    virtual void Write(
        const void* data,
        long size,
        CASuspendableProcess* process);
    virtual void SetPosition(long position);
    virtual void MovePosition(long offset);
    virtual long GetPosition() const;
    virtual void SetLength(long length);
    virtual long GetLength() const;
    virtual bool IsOpen() const;
    virtual CWideString GetPathname() const { return Pathname; }
    virtual CAFile* Clone() const;

    void DumpToFile();

private:
    // Ego_r private CMemoryFile::Copy, retail 0x009E9760.
    void Copy(const CMemoryFile& other);

public:

    FileAccessType AccessType;
    long Position;
    long Length;
    CArrayVector<unsigned char> DataBuffer;
    bool Opened;
    CWideString Pathname;
};

FABLE_STATIC_ASSERT(sizeof(CMemoryFile) == 0x24);
