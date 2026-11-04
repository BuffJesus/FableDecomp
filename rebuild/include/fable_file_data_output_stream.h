#pragma once
#include "fable_data_output_stream.h"
#include "fable_filesystem.h"
class CFileDataOutputStream : public CDataOutputStream {
public:
    explicit CFileDataOutputStream(CAFile* file);
    virtual ~CFileDataOutputStream();
    virtual void SetPosition(unsigned long position);
    virtual unsigned long GetPosition() const;
    virtual void Close();
    virtual void Write(const void* data, long size);
    CAFile* File;
};
FABLE_STATIC_ASSERT(sizeof(CFileDataOutputStream) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CFileDataOutputStream, File) == 8);
