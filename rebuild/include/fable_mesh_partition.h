#pragma once
// C3DMeshPartitionElement (Ego_r.pdb, size 0x38): a binary partition-tree node over a mesh's triangles. Layout and
// method list from Ego_r; bodies in C3DMeshPartitionElement_Methods_00ac2160.cpp (retail 0x00AC2100-0x00AC2930,
// scalar deleting destructor 0x00AC37B0).
#include "fable_bounding_volumes.h"
#include "fable_chunked_file.h"   // C3DVertex2, C3DTriangle2, CArray, CCountedPointer

class CDataInputStream;
class CDataOutputStream;

class C3DMeshPartitionElement
{
public:
    // Inline in retail (BuildSubElement 0x00AC2160, ReadFromFile 0x00AC21E0 and
    // C3DMeshFilePartitionChunk::ReadFromFile 0x00AC2970 expand it).
    C3DMeshPartitionElement()
    {
        SubElements[0] = 0;
        SubElements[1] = 0;
        StartTriangle = 0;
        TriangleCount = 0;
    }
    // Ego_r ??1C3DMeshPartitionElement@@QAE@XZ (retail 0x00AC2100); the scalar deleting destructor 0x00AC37B0 and
    // the unit's other callers expand it.
    ~C3DMeshPartitionElement();

    C3DMeshPartitionElement* BuildSubElement(long index);           // 0x00AC2160
    void ReadFromFile(CDataInputStream& stream);                    // 0x00AC21E0
    void WriteToFile(CDataOutputStream& stream) const;              // 0x00AC27A0
    unsigned long Sizeof() const;                                   // 0x00AC2930

    const C3DBoundingBox& PeekBoundingBox() const { return BoundingBox; }
    const C3DBoundingSphere& PeekBoundingSphere() const { return BoundingSphere; }
    long PeekStartTri() const { return StartTriangle; }
    long PeekTriCount() const { return TriangleCount; }
    const C3DMeshPartitionElement* PeekSubElement(long index) const { return SubElements[index]; }

    C3DBoundingBox BoundingBox;                 // +0x00
    long StartTriangle;                         // +0x18
    long TriangleCount;                         // +0x1C
    C3DMeshPartitionElement* SubElements[2];    // +0x20
    C3DBoundingSphere BoundingSphere;           // +0x28
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshPartitionElement) == 0x38);

// C3DMeshPartitionBuilderElement (Ego_r.pdb): the build-time node that splits a triangle set into a partition tree, then
// writes it out as C3DMeshPartitionElement nodes plus the reordered triangle list. Retail layout (0x44 bytes: retail's
// CArray is the 12-byte STLport vector, Ego_r's is 0x10); bodies in C3DMeshPartitionBuilderElement_00ac2a30.cpp
// (destructor 0x00AC2A30, BuildOutput 0x00AC2A80, Build 0x00AC36D0). PartitionSpace (0x00AC2BE0) is not
// reconstructed yet.
class C3DMeshPartitionBuilderElement
{
public:
    ~C3DMeshPartitionBuilderElement();                                                       // 0x00AC2A30
    void Build(const C3DVertex2* vertices, const C3DTriangle2* triangles, long triangleCount,
               CCountedPointer<C3DMeshPartitionElement>* tree,
               CCountedPointer<CArray<C3DTriangle2> >* triangleList);                         // 0x00AC36D0

private:
    long PartitionSpace(const C3DVertex2* vertices, long depth, float limit);                // 0x00AC2BE0
    void BuildOutput(C3DMeshPartitionElement* output, CArray<C3DTriangle2>* triangleList);   // 0x00AC2A80

public:
    long StartTriangle;                                 // +0x00
    long TriangleCount;                                 // +0x04
    C3DBoundingBox BoundingBox;                         // +0x08
    C3DBoundingSphere BoundingSphere;                   // +0x20
    CArray<C3DTriangle2> Triangles;                     // +0x30
    C3DMeshPartitionBuilderElement* SubElements[2];     // +0x3C
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshPartitionBuilderElement) == 0x44);
