// CFileDataInputStream::InternalReadUnbuffered(fable_u8*, long) @ 0x009943D0
// (Ego_r ?InternalReadUnbuffered@CFileDataInputStream@@EAEXPAEJ@Z, vtable
// 0x129A728 slot 10): reads straight from the file at the stream position.
#include "fable_data_input_stream.h"
#include "fable_filesystem.h"

void CFileDataInputStream::InternalReadUnbuffered(fable_u8* data, long size)
{
    unsigned long position = GetPosition();
    File->SetPosition(position);
    File->Read(data, size, 0);
}