// Ego_r.pdb and retail 0x00995B10: compressed memory-source reader.
#include "fable_data_input_stream.h"
#include "fable_filesystem.h"
#include "fable_lzo.h"

void CMemoryDataInputStream::ReadCompressed(void* data, long size)
{
    if (size <= 0)
        return;
    long compressedSize = ReadValue<short>();
    if (compressedSize == -1)
        compressedSize = ReadValue<long>();
    if (compressedSize > 0) {
        const unsigned char* compressed = ReadConstData(compressedSize);
        unsigned long decompressedSize = size;
        lzo1x_decompress_asm_fast(compressed, compressedSize,
            static_cast<unsigned char*>(data), &decompressedSize, 0);
        if (decompressedSize != (unsigned long)(size - 3))
            CAFile::DoErrorCallback();
        Read(static_cast<char*>(data) + size - 3, 3);
    }
    else {
        Read(data, size);
    }
}
