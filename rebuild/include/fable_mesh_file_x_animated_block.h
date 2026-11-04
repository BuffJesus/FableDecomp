#pragma once
// Ego_r C3DMeshFileXAnimatedPrimitiveBlockChunk ("XAPB", vtable 0x0129E404;
// lib_3d_mesh_file_extras.cpp): one skinned block of an animated primitive,
// with its bone groups, per-vertex indices and bone weights. 0x60 with
// STLport's 12-byte containers (Ego_r 0x78).
#include "fable_chunked_file.h"
#include "fable_vertex_group_block.h"

// Ego_r C3DIndexedBoneWeight (0x08).
class C3DIndexedBoneWeight
{
public:
    C3DIndexedBoneWeight() {}
    // FableWin ?GetBoneIndex@ / ?GetWeight@C3DIndexedBoneWeight@@ (inline): the weight as a float.
    unsigned long GetBoneIndex() const { return BoneIndex; }
    float GetWeight() const { return (float)Weight; }
    void SetBoneIndex(unsigned long bone_index) { BoneIndex = bone_index; }
    void SetWeight(float weight) { Weight = (unsigned long)weight; }
    unsigned long BoneIndex;  // +0x00
    unsigned long Weight;     // +0x04
};

class C3DMeshFileXAnimatedPrimitiveBlockChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFileXAnimatedPrimitiveBlockChunk();  // retail 0x00AA5B90
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA73B0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA6C70
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA5720
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA5C20

    void ClearVertices();                                // retail 0x00AA5BD0
    void AddVertex(unsigned long vertex_index, const CArray<C3DIndexedBoneWeight>& weights);
    // SetGroupBlock (Ego_r) needs CVertexGroupBlock's deep operator=, which our STLport
    // map model lacks (its implicit copy would share nodes); not reconstructed yet.

    CVertexGroupBlock GroupBlock;                  // +0x2C
    unsigned long BonesPerVertex;                  // +0x44
    CArray<unsigned long> VertexList;              // +0x48
    CArray<C3DIndexedBoneWeight> BoneWeights;      // +0x54
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXAnimatedPrimitiveBlockChunk) == 0x60);

// Ego_r C3DMeshFileXLoader (lib_3d_mesh_file_x.cpp): the mesh loader plus the
// X chunk types (bounds, tangents, vertex buffers, strips, animated blocks).
class C3DMeshFileXLoader : public C3DMeshFileLoader
{
public:
    C3DMeshFileXLoader();  // 0x00AA5C80
};
