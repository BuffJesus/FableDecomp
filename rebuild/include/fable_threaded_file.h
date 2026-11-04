#pragma once

#include "fable_threaded_file_priority.h"
#include "fable_threaded_file_device.h"

#include <stddef.h>

#include "rebuild_abi.h"
#include "fable_base_class.h"
#include "fable_counted_pointer.h"
#include "fable_string.h"

class CThreadedFileObjectDesc;
class CThreadedFile : public CBaseClassNonCopyable
{
public:
    CThreadedFile();
    virtual ~CThreadedFile();
    void Close();
    void* FinishAndGetFileHandle();
    bool RemoveObject(unsigned long uid);
    bool IsObjectFinished(unsigned long uid) const;
    bool ObjectExists(unsigned long uid) const;
    bool IsOpenForWrite() const;
    EThreadedFileDeviceID PeekDeviceID() const;
    unsigned long PeekPhysicalSortKey() const;
    void KeepUpdating();
    void WaitForObjectToFinish(unsigned long uid);
    void SetObjectPriority(unsigned long uid, EThreadedFilePriority priority);
    unsigned long AddObject(const CThreadedFileObjectDesc& desc);
    bool Open(const CWideString& name, bool noCaching);
    bool OpenForWrite(const CWideString& name, bool overwrite);
    bool ResizeFile(unsigned long length);

    fable_u32 deviceId_;
    void* fileHandle_;
    CWideString filenameStorage_;
    fable_u32 length_;
    fable_u32 physicalSortKey_;
    bool openedForWrite_;
    bool openFlag_;
};

FABLE_STATIC_ASSERT(sizeof(CThreadedFile) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, fileHandle_) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, filenameStorage_) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CThreadedFile, openedForWrite_) == 0x18);

template<>
class CCountedPointer<CThreadedFile>
{
public:
    CCountedPointer() : data_(0), control_(0) {}
    CCountedPointer(const CCountedPointer& other)
        : data_(other.data_), control_(other.control_)
    {
        if (control_) ++control_->RefCount;
    }
    CCountedPointer& operator=(const CCountedPointer& other); // retail 009D6FD0
    ~CCountedPointer()
    {
        if (control_ != 0)
        {
            --control_->RefCount;
            if (control_->RefCount == 0)
            {
                control_->DeleteFunc(control_->Data);
                delete control_;
            }
        }
        data_ = 0;
        control_ = 0;
    }
    CThreadedFile* operator->() const { return data_; }

    void Reset(CThreadedFile* data);

    static void FABLE_FASTCALL DeleteData(void* data);

    CThreadedFile* data_;
    CCPPointerInfo* control_;
};

FABLE_STATIC_ASSERT(sizeof(CCountedPointer<CThreadedFile>) == 0x08);
