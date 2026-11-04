#pragma once

#include "fable_filesystem.h"

// RTTI: CDiskFileWin32 : CAFile. The inline members are the bodies retail
// emits as COMDATs at 0x401650..0x4016B0.
class CDiskFileWin32 : public CAFile
{
public:
    // Inlined by retail callers (0x9d4c30): base ctor, vptr, Pathname ctor,
    // then the flag. An init-list Opened(false) stores before Pathname.
    CDiskFileWin32() { Opened = false; }
    CDiskFileWin32(
        const CWideString& pathname,
        FileAccessType accessType,
        unsigned long flags);
    // FableWin ??0CDiskFileWin32@@QAE@ABV0@@Z; inline in retail (Clone
    // 0x0099AE00: base ctor, vptr, Opened=false, Pathname ctor, then a direct
    // call to Open on the source's path, read-only, flags 2).
    CDiskFileWin32(const CDiskFileWin32& other)
        : Opened(false)
    {
        Open(other.Pathname, FILE_ACCESS_READ, 2);
    }
    virtual ~CDiskFileWin32() { Close(); }

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
    virtual long GetLength() const { return Length; }
    virtual bool IsOpen() const { return Opened; }
    virtual CWideString GetPathname() const { return Pathname; }
    virtual CAFile* Clone() const;
    virtual CFileInformation GetFileInformation();

private:
    FileAccessType AccessType;
    long Position;
    long Length;
    void* WinFileHandle;
    bool Opened;
    CWideString Pathname;
};

FABLE_STATIC_ASSERT(sizeof(CDiskFileWin32) == 0x1C);
