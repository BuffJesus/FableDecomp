// CDataInputStream::ReadZlibCompressed(void*, long) @ 0x00996230 (FableWin
// ?ReadZlibCompressed@CDataInputStream@@UAEXPAXJ@Z; vtable slot 5).
#include "fable_data_input_stream.h"
#include "fable_carray.h"
#include "fable_filesystem.h"
#include "engine/CCompressorZlib.h"

// A 32-bit compressed size, then a zlib stream that must inflate to exactly
// `size` bytes (CAFile's error callback otherwise).
void CDataInputStream::ReadZlibCompressed(void* data, long size)
{
    if (size <= 0)
        return;
    long compressedSize = ReadValue<long>();
    CArrayVector<unsigned char> compressed(compressedSize);
    Read(compressed.GetAsCArray(), compressedSize);
    CCompressorZlib zlib;
    if (zlib.InternalDecompress(compressed.GetAsCArray(), compressedSize, data, size) != (unsigned long)size)
        CAFile::DoErrorCallback();
}
