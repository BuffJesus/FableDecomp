#pragma once
// Hand-owned (retail model): QuadricOptimiserInternals::COptimisedFace, one face of the quadric-based mesh optimiser's
// working mesh (lib_3d_quadric_based_mesh_optimiser). Ego_r.pdb names and members (0x48 there: VertexIndices,
// UniqueVertexIndices, Edges, UTangent/VTangent C3DVector, Exists, OutputIndex, Segment). Partial view: only the
// members read by a reconstructed retail body are declared (+0x00..+0x3C); the trailing fields are not.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_2d_vector.h"
#include "fable_carray.h"
#include "fable_counted_pointer.h"
#include "fable_stlport_tree.h"

namespace QuadricOptimiserInternals
{
// Ego_r CQuadric (0x58): the plane-distance error quadric Q = w * (n.x + d)^2 as its ten distinct coefficients plus the
// accumulated weight, all doubles.
class CQuadric
{
public:
    // Ego_r ?Initialise@CQuadric@QuadricOptimiserInternals@@QAEXABVC3DVector@@0M@Z (retail 0x00AC7050): the quadric of
    // the plane through point with the given normal (d = -normal.point), carrying weight.
    void Initialise(const C3DVector& normal, const C3DVector& point, float weight);
    // Ego_r ?EvaluateError@...@@QBENABVC3DVector@@@Z (retail 0x00AC6D50): v^T Q v for (v, 1).
    double EvaluateError(const C3DVector& v) const;
    // Ego_r ??YCQuadric@...@@QAEXABV01@@Z (retail 0x00AC6E80) and ??XCQuadric@...@@QAEXN@Z (retail 0x00AC6EF0).
    void operator+=(const CQuadric& other);
    void operator*=(double scale);

    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD;   // +0x00 .. +0x48
    double Weight;                                    // +0x50
};

class COptimisedFace
{
public:
    // Ego_r names; retail 0x00AC6F50, 0x00AC6F80, 0x00AC6FA0, 0x00AC6FC0, 0x00AC6FE0, 0x00AC7030.
    void RemapIndex(unsigned long oldIndex, unsigned long newIndex);
    bool HasVertex(unsigned long index) const;
    bool HasUniqueVertex(unsigned long index) const;
    long FindVertex(unsigned long index) const;
    bool IsIdentical(const COptimisedFace& other) const;
    bool IsDegenerate() const;

    unsigned long VertexIndices[3];        // +0x00
    unsigned long UniqueVertexIndices[3];  // +0x0c
    long Edges[3];                         // +0x18
    C3DVector UTangent;                    // +0x24
    C3DVector VTangent;                    // +0x30
};

// Ego_r CSubContraction (8 bytes): one vertex pair of a contraction.
class CSubContraction
{
public:
    bool operator==(const CSubContraction& other) const { return Index1 == other.Index1 && Index2 == other.Index2; }
    bool operator!=(const CSubContraction& other) const { return !(*this == other); }

    unsigned long Index1;  // +0x00
    unsigned long Index2;  // +0x04
};

// Ego_r CIndexList: a set of vertex or face indices (retail: the 12-byte STLport set).
class CIndexList
{
public:
    _STL::set<unsigned long> Indices;   // +0x00
};

class CVertexPairContraction;
class CChunkedFileChunk;

// Ego_r CContractionReference (0x0C): a contraction this vertex takes part in, and which of its pairs.
class CContractionReference
{
public:
    CCountedPointer<CVertexPairContraction> Contraction;  // +0x00
    unsigned long SubIndex;                               // +0x08
};

// Ego_r CUniqueVertex (0x24 there, 0x1C in retail).
class CUniqueVertex
{
public:
    bool Exists;              // +0x00
    CIndexList VertexIndices;  // +0x04
    CIndexList FaceList;       // +0x10
};

// Ego_r COptimisedVertex (0xC0 there, 0xB8 in retail with STLport's 12-byte set and maps).
class COptimisedVertex
{
public:
    // Ego_r ?RemapGroups@COptimisedVertex@QuadricOptimiserInternals@@QAEXABV?$map@KK...@std@@@Z (retail 0x00AC97D0, not
    // reconstructed): renumber the vertex's bone groups through the old -> new map.
    void RemapGroups(const _STL::map<unsigned long, unsigned long>& remap);

    C3DVector Position;                                                      // +0x00
    C3DVector Normal;                                                        // +0x0C
    C3DVector TextureCoord;                                                  // +0x18
    CQuadric PositionError;                                                  // +0x28
    float Cost;                                                              // +0x80
    CIndexList FaceList;                                                     // +0x84
    bool Exists;                                                             // +0x90
    unsigned long UniqueIndex;                                               // +0x94
    unsigned long OutputIndex;                                               // +0x98
    _STL::map<unsigned long, float> Groups;                                  // +0x9C
    _STL::map<unsigned long, CContractionReference> PairContractions;        // +0xA8
};

// Ego_r COptimisationParameters (0x28): the optimiser's cost weights.
class COptimisationParameters
{
public:
    float Scaling;                 // +0x00
    float EdgeWeight;              // +0x04
    float EdgeCost;                // +0x08
    float GroupWeight;             // +0x0C
    float TextureWeight;           // +0x10
    float TexturePenalty;          // +0x14
    float UnsplitTextureWeight;    // +0x18
    float UnsplitNormalWeight;     // +0x1C
    float HighPenalty;             // +0x20
    long MaximumBonesPerVertex;    // +0x24
};

// Ego_r COptimisedEdge (0x10): the two faces sharing an edge (the rest is not declared).
class COptimisedEdge
{
public:
    long FaceA;  // +0x00
    long FaceB;  // +0x04
};

// Ego_r COptimisedPrimitive (0xA0 there, 0x84 in retail with STLport's 12-byte containers). The contraction multimap
// is opaque here (STLport multimap not modelled).
class COptimisedPrimitive
{
public:
    // Ego_r ?GetFaceNormal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABVCOptimisedFace@2@@Z
    // (private there; retail 0x00AC7CB0): the unit normal of a face's plane (unnormalised when degenerate).
    C3DVector GetFaceNormal(const COptimisedFace& face) const;
    // Ego_r ?FindFaceTextureTangents@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCOptimisedFace@2@@Z (private
    // there; retail 0x00AC7E10): the face's U and V texture tangents from its three vertices.
    void FindFaceTextureTangents(COptimisedFace& face);
    // Ego_r ?MakeOrthogonal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABV3@0@Z (private there;
    // retail 0x00AC7200): vector with its component along axis removed, rescaled to the original length.
    C3DVector MakeOrthogonal(const C3DVector& vector, const C3DVector& axis) const;
    // Ego_r ?ComputeSelfContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z
    // (private there; retail 0x00AC7EB0): the cost of collapsing an unsplit vertex pair onto its first vertex.
    void ComputeSelfContraction(CVertexPairContraction& contraction);
    // Ego_r ?RemapGroups@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABV?$map@KK...@std@@@Z (retail 0x00AC99F0):
    // renumber every vertex's bone groups.
    void RemapGroups(const _STL::map<unsigned long, unsigned long>& remap);
    // Ego_r FindTextureTangent (private there; retail 0x00AC7300, not reconstructed): the direction in which the
    // given per-vertex texture coordinate increases across the triangle.
    C3DVector FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2, float t0, float t1,
                                 float t2);

    unsigned long MaterialIndex;               // +0x00
    CArray<CUniqueVertex> UniqueVertices;      // +0x04
    CArray<COptimisedVertex> Vertices;         // +0x10
    CArray<COptimisedFace> Faces;              // +0x1C
    CArray<COptimisedEdge> Edges;              // +0x28
    unsigned char VertexPairContractions[0xC]; // +0x34 std::multimap<float, CCountedPointer<CVertexPairContraction> >
    long SegmentCount;                         // +0x40
    CArray<float> SegmentSizes;                // +0x44
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunksToKeep;  // +0x50
    COptimisationParameters Parameters;        // +0x5C
};

// Ego_r CVertexPairContraction (0x2C there; retail's CArray is the 12-byte STLport vector). Partial view: the flags and
// the sub-contraction list, the costs and the resulting position read by the reconstructed bodies; the trailing
// cost-map iterator (+0x24) is not declared.
class CVertexPairContraction
{
public:
    void RemapIndex(unsigned long oldIndex, unsigned long newIndex);
    bool HasVertex(unsigned long index);
    void CleanUp();

    bool Exists;                               // +0x00
    bool Unsplit;                              // +0x01
    CArray<CSubContraction> SubContractions;   // +0x04
    float Weight2;                             // +0x10
    float Cost;                                // +0x14
    C3DVector ResultingPosition;               // +0x18
};
}

FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::COptimisedFace, UniqueVertexIndices) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::COptimisedFace, Edges) == 0x18);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::COptimisedFace, VTangent) == 0x30);
FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::CQuadric) == 0x58);
FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::CSubContraction) == 8);
FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::CUniqueVertex) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::COptimisedVertex, PositionError) == 0x28);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::COptimisedVertex, FaceList) == 0x84);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::COptimisedVertex, PairContractions) == 0xA8);
FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::COptimisedVertex) == 0xB8);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::COptimisedPrimitive, Vertices) == 0x10);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::COptimisedPrimitive, Parameters) == 0x5C);
FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::COptimisedPrimitive) == 0x84);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::CVertexPairContraction, ResultingPosition) == 0x18);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::CVertexPairContraction, SubContractions) == 0x04);
FABLE_STATIC_ASSERT(offsetof(QuadricOptimiserInternals::CQuadric, Weight) == 0x50);
