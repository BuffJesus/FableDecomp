#pragma once
// Hand-maintained retail model: ctor inlined at 0x009AD639, Initialise 0x00A3AA10,
// destructor 0x00A3ACC0. Ego_r's 0x5C size includes a larger base vector.
#include "engine/CACompressor.h"
#include "fable_zlib.h"
class CCompressorZlib : public CACompressor
{
public:
    CCompressorZlib() : CACompressor(GetRequiredWorkspaceSize()) { Initialise(); }
    virtual ~CCompressorZlib();
    virtual void Compress(const void*, unsigned long, void*, unsigned long, CCompressionInfo&);
    virtual void CompressAsImage(const void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&);
    virtual void Decompress(const void*, unsigned long, void*, unsigned long, CCompressionInfo&);
    static unsigned long FABLE_FASTCALL GetRequiredWorkspaceSize(); // 0x00A3A7D0
    void FinishCompressStream(); // 0x00A3A930
    void FinishDecompressStream();
    bool HasStreamInput() const; // retail 00A3A7E0
    void SetStreamInput(const void*, unsigned long); // 0x00A3AB90 (Ego_r ?SetStreamInput@CCompressorZlib@@QAEXPBXK@Z)
    unsigned long InternalCompress(const void*, unsigned long, void*, unsigned long); // 0x00A3AA40
    unsigned long InternalDecompress(const void*, unsigned long, void*, unsigned long); // 0x00A3AB00

private:
    friend class CCompressorZlibTestAccess;
    void Initialise(); // 0x00A3AA10
    static void* __cdecl Alloc(void*, unsigned int, unsigned int); // 0x00A3A9C0
    static void __cdecl Free(void*, void*); // 0x00A3AA00
    void SetupStream(z_stream_s& stream, const void* data, unsigned long size, void* output, unsigned long capacity)
    {
        stream.next_in = (unsigned char*)data;
        stream.next_out = (unsigned char*)output;
        stream.avail_in = size;
        stream.avail_out = capacity;
    }
public:
    z_stream_s Stream;
    bool ZlibInitialised;
    bool Compressing;
    bool ErrorFlag;
};
FABLE_STATIC_ASSERT(sizeof(CCompressorZlib) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CCompressorZlib, Stream) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CCompressorZlib, ZlibInitialised) == 0x54);
