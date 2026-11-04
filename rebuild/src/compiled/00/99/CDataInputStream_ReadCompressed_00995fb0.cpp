// CDataInputStream::ReadCompressed(void*, long) @ 0x00995FB0 (FableWin
// ?ReadCompressed@CDataInputStream@@UAEXPAXJ@Z; vtable slot 4).
#include "fable_data_input_stream.h"
#include "fable_carray.h"
#include "fable_filesystem.h"
#include "fable_lzo.h"

// A 16-bit compressed size (0xFFFF: a 32-bit size follows). A positive size
// is an LZO block that inflates to all but the last three bytes, which follow
// raw; otherwise the data is stored as is.
void CDataInputStream::ReadCompressed(void* data, long size)
{
    if (size <= 0)
        return;
    long compressedSize = ReadValue<short>();
    if (compressedSize == -1)
        compressedSize = ReadValue<long>();
    if (compressedSize > 0)
    {
        CArrayVector<unsigned char> compressed(compressedSize);
        Read(compressed.GetAsCArray(), compressedSize);
        unsigned long decompressedSize = size;
        lzo1x_decompress_asm_fast(compressed.GetAsCArray(), compressedSize,
                                  static_cast<unsigned char*>(data), &decompressedSize, 0);
        if (decompressedSize != (unsigned long)(size - 3))
            CAFile::DoErrorCallback();
        Read(static_cast<char*>(data) + size - 3, 3);
    }
    else
    {
        Read(data, size);
    }
}
