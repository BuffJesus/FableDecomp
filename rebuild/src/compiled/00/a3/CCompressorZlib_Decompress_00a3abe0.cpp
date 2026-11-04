// CCompressorZlib::Decompress(const void*, unsigned long, void*, unsigned long,
// CCompressionInfo&) @ 0x00A3ABE0 (FableWin ?Decompress@CCompressorZlib@@UAEXPBXKPAXKAAVCCompressionInfo@@@Z;
// vtable 0x01245974 slot 6).
#include "engine/CCompressorZlib.h"

// Inflate the block after its 13-byte header (a header-only block holds
// nothing) and report the header's sizes and type.
void CCompressorZlib::Decompress(const void* data, unsigned long size, void* output, unsigned long capacity, CCompressionInfo& info)
{
    size -= 13;
    if (size == 0)
    {
        GetCompressionInfo(data, info);
        return;
    }
    InternalDecompress(static_cast<const unsigned char*>(data) + 13, size, output, capacity);
    GetCompressionInfo(data, info);
}
