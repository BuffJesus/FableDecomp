// CCompressorZlib::Compress(const void*, unsigned long, void*, unsigned long,
// CCompressionInfo&) @ 0x00A3AC40 (FableWin ?Compress@CCompressorZlib@@UAEXPBXKPAXKAAVCCompressionInfo@@@Z;
// vtable 0x01245974 slot 5).
#include "engine/CCompressorZlib.h"

// Deflate behind a 13-byte header ("CMP1", uncompressed size, compressed
// size including the header, type 6); nothing to compress is a bare header.
void CCompressorZlib::Compress(const void* data, unsigned long size, void* output, unsigned long capacity, CCompressionInfo& info)
{
    unsigned char* header = static_cast<unsigned char*>(output);
    if (size <= 0)
    {
        *reinterpret_cast<unsigned long*>(header) = 'CMP1';
        *reinterpret_cast<long*>(header + 4) = 0;
        *reinterpret_cast<long*>(header + 8) = 13;
        header[12] = 6;
        info.CompressedSize = 13;
        info.UncompressedSize = 0;
        info.CompressionType = 6;
        return;
    }
    *reinterpret_cast<unsigned long*>(header) = 'CMP1';
    *reinterpret_cast<long*>(header + 4) = size;
    *reinterpret_cast<long*>(header + 8) = size;
    header[12] = 6;
    const long compressed = InternalCompress(data, size, header + 13, capacity - 13) + 13;
    *reinterpret_cast<long*>(header + 8) = compressed;
    info.UncompressedSize = size;
    info.CompressedSize = compressed;
    info.CompressionType = 6;
}
