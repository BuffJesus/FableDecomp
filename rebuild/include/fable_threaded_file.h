#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

class CWideString;

class CThreadedFile
{
public:
    CThreadedFile();
    bool Open(const CWideString& name, bool noCaching);

    fable_u32 vtable_;
    fable_u32 deviceId_;
    void* fileHandle_;
    void* filenameStorage_;
    fable_u32 length_;
    fable_u32 physicalSortKey_;
    bool openedForWrite_;
    bool openFlag_;
    fable_u8 padding1A_[2];
};

FABLE_STATIC_ASSERT(sizeof(CThreadedFile) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, fileHandle_) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, filenameStorage_) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, openedForWrite_) == 0x18);

template<typename T>
class CCountedPointer;

template<>
class CCountedPointer<CThreadedFile>
{
public:
    void Reset(CThreadedFile* data);

    CThreadedFile* data_;
    void* control_;
};

FABLE_STATIC_ASSERT(sizeof(CCountedPointer<CThreadedFile>) == 0x08);
