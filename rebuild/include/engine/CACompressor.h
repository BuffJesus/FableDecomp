#pragma once
// Hand-maintained retail model. Ego_r names; retail ctor 0x00A3AEA0.
// The donor's 16-byte vector is a 12-byte STLport vector here.
#include "fable_base_class.h"
#include "fable_stlport_vector.h"
#include "engine/CCompressionInfo.h"
class CACompressor : public CBaseClass
{
public:
    explicit CACompressor(long workspaceSize);
    virtual void CompressAsImage(void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&);
    virtual void Compress(void*, unsigned long, void*, unsigned long, CCompressionInfo&);
    virtual void Decompress(void*, unsigned long, void*, unsigned long, CCompressionInfo&);
    virtual void SetQuality(float);

    // Ego_r CACompressor::GetCompressionInfo (inline): a compressed block starts
    // with a 13-byte header, the "CMP1" tag then the uncompressed size, the
    // compressed size (header included) and the compression type.
    void GetCompressionInfo(const void* data, CCompressionInfo& info)
    {
        const unsigned char* header = static_cast<const unsigned char*>(data);
        info.CompressedSize = *reinterpret_cast<const long*>(header + 8);
        info.UncompressedSize = *reinterpret_cast<const long*>(header + 4);
        info.CompressionType = header[12];
    }

    void* Workspace;
    unsigned long WorkspaceSize;
    bool CompressionOn;
    _STL::vector<unsigned char> OwnWorkspace;
};
FABLE_STATIC_ASSERT(sizeof(CACompressor) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CACompressor, OwnWorkspace) == 0x10);
