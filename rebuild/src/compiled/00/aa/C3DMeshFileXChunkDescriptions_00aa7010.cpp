// The mesh X chunks' descriptions (retail 0x00AA7010..0x00AA7350, each beside its chunk's deleting destructor;
// the X chunk constructors and vtables are C3DMeshFileXChunks_00aa4b50.cpp, TriangleStrip's description 0x00AA7280 too).
#include "fable_chunked_file.h"

// Retail 00AA7010.
const CCharString C3DMeshFileXTextureIndexChunk::GetChunkDescription() const
{
    return CCharString("Texture Index");
}

// Retail 00AA7050.
const CCharString C3DMeshFileXMappingInfoChunk::GetChunkDescription() const
{
    return CCharString("Mapping Info");
}

// Retail 00AA7090.
const CCharString C3DMeshFileXAnimatedPrimitiveChunk::GetChunkDescription() const
{
    return CCharString("Animated Primitive");
}

// Retail 00AA7100.
const CCharString C3DMeshFileXBoundingSphereChunk::GetChunkDescription() const
{
    return CCharString("Bounding Sphere");
}

// Retail 00AA7140.
const CCharString C3DMeshFileXBoundingBoxChunk::GetChunkDescription() const
{
    return CCharString("Bounding Box");
}

// Retail 00AA7230.
const CCharString C3DMeshFileXVertexDataChunk::GetChunkDescription() const
{
    return CCharString("Vertex Buffer Data");
}

// Retail 00AA7350.
const CCharString C3DMeshFileXUVTangentChunk::GetChunkDescription() const
{
    return CCharString("UV Tangent Vectors");
}
