#pragma once
// Chunked-file object model (lib_chunked_file.cpp and the 3D mesh/anim chunk
// files). Ego_r.pdb names, signatures and virtual order; retail layout uses
// STLport's 12-byte vectors for CArray, so CChunkedFileSuperChunk is 0x2C
// here (0x34 in Ego_r) and every derived chunk's members move down by 8.
// Virtual slots (retail C3DMeshFileSubMeshChunk vtable 0x0129EB18):
//   0 scalar deleting dtor, 1 GetChunkDescription, 2 GetChunkInfo,
//   3 IsSuperChunk, 4 IsUnknownType, 5/6 GetAsSuperChunk, 7 ReadFromFile,
//   8 WriteToFile, 9 CreateNewInstance, 10 InitNewInstance.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_base_class.h"
#include "fable_string.h"
#include "fable_counted_pointer.h"
#include "fable_carray.h"
#include "engine/CMatrix3x4.h"
#include "fable_2d_vector.h"
#include "fable_bounding_volumes.h"
#include "fable_rgb_colour.h"
#include "fable_iv_counted_pointer.h"
#include "fable_exception.h"

// Ego_r C3DVertex2 / C3DTriangle2: the mesh file's vertex and triangle records.
class C3DVertex2
{
public:
    C3DVertex2() {}
    C3DVector Position;  // +0x00
    C3DVector Normal;    // +0x0C
    float U;             // +0x18
    float V;             // +0x1C
};

class C3DVertexBlend2
{
public:
    C3DVertexBlend2() {}
    C3DVertexBlend2(unsigned long vertex_index, float blend_value)
        : VertexIndex(vertex_index), BlendValue(blend_value) {}
    // FableWin ?GetVertexIndex@ / ?GetBlendValue@ / ?SetBlendValue@C3DVertexBlend2@@; inline in retail.
    unsigned long GetVertexIndex() const { return VertexIndex; }
    float GetBlendValue() const { return BlendValue; }
    void SetBlendValue(float blend_value) { BlendValue = blend_value; }
    unsigned long VertexIndex;  // +0x00 (stored as 16 bits)
    float BlendValue;           // +0x04
};

class C3DTriangle2
{
public:
    C3DTriangle2() {}
    unsigned short VertexIndex[3];
};

// Ego_r CVertexCompressionParams (0x20): per-axis position scale and bias of
// a compressed vertex buffer; identity when new.
class CVertexCompressionParams
{
public:
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f)
    {
    }
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};

class CDataInputStream;
class CDataOutputStream;
class CChunkedFileLoader;
class CHandleBasedMemoryPool;
class CChunkedFileSaver;
class CChunkedFileSuperChunk;
class C3DMeshPartitionElement;
class C3DMeshFileTransformationChunk;
class C3DMeshFileBoneChunk;
class C3DMeshFilePrimitiveChunk;
class C3DMeshFileClothChunk;
class C3DMeshFileVerticesChunk;
class C3DMeshFileTrianglesChunk;
class C3DMeshFileSmoothingGroupsChunk;
class C3DMeshFileUniqueVerticesChunk;
class C3DMeshFileGroupChunk;
class C3DAnimFileObjectChunk;
class C3DAnimFileHelpersChunk;
class C3DMeshFileMaterialListChunk;
class C3DMeshFileHelpersChunk;
class C3DMeshFileHelperPointChunk;
class C3DMeshFileDummyObjectChunk;
class C3DMeshFileConvexVolumeChunk;
class C3DAnimFileMovementVectorChunk;
class C3DAnimFileTimingEventChunk;
class C3DAnimFileSequenceChunk;
class C3DAnimFileBoneMaskChunk;
class C3DMeshFileMaterialChunk;
class C3DMeshFileEngineMaterialChunk;
class C3DMeshFileMaterialExtChunk;
class C3DMeshFileMapChunk;
class C3DMeshFileSubMeshChunk;

class CChunkedFileChunk : public CBaseClassNonCopyable
{
public:
    explicit CChunkedFileChunk(const char* chunk_type);  // retail 0x00AA7590
    virtual ~CChunkedFileChunk();
    virtual const CCharString GetChunkDescription() const;
    virtual const CCharString GetChunkInfo() const;
    virtual int IsSuperChunk() const;
    virtual int IsUnknownType() const;
    virtual CChunkedFileSuperChunk* GetAsSuperChunk();
    virtual const CChunkedFileSuperChunk* GetAsSuperChunk() const;
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader) = 0;
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const = 0;
    virtual CChunkedFileChunk* CreateNewInstance() const = 0;

    const char* GetChunkType() const { return ChunkType; }
    int IsParentRecursive(const CChunkedFileChunk* chunk) const;

protected:
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const;

private:
    friend class CChunkedFileSuperChunk;
    // Ego_r ?SetParent@CChunkedFileChunk@@AAEXPAVCChunkedFileSuperChunk@@@Z; inline in retail.
    void SetParent(CChunkedFileSuperChunk* parent) { Parent = parent; }

public:
    char ChunkType[5];               // +0x04
    CChunkedFileSuperChunk* Parent;  // +0x0C
};

class CChunkedFileSuperChunk : public CChunkedFileChunk
{
public:
    typedef CArray<CCountedPointer<CChunkedFileChunk> > TChunkArray;

    explicit CChunkedFileSuperChunk(const char* chunk_type);  // retail 0x00AA8360
    virtual ~CChunkedFileSuperChunk();
    virtual int IsSuperChunk() const;
    virtual CChunkedFileSuperChunk* GetAsSuperChunk();
    virtual const CChunkedFileSuperChunk* GetAsSuperChunk() const;
    // Retail 0x00AA8830 / 0x00AA78C0.
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;

    unsigned long GetNoSubChunks() const;  // retail 0x00AA78B0
    void AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk);           // retail 0x00AA84F0
    void RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>& chunk);  // retail 0x00AA87E0
    void RemoveAllSubChunks();  // retail 0x00AA8400
    // Retail 0x00AA7E40 / 0x00AA7E70.
    CCountedPointer<CChunkedFileChunk> GetSubChunkAt(unsigned long index);
    const CCountedPointer<CChunkedFileChunk> GetSubChunkAt(unsigned long index) const;
    // Retail 0x00AA7EA0 .. 0x00AA82C0, in Ego_r order.
    unsigned long CountSubChunksOfType(const char* chunk_type) const;
    const CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* chunk_type) const;
    const CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous) const;
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* chunk_type);
    CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous);
    void RemoveSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk);
    void RemoveSubChunksOfType(const char* chunk_type);
    // Retail 0x00AA8540.
    void CloneSubChunksOfType(const CChunkedFileSuperChunk& source, const char* chunk_type,
                              CChunkedFileSaver& saver, CChunkedFileLoader& loader);

protected:
    // Retail 0x00AA8460.
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const;

private:
    // Retail 0x00AA7950.
    const CChunkedFileChunk* FindRegisteredSubChunkType(const char* chunk_type) const;
    // Retail 0x00AA83B0: a fresh chunk of the same type (no sub-chunks).
    CChunkedFileSuperChunk(const CChunkedFileSuperChunk& other);

public:
    TChunkArray SubChunks;                // +0x10
    TChunkArray RegisteredSubChunkTypes;  // +0x1C
    // Where the last Find*SubChunkOfType stopped (the const finds update it).
    mutable unsigned long CurrentChunkIndex;  // +0x28
};

class C3DMeshFileSubMeshChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileSubMeshChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "SUBM"; }
    C3DMeshFileSubMeshChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABD3C0
    virtual const CCharString GetChunkInfo() const;         // retail 0x00ABA390
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AB9FC0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AB9AF0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AB9B50

    // Typed finders (retail 0x00ABA4A0-0x00ABAD70, Ego_r order).
    CCountedPointer<C3DMeshFileTransformationChunk> FindTransformation();
    CCountedPointer<const C3DMeshFileTransformationChunk> FindTransformation() const;
    CCountedPointer<C3DMeshFileBoneChunk> FindFirstBone();
    CCountedPointer<C3DMeshFileBoneChunk> FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& previous);
    CCountedPointer<const C3DMeshFileBoneChunk> FindFirstBone() const;
    CCountedPointer<const C3DMeshFileBoneChunk> FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& previous) const;
    CCountedPointer<C3DMeshFilePrimitiveChunk> FindFirstPrimitive();
    CCountedPointer<C3DMeshFilePrimitiveChunk> FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& previous);
    CCountedPointer<const C3DMeshFilePrimitiveChunk> FindFirstPrimitive() const;
    CCountedPointer<const C3DMeshFilePrimitiveChunk> FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& previous) const;
    CCountedPointer<C3DMeshFileClothChunk> FindFirstCloth();
    CCountedPointer<C3DMeshFileClothChunk> FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous);
    CCountedPointer<const C3DMeshFileClothChunk> FindFirstCloth() const;
    CCountedPointer<const C3DMeshFileClothChunk> FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous) const;

    // Ego_r setters, out of line in retail (0x00AB9AA0-0x00AB9AE0).
    void SetName(const CCharString& name);
    void SetSubMeshIndex(long index);
    void SetParentIndex(long index);
    void SetFirstChildIndex(long index);
    void SetNextSiblingIndex(long index);

    CCharString Name;        // +0x2C
    long SubMeshIndex;       // +0x30
    long ParentIndex;        // +0x34
    long FirstChildIndex;    // +0x38
    long NextSiblingIndex;   // +0x3C
    unsigned long CountVertices() const;   // retail 0x00ABC080
    unsigned long CountTriangles() const;  // retail 0x00ABC1B0
};

class C3DMeshFileTransformationChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFileTransformationChunk();
    explicit C3DMeshFileTransformationChunk(const CMatrix3x4& transform);  // retail 0x00AB9BB0
    // Ego_r ?GetStaticChunkType@C3DMeshFileTransformationChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "TRFM"; }
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABD3E0
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AB9C30
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AB9C50
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AB9CB0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AB9CE0

    CMatrix3x4 Transform;  // +0x2C
};

class C3DMeshFileBoneChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFileBoneChunk();
    C3DMeshFileBoneChunk(long bone_index, const CMatrix3x4& localization_matrix);  // retail 0x00AB9D50
    // Ego_r ?GetStaticChunkType@C3DMeshFileBoneChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "BONE"; }
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABD400
    virtual const CCharString GetChunkInfo() const;         // retail 0x00ABADD0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00ABA170
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AB9EA0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AB9F00

    // Ego_r setters, out of line in retail (0x00AB9E30-0x00AB9E90).
    void SetBoneIndex(long index);
    void SetParentIndex(long index);
    void SetFirstChildIndex(long index);
    void SetNextSiblingIndex(long index);
    void SetLocalizationMatrix(const CMatrix3x4& matrix);
    void SetName(const CCharString& name);

    long BoneIndex;                 // +0x2C
    long ParentIndex;               // +0x30
    long FirstChildIndex;           // +0x34
    long NextSiblingIndex;          // +0x38
    CMatrix3x4 LocalizationMatrix;  // +0x3C
    CCharString Name;               // +0x6C
};

class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFilePrimitiveChunk();
    // Ego_r ?GetStaticChunkType@C3DMeshFilePrimitiveChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "PRIM"; }
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABEE90
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00ABCF40
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABC390
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00ABD0D0

    // Typed finders (retail 0x00ABAF50-0x00ABBD70, Ego_r order).
    CCountedPointer<C3DMeshFileVerticesChunk> GetVertices();
    CCountedPointer<const C3DMeshFileVerticesChunk> PeekVertices() const;
    CCountedPointer<C3DMeshFileTrianglesChunk> GetTriangles();
    CCountedPointer<const C3DMeshFileTrianglesChunk> PeekTriangles() const;
    CCountedPointer<C3DMeshFileTrianglesChunk> FindFirstTriangles();
    CCountedPointer<C3DMeshFileTrianglesChunk> FindNextTriangles(const CCountedPointer<const C3DMeshFileTrianglesChunk>& previous);
    CCountedPointer<const C3DMeshFileTrianglesChunk> FindFirstTriangles() const;
    CCountedPointer<const C3DMeshFileTrianglesChunk> FindNextTriangles(const CCountedPointer<const C3DMeshFileTrianglesChunk>& previous) const;
    bool HasSmoothingGroups() const;
    CCountedPointer<C3DMeshFileSmoothingGroupsChunk> GetSmoothingGroups();
    CCountedPointer<const C3DMeshFileSmoothingGroupsChunk> PeekSmoothingGroups() const;
    bool HasUniqueVertices() const;
    CCountedPointer<C3DMeshFileUniqueVerticesChunk> GetUniqueVertices();
    CCountedPointer<const C3DMeshFileUniqueVerticesChunk> PeekUniqueVertices() const;
    CCountedPointer<C3DMeshFileGroupChunk> FindFirstGroup();
    CCountedPointer<C3DMeshFileGroupChunk> FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous);
    CCountedPointer<const C3DMeshFileGroupChunk> FindFirstGroup() const;
    CCountedPointer<const C3DMeshFileGroupChunk> FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous) const;
    CCountedPointer<C3DMeshFileClothChunk> FindFirstCloth();
    CCountedPointer<C3DMeshFileClothChunk> FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous);
    CCountedPointer<const C3DMeshFileClothChunk> FindFirstCloth() const;
    CCountedPointer<const C3DMeshFileClothChunk> FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous) const;

    // Ego_r ?SetMaterialIndex@C3DMeshFilePrimitiveChunk@@QAEXJ@Z; inline in retail.
    void SetMaterialIndex(long index)
    {
        MaterialIndices.resize(1);
        MaterialIndices[0] = index;
    }

    CArray<long> MaterialIndices;  // +0x2C
    // List accessors (C3DMeshFilePrimitiveSubChunks_Accessors_00abc370.cpp).
    void AddMaterialIndex(long index);
};

// No members of its own: a typed container of sub-chunks.
class C3DMeshFileHelpersChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileHelpersChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "HLPR"; }
    C3DMeshFileHelpersChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AAACD0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA9670
    // Typed finders (retail 0x00AAA100..0x00AAA8C0, Ego_r order).
    CCountedPointer<C3DMeshFileHelperPointChunk> FindFirstPoint();
    CCountedPointer<C3DMeshFileHelperPointChunk> FindNextPoint(const CCountedPointer<const C3DMeshFileHelperPointChunk>& previous);
    CCountedPointer<const C3DMeshFileHelperPointChunk> FindFirstPoint() const;
    CCountedPointer<const C3DMeshFileHelperPointChunk> FindNextPoint(const CCountedPointer<const C3DMeshFileHelperPointChunk>& previous) const;
    CCountedPointer<C3DMeshFileDummyObjectChunk> FindFirstDummy();
    CCountedPointer<C3DMeshFileDummyObjectChunk> FindNextDummy(const CCountedPointer<const C3DMeshFileDummyObjectChunk>& previous);
    CCountedPointer<const C3DMeshFileDummyObjectChunk> FindFirstDummy() const;
    CCountedPointer<const C3DMeshFileDummyObjectChunk> FindNextDummy(const CCountedPointer<const C3DMeshFileDummyObjectChunk>& previous) const;
    CCountedPointer<C3DMeshFileConvexVolumeChunk> FindFirstVolumeW();
    CCountedPointer<C3DMeshFileConvexVolumeChunk> FindNextVolumeW(const CCountedPointer<const C3DMeshFileConvexVolumeChunk>& previous);
    CCountedPointer<const C3DMeshFileConvexVolumeChunk> FindFirstVolumeW() const;
    CCountedPointer<const C3DMeshFileConvexVolumeChunk> FindNextVolumeW(const CCountedPointer<const C3DMeshFileConvexVolumeChunk>& previous) const;
};

// No members of its own: a typed container of sub-chunks.
class C3DMeshFileMaterialListChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileMaterialListChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "MTLS"; }
    C3DMeshFileMaterialListChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AC1270
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00ABFBB0
    // Typed finders (retail 0x00AC04C0..0x00AC09A0, Ego_r order).
    CCountedPointer<C3DMeshFileMaterialChunk> FindFirstMaterial();
    CCountedPointer<C3DMeshFileMaterialChunk> FindNextMaterial(const CCountedPointer<const C3DMeshFileMaterialChunk>& previous);
    CCountedPointer<const C3DMeshFileMaterialChunk> FindFirstMaterial() const;
    CCountedPointer<const C3DMeshFileMaterialChunk> FindNextMaterial(const CCountedPointer<const C3DMeshFileMaterialChunk>& previous) const;
    CCountedPointer<C3DMeshFileEngineMaterialChunk> FindFirstEngineMaterial();
    CCountedPointer<C3DMeshFileEngineMaterialChunk> FindNextEngineMaterial(const CCountedPointer<const C3DMeshFileEngineMaterialChunk>& previous);
    CCountedPointer<const C3DMeshFileEngineMaterialChunk> FindFirstEngineMaterial() const;
    CCountedPointer<const C3DMeshFileEngineMaterialChunk> FindNextEngineMaterial(const CCountedPointer<const C3DMeshFileEngineMaterialChunk>& previous) const;
};

// No members of its own: a typed container of sub-chunks.
class C3DMeshFileRootChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFileRootChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00A9C9C0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00A9A8C0
    // Typed finders (retail 0x00A9AEC0..0x00A9B3D0, Ego_r order).
    CCountedPointer<C3DMeshFileSubMeshChunk> FindFirstSubMesh();
    CCountedPointer<C3DMeshFileSubMeshChunk> FindNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>& previous);
    CCountedPointer<const C3DMeshFileSubMeshChunk> FindFirstSubMesh() const;
    CCountedPointer<const C3DMeshFileSubMeshChunk> FindNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>& previous) const;
    CCountedPointer<C3DMeshFileMaterialListChunk> GetMaterialList();
    CCountedPointer<const C3DMeshFileMaterialListChunk> GetMaterialList() const;
    bool HasHelpers() const;
    CCountedPointer<C3DMeshFileHelpersChunk> GetHelpers();
    CCountedPointer<const C3DMeshFileHelpersChunk> GetHelpers() const;
    unsigned long CountVertices() const;   // 0x00A9B460
    unsigned long CountTriangles() const;  // 0x00A9B550
};

// No members of its own: a typed container of sub-chunks.
class C3DAnimFileHelpersChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DAnimFileHelpersChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "HLPR"; }
    C3DAnimFileHelpersChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AAEA90
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AAE0B0
    // Typed finders (retail 0x00AAE4D0..0x00AAE840, Ego_r order).
    bool HasMovementVector() const;
    CCountedPointer<C3DAnimFileMovementVectorChunk> GetMovementVector();
    CCountedPointer<const C3DAnimFileMovementVectorChunk> GetMovementVector() const;
    CCountedPointer<C3DAnimFileTimingEventChunk> FindFirstEvent();
    CCountedPointer<C3DAnimFileTimingEventChunk> FindNextEvent(const CCountedPointer<const C3DAnimFileTimingEventChunk>& previous);
    CCountedPointer<const C3DAnimFileTimingEventChunk> FindFirstEvent() const;
    CCountedPointer<const C3DAnimFileTimingEventChunk> FindNextEvent(const CCountedPointer<const C3DAnimFileTimingEventChunk>& previous) const;
};

class C3DMeshFileXTextureIndexChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileXTextureIndexChunk();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA4D80
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA7010
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA4AF0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA4B00

    unsigned long Index;  // +0x10
};

class C3DMeshFileXMappingInfoChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileXMappingInfoChunk();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA52B0
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA7050
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA4C00
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA4C30

    float AverageTextureStretch;  // +0x10
};

class C3DMeshFilePartitionChunk : public CChunkedFileChunk
{
public:
    C3DMeshFilePartitionChunk();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AC2970
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AC2920
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AC29C0

    CCountedPointer<C3DMeshPartitionElement> PartitionTree;  // +0x10
};

class C3DAnimFileXAllocationSizeChunk : public CChunkedFileChunk
{
public:
    // Inline: retail expands it into CreateNewInstance 0x00AA4530 and the
    // C3DAnimFileXLoader constructor 0x00AA4710 (its own copy is 0x00AA44E0).
    C3DAnimFileXAllocationSizeChunk() : CChunkedFileChunk("XALO") {}
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA4970
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA4570
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA4500
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA4530

    unsigned long AllocSize;  // +0x10
};

class C3DAnimFileRootChunk : public CChunkedFileSuperChunk
{
public:
    C3DAnimFileRootChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00A99E30
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00A98AF0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00A98A70
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00A98AB0

    // Ego_r setters, out of line in retail (0x00A98A50 / 0x00A98A60).
    void SetCyclic(bool cyclic);
    void SetDuration(float duration);
    float GetNonLoopingDuration() const;  // 0x00A99610

    bool LoopFlag;   // +0x2C
    float Duration;  // +0x30
    // Typed finders (retail 0x00A98FA0..0x00A99390, Ego_r order).
    CCountedPointer<C3DAnimFileObjectChunk> FindFirstObject();
    CCountedPointer<C3DAnimFileObjectChunk> FindNextObject(const CCountedPointer<const C3DAnimFileObjectChunk>& previous);
    CCountedPointer<const C3DAnimFileObjectChunk> FindFirstObject() const;
    CCountedPointer<const C3DAnimFileObjectChunk> FindNextObject(const CCountedPointer<const C3DAnimFileObjectChunk>& previous) const;
    bool HasHelpers() const;
    CCountedPointer<C3DAnimFileHelpersChunk> GetHelpers();
    CCountedPointer<const C3DAnimFileHelpersChunk> GetHelpers() const;
};

class C3DAnimFileTimingEventChunk : public CChunkedFileChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DAnimFileTimingEventChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "TMEV"; }
    C3DAnimFileTimingEventChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AAEAB0
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AAE190
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AAE440
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AAE220
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AAE250

    CCharString Name;  // +0x10
    float Time;        // +0x14
};

class C3DMeshFileMaterialExtChunk : public CChunkedFileChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileMaterialExtChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "MTLE"; }
    C3DMeshFileMaterialExtChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AC1920
    virtual const CCharString GetChunkInfo() const;         // retail 0x00ABFD70
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AC02D0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABFDE0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AC03B0

    // Ego_r accessors, out of line in retail (0x00ABFDC0 / 0x00ABFDD0).
    unsigned long GetSelfIllumination() const;
    void SetSelfIllumination(unsigned long value);

private:
    void ResetParams();  // retail 0x00ABFE10 (empty)

public:
    unsigned long SelfIllumination;  // +0x10
};

class C3DAnimFileBoneMaskChunk : public CChunkedFileChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DAnimFileBoneMaskChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "AMSK"; }
    C3DAnimFileBoneMaskChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AB0B30
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AB0110
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AB0790
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AB06B0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AB0730

    CArray<bool> EnableMask;  // +0x10
};

class C3DAnimFileXSequenceChunk : public CChunkedFileChunk
{
public:
    C3DAnimFileXSequenceChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA49D0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA48D0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA4650
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA46B0

    CArray<unsigned char> CompressedData;  // +0x10
};

class C3DAnimFileMovementVectorChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DAnimFileMovementVectorChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "MVEC"; }
    C3DAnimFileMovementVectorChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AAEE90
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AAE0F0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AAE2C0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AAE130
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AAE400

    void SetVector(const C3DVector& vector);  // Ego_r lib_3d_anim_file_helpers.cpp

    C3DVector Vector;  // +0x2C
    // Typed finders (retail 0x00AAE920..0x00AAEA00, Ego_r order).
    bool HasMovementTrack() const;
    CCountedPointer<C3DAnimFileSequenceChunk> GetMovementTrack();
    CCountedPointer<const C3DAnimFileSequenceChunk> GetMovementTrack() const;
};

class C3DMeshFileXBoundingSphereChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileXBoundingSphereChunk();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA4E40
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA7100
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA4B50
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA4FB0

    C3DVector Center;  // +0x10
    float Radius;      // +0x1C
};

class C3DMeshFileXAnimatedPrimitiveChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFileXAnimatedPrimitiveChunk();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA5370
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA7090
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA4CA0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA4CD0

    unsigned long BonesPerBlock;   // +0x2C
    unsigned long BonesPerVertex;  // +0x30
};

// Ego_r EMaterialMapType (values not recovered; -1 when unset).
enum EMaterialMapType { MATERIAL_MAP_TYPE_NONE = -1 };

class C3DMeshFileMapChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileMapChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "MMAP"; }
    C3DMeshFileMapChunk();
    // FableWin ?GetType@C3DMeshFileMapChunk@@QBE?AW4EMaterialMapType@@XZ; inline in retail.
    EMaterialMapType GetType() const { return MapType; }
    explicit C3DMeshFileMapChunk(EMaterialMapType type);  // retail 0x00ABFE20
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AC1290
    virtual const CCharString GetChunkInfo() const;         // retail 0x00ABFE80
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AC03F0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABFF70
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00ABFFB0

    // Ego_r inline accessors (by value).
    const CCharString GetMapDescription() const { return MapDescription; }
    const CCharString GetFilename() const { return Filename; }

    // Ego_r accessors, out of line in retail (0x00ABFF20-0x00ABFF60).
    void SetMapDescription(const CCharString& description);
    void SetFilename(const CCharString& filename);
    const unsigned long GetCRC() const;
    void SetCRC(unsigned long crc);
    void SetType(EMaterialMapType type);

    EMaterialMapType MapType;    // +0x2C
    CCharString MapDescription;  // +0x30
    CCharString Filename;        // +0x34
    unsigned long CRC;           // +0x38
};

class C3DMeshFileUniqueVerticesChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileUniqueVerticesChunk();
    // Ego_r ?GetStaticChunkType@C3DMeshFileUniqueVerticesChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "UNIV"; }
    virtual ~C3DMeshFileUniqueVerticesChunk();               // inlined into 0x00ABECF0
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABEAB0
    virtual const CCharString GetChunkInfo() const;  // retail 0x00AB9FA0 (empty)
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00ABCAF0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABC040
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00ABC5F0

    CArray<unsigned long> UniqueVerticesList;  // +0x10
    // List accessors (C3DMeshFilePrimitiveSubChunks_Accessors_00abc370.cpp).
    unsigned long FindHighestUniqueVertexIndex() const;
    void ClearList();
    void AddUniqueVertexIndex(unsigned long index);
};

class C3DMeshFileSmoothingGroupsChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileSmoothingGroupsChunk();
    // Ego_r ?GetStaticChunkType@C3DMeshFileSmoothingGroupsChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "SMTH"; }
    virtual ~C3DMeshFileSmoothingGroupsChunk();               // inlined into 0x00ABECB0
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABEA90
    virtual const CCharString GetChunkInfo() const;  // retail 0x00AB9F80 (empty)
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00ABC980
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABBFC0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00ABC500

    CArray<unsigned long> FaceGroupsList;  // +0x10
    // List accessors (C3DMeshFilePrimitiveSubChunks_Accessors_00abc370.cpp).
    void ClearList();
    void AddFaceGroupMask(unsigned long mask);
};

class C3DMeshFileXTriangleStripChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileXTriangleStripChunk();
    void Initialise(const CArray<unsigned short>& strip_vertices);  // Ego_r lib_3d_mesh_file_extras.cpp
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA7280
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA6960
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA5930
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA5A60

    CArray<unsigned short> StripVertices;  // +0x10
};

class C3DAnimFileObjectChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DAnimFileObjectChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "AOBJ"; }
    C3DAnimFileObjectChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AB0870
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AB01E0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AB0130
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AB0090
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AB00C0

    // Ego_r setters (lib_3d_anim_file_object.cpp).
    void SetName(const CCharString& name);
    void SetSubMeshIndex(long index);
    void SetParentIndex(long index);
    void SetFirstChildIndex(long index);
    void SetNextSiblingIndex(long index);

    CCharString Name;        // +0x2C
    long SubMeshIndex;       // +0x30
    long ParentIndex;        // +0x34
    long FirstChildIndex;    // +0x38
    long NextSiblingIndex;   // +0x3C
    // Typed finders (retail 0x00AB0260..0x00AB0620, Ego_r order).
    CCountedPointer<C3DAnimFileSequenceChunk> FindFirstSequence();
    CCountedPointer<C3DAnimFileSequenceChunk> FindNextSequence(const CCountedPointer<const C3DAnimFileSequenceChunk>& previous);
    CCountedPointer<const C3DAnimFileSequenceChunk> FindFirstSequence() const;
    CCountedPointer<const C3DAnimFileSequenceChunk> FindNextSequence(const CCountedPointer<const C3DAnimFileSequenceChunk>& previous) const;
    bool HasBoneMask() const;
    CCountedPointer<C3DAnimFileBoneMaskChunk> GetBoneMask();
    CCountedPointer<const C3DAnimFileBoneMaskChunk> PeekBoneMask() const;
};

class C3DMeshFileVerticesChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileVerticesChunk();
    virtual ~C3DMeshFileVerticesChunk();  // retail 0x00ABC7B0 (out of line; C3DMeshFileLoader's constructor calls it)
    // Ego_r ?GetStaticChunkType@C3DMeshFileVerticesChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "VERT"; }
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABEC70
    virtual const CCharString GetChunkInfo() const;         // retail 0x00ABBD70
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00ABD150
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABBDF0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00ABC800

    CArray<C3DVertex2> VertexList;  // +0x10
    // List accessors (C3DMeshFilePrimitiveSubChunks_Accessors_00abc370.cpp).
    unsigned long GetCount() const;
    const C3DVertex2* GetList() const;
    C3DVertex2* GetList();
    void ClearList();
    void AddVertex(const C3DVertex2& vertex);
};

class C3DMeshFileTrianglesChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFileTrianglesChunk();
    virtual ~C3DMeshFileTrianglesChunk();  // retail 0x00ABC890 (out of line; C3DMeshFileLoader's constructor calls it)
    // Ego_r ?GetStaticChunkType@C3DMeshFileTrianglesChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "TRIS"; }
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABEC90
    virtual const CCharString GetChunkInfo() const;         // retail 0x00ABBE90
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00ABD280
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABBF30
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00ABC8E0

    CArray<C3DTriangle2> TriangleList;  // +0x2C
    // List accessors (C3DMeshFilePrimitiveSubChunks_Accessors_00abc370.cpp).
    unsigned long GetCount() const;
    const C3DTriangle2* GetList() const;
    C3DTriangle2* GetList();
    void ClearList();
    void AddTriangle(const C3DTriangle2& triangle);
};

class C3DMeshFileGroupChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFileGroupChunk();
    // Ego_r ?GetStaticChunkType@C3DMeshFileGroupChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "VGRP"; }
    virtual ~C3DMeshFileGroupChunk();                        // inlined into 0x00ABEDD0
    virtual const CCharString GetChunkDescription() const;  // retail 0x00ABEC50
    virtual const CCharString GetChunkInfo() const;         // retail 0x00ABAE50
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00ABCC80
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABAEE0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00ABC6E0

    long BoneIndex;                       // +0x2C
    CArray<C3DVertexBlend2> VertexList;   // +0x30
    // List accessors (C3DMeshFilePrimitiveSubChunks_Accessors_00abc370.cpp).
    long GetBoneIndex() const;
    void SetBoneIndex(long index);
    unsigned long GetCount() const;
    const C3DVertexBlend2* GetList() const;
    C3DVertexBlend2* GetList();
    void ClearList();
    void AddVertex(const C3DVertexBlend2& vertex);
};

class C3DMeshFileConvexVolumeChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileConvexVolumeChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "HCVL"; }
    C3DMeshFileConvexVolumeChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AAB6B0
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AA9AD0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AAAB10
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AAA9A0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AAAA60

    // Ego_r, out of line in retail (0x00AA9B10 / 0x00AAAA40).
    void SetName(const CCharString& name);
    void ClearPlanes();
    void AddPlane(const CPlane& plane);

    CCharString Name;       // +0x2C
    CArray<CPlane> Planes;  // +0x30
};

class C3DMeshFileHelperPointChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileHelperPointChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "HPNT"; }
    C3DMeshFileHelperPointChunk();
    // Inline: derived helpers (C3DMeshFileDummyObjectChunk 0x00AA9E10) pass their own type.
    explicit C3DMeshFileHelperPointChunk(const char* chunk_type)
        : CChunkedFileSuperChunk(chunk_type), Position(0.0f, 0.0f, 0.0f), SubMeshIndex(-1), BoneIndex(-1)
    {
    }
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AAB130
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AA96B0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA9BA0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA98B0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA9DA0

    // Ego_r setters, out of line in retail (0x00AA9860-0x00AA98A0).
    void SetPosition(const C3DVector& position);
    void SetSubMeshIndex(long index);
    void SetBoneIndex(long index);
    void SetName(const CCharString& name);

    C3DVector Position;  // +0x2C
    long SubMeshIndex;   // +0x38
    long BoneIndex;      // +0x3C
    CCharString Name;    // +0x40
};

class C3DMeshFileDummyObjectChunk : public C3DMeshFileHelperPointChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileDummyObjectChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "HDMY"; }
    C3DMeshFileDummyObjectChunk();                          // retail 0x00AA9E10
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AAB180
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AA9910
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA9E60
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA9A70
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AAA0C0

    // Ego_r setters, out of line in retail (0x00AA9A20-0x00AA9A60).
    void SetDirection(const C3DVector& direction);
    void SetTransform(const CMatrix3x4& transform);
    void SetUseLocalOrigin(bool use_local_origin);

    C3DVector Direction;   // +0x44
    CMatrix3x4 Transform;  // +0x50
    bool UseLocalOrigin;   // +0x80
};

class C3DMeshFileMaterialChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileMaterialChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "MTRL"; }
    C3DMeshFileMaterialChunk();
    explicit C3DMeshFileMaterialChunk(long index);  // retail 0x00AC0A80
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AC1E70
    virtual const CCharString GetChunkInfo() const;         // retail 0x00ABFBF0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AC0010
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00ABFCF0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AC0FC0

    // Ego_r inline accessor (by value).
    const CCharString GetName() const { return Name; }

    // Ego_r setters, out of line in retail (0x00ABFC40-0x00ABFCC0).
    void SetMaterialIndex(long index);
    void SetName(const CCharString& name);
    void SetTwoSided(bool two_sided);
    void SetAmbient(CRGBColour colour);
    void SetDiffuse(CRGBColour colour);
    void SetSpecular(CRGBColour colour);
    void SetShiny(float shiny);
    void SetShinyStrength(float strength);
    void SetTransparency(float transparency);

private:
    void ResetParams();  // retail 0x00AC0250

public:
    long MaterialIndex;   // +0x2C
    bool TwoSided;        // +0x30
    // Ego_r's CRGBColour is a union with a dword (4-byte aligned); the shared
    // model is four chars, so the alignment padding is explicit here.
    unsigned char _pad_0x31[3];
    CRGBColour Ambient;   // +0x34
    CRGBColour Diffuse;   // +0x38
    CRGBColour Specular;  // +0x3C
    float Shiny;          // +0x40
    float ShinyStrength;  // +0x44
    float Transparency;   // +0x48
    CCharString Name;     // +0x4C
    // Typed finders (retail 0x00AC0AE0..0x00AC0EE0, Ego_r order).
    CCountedPointer<C3DMeshFileMaterialExtChunk> GetExtendedProperties();
    CCountedPointer<const C3DMeshFileMaterialExtChunk> GetExtendedProperties() const;
    CCountedPointer<C3DMeshFileMapChunk> FindFirstMap();
    CCountedPointer<C3DMeshFileMapChunk> FindNextMap(const CCountedPointer<const C3DMeshFileMapChunk>& previous);
    CCountedPointer<const C3DMeshFileMapChunk> FindFirstMap() const;
    CCountedPointer<const C3DMeshFileMapChunk> FindNextMap(const CCountedPointer<const C3DMeshFileMapChunk>& previous) const;
    // Material maps (C3DMeshFileMaterialChunk_Maps_00ac0c00.cpp).
    bool HasExtendedProperties() const;
    CCountedPointer<C3DMeshFileMapChunk> CreateMap(EMaterialMapType type);
    CCountedPointer<C3DMeshFileMapChunk> GetMap(EMaterialMapType type);
    CCountedPointer<const C3DMeshFileMapChunk> GetMap(EMaterialMapType type) const;
};

class C3DMeshFileXBoundingBoxChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileXBoundingBoxChunk();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA5030
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA7140
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA4B90
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA5250

    C3DVector Center;    // +0x10
    C3DVector Diagonal;  // +0x1C
};

// Ego_r EBoneType (values not recovered; 0 when new).
enum EBoneType { BONE_TYPE_DEFAULT = 0 };

class C3DAnimFileSequenceChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DAnimFileSequenceChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "ASEQ"; }
    C3DAnimFileSequenceChunk();
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AAFF90
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AAF5B0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AAF850
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AAF650
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AAF7A0

    // FableWin ?GetMatrixCount@ / ?GetSamplesPerSecond@C3DAnimFileSequenceChunk@@; inline in retail.
    unsigned long GetMatrixCount() const { return Matrices.size(); }
    float GetSamplesPerSecond() const { return SamplesPerSecond; }
    // Ego_r setters (retail 0x00AAF570 SetBoneName, 0x00AAF580 SetBoneIndex, 0x00AAF590 SetParentIndex,
    // 0x00AAF5A0 SetSamplesPerSecond); C3DAnimationBuilder fills a sequence chunk through them.
    void SetBoneName(const CCharString& name);
    void SetBoneIndex(long index);
    void SetParentIndex(long index);
    void SetSamplesPerSecond(float samplesPerSecond);

    long BoneIndex;              // +0x2C
    long ParentIndex;            // +0x30
    CCharString BoneName;        // +0x34
    float SamplesPerSecond;      // +0x38
    EBoneType BoneType;          // +0x3C
    CArray<CMatrix3x4> Matrices; // +0x40
};

class C3DMeshFileClothChunk : public CChunkedFileSuperChunk
{
public:
    C3DMeshFileClothChunk();
    // Ego_r ?GetStaticChunkType@C3DMeshFileClothChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "CLTH"; }
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AC69B0
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AC62B0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AC6750
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AC6600
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AC66E0

    CCharString Name;           // +0x2C
    long MaterialIndex;         // +0x30
    CCharString ParticleCode;   // +0x34
    CArray<char> CompiledCode;  // +0x38
    const CCharString GetName() const;                    // 0x00AC62D0
    void SetName(const CCharString& name);                // 0x00AC62F0
    void SetParticleCode(const CCharString& code);        // 0x00AC6300
    void SetMaterialIndex(long index);                    // 0x00AC6310
    // Typed finders (retail 0x00AC6320..0x00AC6520, Ego_r order).
    CCountedPointer<C3DMeshFileGroupChunk> FindFirstGroup();
    CCountedPointer<C3DMeshFileGroupChunk> FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous);
    CCountedPointer<const C3DMeshFileGroupChunk> FindFirstGroup() const;
    CCountedPointer<const C3DMeshFileGroupChunk> FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous) const;
};

class C3DMeshFileXUVTangentChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileXUVTangentChunk();
    void ClearTangents();
    void AddTangent(const C3DVector& u_tangent, const C3DVector& v_tangent);
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA7350
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA6A60
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA57F0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA5B30

    CArray<C3DVector> UTangents;  // +0x10
    CArray<C3DVector> VTangents;  // +0x1C
};

class C3DMeshFileEngineMaterialChunk : public CChunkedFileSuperChunk
{
public:
    // Ego_r ?GetStaticChunkType@C3DMeshFileEngineMaterialChunk@@SIPBDXZ (inline in retail).
    static const char* FABLE_FASTCALL GetStaticChunkType() { return "EMTL"; }
    // Ego_r C3DMeshFileEngineMaterialChunk::CMapFilenames (0x18).
    class CMapFilenames : public CIVCountedPointeeBase
    {
    public:
        CMapFilenames();  // retail 0x00AC5190
        CCharString DiffuseMap;       // +0x08
        CCharString BumpMap;          // +0x0C
        CCharString ReflectionMap;    // +0x10
        CCharString IlluminationMap;  // +0x14
    };

    C3DMeshFileEngineMaterialChunk();                       // retail 0x00AC56F0
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AC6270
    virtual const CCharString GetChunkInfo() const;         // retail 0x00AC51D0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AC5770
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AC52F0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AC6180

    // Ego_r inline accessor (by value).
    const CCharString GetName() const { return Name; }
    void RemoveMapFilenames();  // 0x00AC5740

private:
    // Bit per map present (1 diffuse, 2 bump, 4 reflection, 8 illumination):
    // from the filenames when loaded by name, else the bank indices. 0x00AC5260.
    void CalculateMapFlags();

public:

    long MaterialIndex;                             // +0x2C
    CCharString Name;                               // +0x30
    CIVCountedPointer<CMapFilenames> MapFilenames;  // +0x34
    unsigned long DiffuseMapBankIndex;              // +0x38
    unsigned long BumpMapBankIndex;                 // +0x3C
    unsigned long ReflectionMapBankIndex;           // +0x40
    unsigned long IlluminationMapBankIndex;         // +0x44
    unsigned long MapFlags;                         // +0x48
    unsigned long SelfIllumination;                 // +0x4C
    bool TwoSided;                                  // +0x50
    bool Transparent;                               // +0x51
    bool BooleanAlpha;                              // +0x52
    bool DegenerateTriangles;                       // +0x53
};

// A chunk of a type nobody registered: its bytes are kept as they are.
class CChunkedFileRawDataChunk : public CChunkedFileChunk
{
public:
    explicit CChunkedFileRawDataChunk(const char* chunk_type);  // retail 0x00AA76D0
    void* GetData() const;                                      // retail 0x00AA7750
    void SetData(const void* data, unsigned long size);         // retail 0x00AA7760
    virtual ~CChunkedFileRawDataChunk();                    // inlined into 0x00AA8D80
    virtual int IsUnknownType() const;                      // retail 0x00AA7740
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA77A0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA7830
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA7850

    unsigned char* Data;     // +0x10
    unsigned long ByteSize;  // +0x14
};

// Ego_r ?WriteToFile@@YIXABVCVertexCompressionParams@@AAVCDataOutputStream@@@Z,
// retail 0x00AA4D20; ?ReadFromFile@@YIXAAVCVertexCompressionParams@@AAVCDataInputStream@@@Z.
void FABLE_FASTCALL WriteToFile(const CVertexCompressionParams& params, CDataOutputStream& stream);
void FABLE_FASTCALL ReadFromFile(CVertexCompressionParams& params, CDataInputStream& stream);

class C3DMeshFileXVertexDataChunk : public CChunkedFileChunk
{
public:
    C3DMeshFileXVertexDataChunk();
    void SetVertexData(unsigned long format_id, const void* data, unsigned long vertex_count, unsigned long vertex_size, unsigned long init_flags);
    virtual const CCharString GetChunkDescription() const;  // retail 0x00AA7230
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AA66B0
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AA58C0
    virtual CChunkedFileChunk* CreateNewInstance() const;   // retail 0x00AA59C0

    unsigned long FormatID;                // +0x10
    unsigned long VertexCount;             // +0x14
    unsigned long VertexSize;              // +0x18
    unsigned long InitFlags;               // +0x1C
    CVertexCompressionParams VBCParams;    // +0x20
    CArray<char> VertexData;               // +0x40
};

FABLE_STATIC_ASSERT(sizeof(CChunkedFileChunk) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CChunkedFileSuperChunk) == 0x2C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileSubMeshChunk) == 0x40);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileTransformationChunk) == 0x5C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileBoneChunk) == 0x70);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFilePrimitiveChunk) == 0x38);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXTextureIndexChunk) == 0x14);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXMappingInfoChunk) == 0x14);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFilePartitionChunk) == 0x18);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileXAllocationSizeChunk) == 0x14);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileRootChunk) == 0x34);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileTimingEventChunk) == 0x18);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileMaterialExtChunk) == 0x14);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileBoneMaskChunk) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileXSequenceChunk) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileMovementVectorChunk) == 0x38);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXBoundingSphereChunk) == 0x20);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXAnimatedPrimitiveChunk) == 0x34);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileMapChunk) == 0x3C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileUniqueVerticesChunk) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileSmoothingGroupsChunk) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXTriangleStripChunk) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileObjectChunk) == 0x40);
FABLE_STATIC_ASSERT(sizeof(C3DVertex2) == 0x20);
FABLE_STATIC_ASSERT(sizeof(C3DTriangle2) == 6);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileVerticesChunk) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileTrianglesChunk) == 0x38);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileGroupChunk) == 0x3C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileConvexVolumeChunk) == 0x3C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileHelperPointChunk) == 0x44);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileDummyObjectChunk) == 0x84);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileMaterialChunk) == 0x50);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXBoundingBoxChunk) == 0x28);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileSequenceChunk) == 0x4C);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileClothChunk) == 0x44);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXUVTangentChunk) == 0x28);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileEngineMaterialChunk::CMapFilenames) == 0x18);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileEngineMaterialChunk) == 0x54);
// Ego_r: thrown by CChunkedFileLoader::LoadChunkedFile on a bad header
// (retail throw info 0x01369548; derives from CExceptionBase).
class CChunkedFileException : public CExceptionBase {};

// Ego_r lib_chunked_file.cpp's file object and the two stream drivers
// (vtables 0x0129E4E8 / 0x0129E464 / 0x0129E46C).
class CChunkedFile : public CBaseClassNonCopyable
{
public:
    explicit CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root_chunk);  // 0x00AA7DB0
    virtual ~CChunkedFile();                                                          // 0x00AA7DF0

    // FableWin ?SetFileComment@CChunkedFile@@ / ?SetVersion@CChunkedFile@@; inline in retail.
    void SetFileComment(const CCharString& comment) { FileComment = comment; }
    void SetVersion(unsigned long version) { Version = version; }

    CCountedPointer<CChunkedFileSuperChunk> RootChunk;  // +0x04
    unsigned long Version;                              // +0x0C
    CCharString FileComment;                            // +0x10
};

class CChunkedFileLoader
{
public:
    explicit CChunkedFileLoader(const char* file_type);  // 0x00AA7510
    virtual ~CChunkedFileLoader();                       // 0x00AA7540

protected:
    void LoadChunkedFile(CChunkedFile& file, CDataInputStream& stream);  // 0x00AA79C0

public:
    int ReadUnknownChunkTypes;   // +0x04
    char SupportedFileType[5];   // +0x08
};

class CChunkedFileSaver
{
public:
    explicit CChunkedFileSaver(const char* file_type);  // 0x00AA7550
    virtual ~CChunkedFileSaver();                       // 0x00AA7580

protected:
    unsigned long SaveChunkedFile(CChunkedFile& file, CDataOutputStream& stream);  // 0x00AA7C70

public:
    char SupportedFileType[5];  // +0x04
};

FABLE_STATIC_ASSERT(sizeof(CChunkedFileRawDataChunk) == 0x18);
FABLE_STATIC_ASSERT(sizeof(CChunkedFile) == 0x14);
// Ego_r C3DAnimFile (a CChunkedFile) and its saver (file type "3DAF", vtable
// 0x0129DFE0).
class C3DAnimFile : public CChunkedFile
{
public:
    C3DAnimFile();                                                                // 0x00A98E60
    explicit C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root_chunk); // 0x00A98F40
};
class C3DAnimFileSaver : public CChunkedFileSaver
{
public:
    C3DAnimFileSaver();                                                      // 0x00A98A00
    unsigned long SaveAnimFile(C3DAnimFile& file, CDataOutputStream& stream); // 0x00A98A20
    unsigned long SaveAnimFileCompressed(C3DAnimFile& data, CDataOutputStream& os); // 0x00A99840
};
// Ego_r C3DAnimFileLoader (lib_3d_anim_file.cpp): the registered chunk types
// (prototypes cloned into their parent's sub-chunk types) and all of them.
class C3DAnimFileLoader : public CChunkedFileLoader
{
public:
    C3DAnimFileLoader();  // 0x00A999B0
    virtual ~C3DAnimFileLoader();  // 0x00A996D0

    CCountedPointer<C3DAnimFile> LoadAnimFile(CDataInputStream& is);  // 0x00A99420
    CCountedPointer<C3DAnimFile> LoadAnimFileCompressed(const void* src, unsigned long src_size, CHandleBasedMemoryPool& memory_pool);  // 0x00A99510

    CCountedPointer<CChunkedFileChunk> RegisterRootSubChunkType(const CChunkedFileChunk& clonable_chunk);           // 0x00A98BE0
    CCountedPointer<CChunkedFileChunk> RegisterObjectSubChunkType(const CChunkedFileChunk& clonable_chunk);         // 0x00A98C80
    CCountedPointer<CChunkedFileChunk> RegisterHelpersSubChunkType(const CChunkedFileChunk& clonable_chunk);        // 0x00A98D20
    CCountedPointer<CChunkedFileChunk> RegisterMovementVectorSubChunkType(const CChunkedFileChunk& clonable_chunk); // 0x00A98DC0

    CCountedPointer<C3DAnimFileRootChunk> RootChunkType;          // +0x10
    CCountedPointer<CChunkedFileChunk> ObjectChunkType;           // +0x18
    CCountedPointer<CChunkedFileChunk> SequenceChunkType;         // +0x20
    CCountedPointer<CChunkedFileChunk> BoneMaskChunkType;         // +0x28
    CCountedPointer<CChunkedFileChunk> HelpersChunkType;          // +0x30
    CCountedPointer<CChunkedFileChunk> MovementVectorChunkType;   // +0x38
    CCountedPointer<CChunkedFileChunk> MovementTrackChunkType;    // +0x40
    CCountedPointer<CChunkedFileChunk> TimingEventChunkType;      // +0x48
    CArray<CChunkedFileChunk*> ChunkTypes;                        // +0x50
};
// Ego_r CMirroredTextureHighlightsChunk ("MTEH", vtable 0x0129E0E0;
// lib_mirrored_texture_highlights_chunk.cpp/.hpp): the vertex indices of a
// primitive's mirrored-texture highlights. The constructor is inline in the
// header (expanded into C3DMeshFileLoader's constructor 0x00A9BBE0).
class CMirroredTextureHighlightsChunk : public CChunkedFileChunk
{
public:
    CMirroredTextureHighlightsChunk() : CChunkedFileChunk("MTEH") {}
    virtual ~CMirroredTextureHighlightsChunk() {}
    // Inline in the .hpp (retail COMDAT 0x00A9D310, emitted with the vtable).
    virtual const CCharString GetChunkDescription() const { return CCharString("Mirrored Texture Triangle List"); }
    virtual const CCharString GetChunkInfo() const;  // retail 0x00AC4EC0
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);  // 0x00AC5080
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                  // 0x00AC4F50
    virtual CChunkedFileChunk* CreateNewInstance() const;  // retail 0x00AC4FE0

    unsigned long GetCount() const;                      // 0x00AC4F20
    const unsigned long* GetList() const;                // 0x00AC4F30
    unsigned long* GetList();                            // 0x00AC4F90
    void ClearList();                                    // 0x00AC4FB0
    void AddVertexIndex(const unsigned long& vertIndex); // 0x00AC5040

    CArray<unsigned long> TriangleVertexIndexList;  // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CMirroredTextureHighlightsChunk) == 0x1C);  // Ego_r 0x20 with its 16-byte vector

// Ego_r C3DMeshFile (a CChunkedFile; vtable 0x0129E0D8) and its saver (file
// type "3DMF", vtable 0x0129E080), lib_3d_mesh_file.cpp.
class C3DMeshFile : public CChunkedFile
{
public:
    C3DMeshFile();                                                                 // 0x00A9AD80
    explicit C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root_chunk);  // 0x00A9AE60
};
class C3DMeshFileSaver : public CChunkedFileSaver
{
public:
    C3DMeshFileSaver();                                                                // 0x00A9A870
    unsigned long SaveMeshFile(C3DMeshFile& data, CDataOutputStream& os);             // 0x00A9A890
    unsigned long SaveMeshFileCompressed(C3DMeshFile& data, CDataOutputStream& os);   // 0x00A9BA70
};
// Ego_r C3DAnimFileXLoader (lib_3d_anim_file_x.cpp): the anim loader plus the
// compressed-sequence and allocation-size chunk types.
class C3DAnimFileXLoader : public C3DAnimFileLoader
{
public:
    C3DAnimFileXLoader();  // 0x00AA4710
};

// Ego_r C3DMeshFileLoader (lib_3d_mesh_file.cpp): one registered prototype per
// chunk type; Register<X>SubChunkType adds a sub-chunk type to X.
class C3DMeshFileLoader : public CChunkedFileLoader
{
public:
    C3DMeshFileLoader();           // 0x00A9BBE0
    virtual ~C3DMeshFileLoader();  // 0x00A9A900

    CCountedPointer<C3DMeshFile> LoadMeshFile(CDataInputStream& is);  // 0x00A9B640
    CCountedPointer<C3DMeshFile> LoadMeshFileCompressed(const void* src, unsigned long src_size, CHandleBasedMemoryPool& memory_pool);  // 0x00A9B740

    CCountedPointer<CChunkedFileChunk> RegisterRootSubChunkType(const CChunkedFileChunk& clonable_chunk);          // 0x00A9B840
    CCountedPointer<CChunkedFileChunk> RegisterSubMeshSubChunkType(const CChunkedFileChunk& clonable_chunk);       // 0x00A9B870
    CCountedPointer<CChunkedFileChunk> RegisterPrimitiveSubChunkType(const CChunkedFileChunk& clonable_chunk);     // 0x00A9B8A0
    CCountedPointer<CChunkedFileChunk> RegisterTrianglesSubChunkType(const CChunkedFileChunk& clonable_chunk);     // 0x00A9B8D0
    CCountedPointer<CChunkedFileChunk> RegisterGroupSubChunkType(const CChunkedFileChunk& clonable_chunk);         // 0x00A9B900
    void RegisterClothSubChunkType(const CChunkedFileChunk& clonable_chunk);                                       // 0x00A9B930
    CCountedPointer<CChunkedFileChunk> RegisterMaterialListSubChunkType(const CChunkedFileChunk& clonable_chunk);  // 0x00A9B9B0
    CCountedPointer<CChunkedFileChunk> RegisterMaterialSubChunkType(const CChunkedFileChunk& clonable_chunk);      // 0x00A9B9E0
    CCountedPointer<CChunkedFileChunk> RegisterMapSubChunkType(const CChunkedFileChunk& clonable_chunk);           // 0x00A9BA10
    CCountedPointer<CChunkedFileChunk> RegisterHelpersSubChunkType(const CChunkedFileChunk& clonable_chunk);       // 0x00A9BA40

private:
    CCountedPointer<CChunkedFileChunk> AddChunkType(const CChunkedFileChunk& chunk_type, CChunkedFileSuperChunk* parent);  // 0x00A9AD00

public:
    CCountedPointer<C3DMeshFileRootChunk> RootChunkType;           // +0x10
    CCountedPointer<CChunkedFileChunk> SubMeshChunkType;           // +0x18
    CCountedPointer<CChunkedFileChunk> TransformationChunkType;    // +0x20
    CCountedPointer<CChunkedFileChunk> BoneChunkType;              // +0x28
    CCountedPointer<CChunkedFileChunk> PrimitiveChunkType;         // +0x30
    CCountedPointer<CChunkedFileChunk> TrianglesChunkType;         // +0x38
    CCountedPointer<CChunkedFileChunk> SmoothingGroupsChunkType;   // +0x40
    CCountedPointer<CChunkedFileChunk> VerticesChunkType;          // +0x48
    CCountedPointer<CChunkedFileChunk> UniqueVerticesChunkType;    // +0x50
    CCountedPointer<CChunkedFileChunk> GroupChunkType;             // +0x58
    CCountedPointer<CChunkedFileChunk> PrimitiveClothChunkType;    // +0x60
    CCountedPointer<CChunkedFileChunk> SubMeshClothChunkType;      // +0x68
    CCountedPointer<CChunkedFileChunk> MaterialListChunkType;      // +0x70
    CCountedPointer<CChunkedFileChunk> MaterialChunkType;          // +0x78
    CCountedPointer<CChunkedFileChunk> MaterialExtChunkType;       // +0x80
    CCountedPointer<CChunkedFileChunk> MapChunkType;               // +0x88
    CCountedPointer<CChunkedFileChunk> EngineMaterialChunkType;    // +0x90
    CCountedPointer<CChunkedFileChunk> HelpersChunkType;           // +0x98
    CCountedPointer<CChunkedFileChunk> HelperPointChunkType;       // +0xA0
    CCountedPointer<CChunkedFileChunk> DummyObjectChunkType;       // +0xA8
    CCountedPointer<CChunkedFileChunk> ConvexVolumeChunkType;      // +0xB0
    CCountedPointer<CChunkedFileChunk> MirroredTextureChunkType;   // +0xB8
};
FABLE_STATIC_ASSERT(sizeof(CChunkedFileLoader) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CChunkedFileSaver) == 0x0C);
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileLoader) == 0x5C);  // Ego_r 0x60 with its 16-byte vector
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileLoader) == 0xC0);
FABLE_STATIC_ASSERT(sizeof(CVertexCompressionParams) == 0x20);
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileXVertexDataChunk) == 0x4C);
