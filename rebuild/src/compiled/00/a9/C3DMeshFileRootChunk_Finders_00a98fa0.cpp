// The mesh/anim-file chunk classes' typed finders (Ego_r lib_3d_mesh_file*.cpp /
// lib_3d_anim_file*.cpp), retail 0x00A98FA0-0x00AC6520: each shares the base
// class's search result as the derived chunk type (CCountedPointer::SetStaticCast,
// as in FableWin) and returns it by value; the short Has* forms test the
// search result directly.
#include "fable_chunked_file.h"
#include "fable_counted_pointer_inline.h"

// Retail 00A98FA0.
CCountedPointer<C3DAnimFileObjectChunk> C3DAnimFileRootChunk::FindFirstObject()
{
    CCountedPointer<C3DAnimFileObjectChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileObjectChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A99030.
CCountedPointer<C3DAnimFileObjectChunk> C3DAnimFileRootChunk::FindNextObject(const CCountedPointer<const C3DAnimFileObjectChunk>& previous)
{
    CCountedPointer<C3DAnimFileObjectChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00A99110.
CCountedPointer<const C3DAnimFileObjectChunk> C3DAnimFileRootChunk::FindFirstObject() const
{
    CCountedPointer<const C3DAnimFileObjectChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileObjectChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A991A0.
CCountedPointer<const C3DAnimFileObjectChunk> C3DAnimFileRootChunk::FindNextObject(const CCountedPointer<const C3DAnimFileObjectChunk>& previous) const
{
    CCountedPointer<const C3DAnimFileObjectChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00A99280.
bool C3DAnimFileRootChunk::HasHelpers() const
{
    CCountedPointer<const C3DAnimFileHelpersChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileHelpersChunk::GetStaticChunkType()));
    return !chunk.IsNull();
}

// Retail 00A99300.
CCountedPointer<C3DAnimFileHelpersChunk> C3DAnimFileRootChunk::GetHelpers()
{
    CCountedPointer<C3DAnimFileHelpersChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileHelpersChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A99390.
CCountedPointer<const C3DAnimFileHelpersChunk> C3DAnimFileRootChunk::GetHelpers() const
{
    CCountedPointer<const C3DAnimFileHelpersChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileHelpersChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A9AEC0.
CCountedPointer<C3DMeshFileSubMeshChunk> C3DMeshFileRootChunk::FindFirstSubMesh()
{
    CCountedPointer<C3DMeshFileSubMeshChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileSubMeshChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A9AF50.
CCountedPointer<C3DMeshFileSubMeshChunk> C3DMeshFileRootChunk::FindNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>& previous)
{
    CCountedPointer<C3DMeshFileSubMeshChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00A9B030.
CCountedPointer<const C3DMeshFileSubMeshChunk> C3DMeshFileRootChunk::FindFirstSubMesh() const
{
    CCountedPointer<const C3DMeshFileSubMeshChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileSubMeshChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A9B0C0.
CCountedPointer<const C3DMeshFileSubMeshChunk> C3DMeshFileRootChunk::FindNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileSubMeshChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00A9B1A0.
CCountedPointer<C3DMeshFileMaterialListChunk> C3DMeshFileRootChunk::GetMaterialList()
{
    CCountedPointer<C3DMeshFileMaterialListChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileMaterialListChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A9B230.
CCountedPointer<const C3DMeshFileMaterialListChunk> C3DMeshFileRootChunk::GetMaterialList() const
{
    CCountedPointer<const C3DMeshFileMaterialListChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileMaterialListChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A9B2C0.
bool C3DMeshFileRootChunk::HasHelpers() const
{
    CCountedPointer<const C3DMeshFileHelpersChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileHelpersChunk::GetStaticChunkType()));
    return !chunk.IsNull();
}

// Retail 00A9B340.
CCountedPointer<C3DMeshFileHelpersChunk> C3DMeshFileRootChunk::GetHelpers()
{
    CCountedPointer<C3DMeshFileHelpersChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileHelpersChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00A9B3D0.
CCountedPointer<const C3DMeshFileHelpersChunk> C3DMeshFileRootChunk::GetHelpers() const
{
    CCountedPointer<const C3DMeshFileHelpersChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileHelpersChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAA100.
CCountedPointer<C3DMeshFileHelperPointChunk> C3DMeshFileHelpersChunk::FindFirstPoint()
{
    CCountedPointer<C3DMeshFileHelperPointChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileHelperPointChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAA190.
CCountedPointer<C3DMeshFileHelperPointChunk> C3DMeshFileHelpersChunk::FindNextPoint(const CCountedPointer<const C3DMeshFileHelperPointChunk>& previous)
{
    CCountedPointer<C3DMeshFileHelperPointChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AAA270.
CCountedPointer<const C3DMeshFileHelperPointChunk> C3DMeshFileHelpersChunk::FindFirstPoint() const
{
    CCountedPointer<const C3DMeshFileHelperPointChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileHelperPointChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAA300.
CCountedPointer<const C3DMeshFileHelperPointChunk> C3DMeshFileHelpersChunk::FindNextPoint(const CCountedPointer<const C3DMeshFileHelperPointChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileHelperPointChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AAA3E0.
CCountedPointer<C3DMeshFileDummyObjectChunk> C3DMeshFileHelpersChunk::FindFirstDummy()
{
    CCountedPointer<C3DMeshFileDummyObjectChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileDummyObjectChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAA470.
CCountedPointer<C3DMeshFileDummyObjectChunk> C3DMeshFileHelpersChunk::FindNextDummy(const CCountedPointer<const C3DMeshFileDummyObjectChunk>& previous)
{
    CCountedPointer<C3DMeshFileDummyObjectChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AAA550.
CCountedPointer<const C3DMeshFileDummyObjectChunk> C3DMeshFileHelpersChunk::FindFirstDummy() const
{
    CCountedPointer<const C3DMeshFileDummyObjectChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileDummyObjectChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAA5E0.
CCountedPointer<const C3DMeshFileDummyObjectChunk> C3DMeshFileHelpersChunk::FindNextDummy(const CCountedPointer<const C3DMeshFileDummyObjectChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileDummyObjectChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AAA6C0.
CCountedPointer<C3DMeshFileConvexVolumeChunk> C3DMeshFileHelpersChunk::FindFirstVolumeW()
{
    CCountedPointer<C3DMeshFileConvexVolumeChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileConvexVolumeChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAA750.
CCountedPointer<C3DMeshFileConvexVolumeChunk> C3DMeshFileHelpersChunk::FindNextVolumeW(const CCountedPointer<const C3DMeshFileConvexVolumeChunk>& previous)
{
    CCountedPointer<C3DMeshFileConvexVolumeChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AAA830.
CCountedPointer<const C3DMeshFileConvexVolumeChunk> C3DMeshFileHelpersChunk::FindFirstVolumeW() const
{
    CCountedPointer<const C3DMeshFileConvexVolumeChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileConvexVolumeChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAA8C0.
CCountedPointer<const C3DMeshFileConvexVolumeChunk> C3DMeshFileHelpersChunk::FindNextVolumeW(const CCountedPointer<const C3DMeshFileConvexVolumeChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileConvexVolumeChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AAE4D0.
bool C3DAnimFileHelpersChunk::HasMovementVector() const
{
    return !FindFirstSubChunkOfType(C3DAnimFileMovementVectorChunk::GetStaticChunkType()).IsNull();
}

// Retail 00AAE520.
CCountedPointer<C3DAnimFileMovementVectorChunk> C3DAnimFileHelpersChunk::GetMovementVector()
{
    CCountedPointer<C3DAnimFileMovementVectorChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileMovementVectorChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAE5B0.
CCountedPointer<const C3DAnimFileMovementVectorChunk> C3DAnimFileHelpersChunk::GetMovementVector() const
{
    CCountedPointer<const C3DAnimFileMovementVectorChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileMovementVectorChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAE640.
CCountedPointer<C3DAnimFileTimingEventChunk> C3DAnimFileHelpersChunk::FindFirstEvent()
{
    CCountedPointer<C3DAnimFileTimingEventChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileTimingEventChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAE6D0.
CCountedPointer<C3DAnimFileTimingEventChunk> C3DAnimFileHelpersChunk::FindNextEvent(const CCountedPointer<const C3DAnimFileTimingEventChunk>& previous)
{
    CCountedPointer<C3DAnimFileTimingEventChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AAE7B0.
CCountedPointer<const C3DAnimFileTimingEventChunk> C3DAnimFileHelpersChunk::FindFirstEvent() const
{
    CCountedPointer<const C3DAnimFileTimingEventChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileTimingEventChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAE840.
CCountedPointer<const C3DAnimFileTimingEventChunk> C3DAnimFileHelpersChunk::FindNextEvent(const CCountedPointer<const C3DAnimFileTimingEventChunk>& previous) const
{
    CCountedPointer<const C3DAnimFileTimingEventChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AAE920.
bool C3DAnimFileMovementVectorChunk::HasMovementTrack() const
{
    return !FindFirstSubChunkOfType(C3DAnimFileSequenceChunk::GetStaticChunkType()).IsNull();
}

// Retail 00AAE970.
CCountedPointer<C3DAnimFileSequenceChunk> C3DAnimFileMovementVectorChunk::GetMovementTrack()
{
    CCountedPointer<C3DAnimFileSequenceChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileSequenceChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AAEA00.
CCountedPointer<const C3DAnimFileSequenceChunk> C3DAnimFileMovementVectorChunk::GetMovementTrack() const
{
    CCountedPointer<const C3DAnimFileSequenceChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileSequenceChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AB0260.
CCountedPointer<C3DAnimFileSequenceChunk> C3DAnimFileObjectChunk::FindFirstSequence()
{
    CCountedPointer<C3DAnimFileSequenceChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileSequenceChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AB02F0.
CCountedPointer<C3DAnimFileSequenceChunk> C3DAnimFileObjectChunk::FindNextSequence(const CCountedPointer<const C3DAnimFileSequenceChunk>& previous)
{
    CCountedPointer<C3DAnimFileSequenceChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AB03D0.
CCountedPointer<const C3DAnimFileSequenceChunk> C3DAnimFileObjectChunk::FindFirstSequence() const
{
    CCountedPointer<const C3DAnimFileSequenceChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileSequenceChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AB0460.
CCountedPointer<const C3DAnimFileSequenceChunk> C3DAnimFileObjectChunk::FindNextSequence(const CCountedPointer<const C3DAnimFileSequenceChunk>& previous) const
{
    CCountedPointer<const C3DAnimFileSequenceChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AB0540: unlike the other short forms, retail keeps the pointer and
// tests it after the search result is released.
bool C3DAnimFileObjectChunk::HasBoneMask() const
{
    const CChunkedFileChunk* chunk = FindFirstSubChunkOfType(C3DAnimFileBoneMaskChunk::GetStaticChunkType()).Get();
    return chunk ? true : false;
}

// Retail 00AB0590.
CCountedPointer<C3DAnimFileBoneMaskChunk> C3DAnimFileObjectChunk::GetBoneMask()
{
    CCountedPointer<C3DAnimFileBoneMaskChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileBoneMaskChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AB0620.
CCountedPointer<const C3DAnimFileBoneMaskChunk> C3DAnimFileObjectChunk::PeekBoneMask() const
{
    CCountedPointer<const C3DAnimFileBoneMaskChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileBoneMaskChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC04C0.
CCountedPointer<C3DMeshFileMaterialChunk> C3DMeshFileMaterialListChunk::FindFirstMaterial()
{
    CCountedPointer<C3DMeshFileMaterialChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileMaterialChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC0550.
CCountedPointer<C3DMeshFileMaterialChunk> C3DMeshFileMaterialListChunk::FindNextMaterial(const CCountedPointer<const C3DMeshFileMaterialChunk>& previous)
{
    CCountedPointer<C3DMeshFileMaterialChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AC0630.
CCountedPointer<const C3DMeshFileMaterialChunk> C3DMeshFileMaterialListChunk::FindFirstMaterial() const
{
    CCountedPointer<const C3DMeshFileMaterialChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileMaterialChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC06C0.
CCountedPointer<const C3DMeshFileMaterialChunk> C3DMeshFileMaterialListChunk::FindNextMaterial(const CCountedPointer<const C3DMeshFileMaterialChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileMaterialChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AC07A0.
CCountedPointer<C3DMeshFileEngineMaterialChunk> C3DMeshFileMaterialListChunk::FindFirstEngineMaterial()
{
    CCountedPointer<C3DMeshFileEngineMaterialChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileEngineMaterialChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC0830.
CCountedPointer<C3DMeshFileEngineMaterialChunk> C3DMeshFileMaterialListChunk::FindNextEngineMaterial(const CCountedPointer<const C3DMeshFileEngineMaterialChunk>& previous)
{
    CCountedPointer<C3DMeshFileEngineMaterialChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AC0910.
CCountedPointer<const C3DMeshFileEngineMaterialChunk> C3DMeshFileMaterialListChunk::FindFirstEngineMaterial() const
{
    CCountedPointer<const C3DMeshFileEngineMaterialChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileEngineMaterialChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC09A0.
CCountedPointer<const C3DMeshFileEngineMaterialChunk> C3DMeshFileMaterialListChunk::FindNextEngineMaterial(const CCountedPointer<const C3DMeshFileEngineMaterialChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileEngineMaterialChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AC0AE0.
CCountedPointer<C3DMeshFileMaterialExtChunk> C3DMeshFileMaterialChunk::GetExtendedProperties()
{
    CCountedPointer<C3DMeshFileMaterialExtChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileMaterialExtChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC0B70.
CCountedPointer<const C3DMeshFileMaterialExtChunk> C3DMeshFileMaterialChunk::GetExtendedProperties() const
{
    CCountedPointer<const C3DMeshFileMaterialExtChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileMaterialExtChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC0CE0.
CCountedPointer<C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::FindFirstMap()
{
    CCountedPointer<C3DMeshFileMapChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileMapChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC0D70.
CCountedPointer<C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::FindNextMap(const CCountedPointer<const C3DMeshFileMapChunk>& previous)
{
    CCountedPointer<C3DMeshFileMapChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AC0E50.
CCountedPointer<const C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::FindFirstMap() const
{
    CCountedPointer<const C3DMeshFileMapChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileMapChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC0EE0.
CCountedPointer<const C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::FindNextMap(const CCountedPointer<const C3DMeshFileMapChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileMapChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AC6320.
CCountedPointer<C3DMeshFileGroupChunk> C3DMeshFileClothChunk::FindFirstGroup()
{
    CCountedPointer<C3DMeshFileGroupChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileGroupChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC63B0.
CCountedPointer<C3DMeshFileGroupChunk> C3DMeshFileClothChunk::FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous)
{
    CCountedPointer<C3DMeshFileGroupChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00AC6490.
CCountedPointer<const C3DMeshFileGroupChunk> C3DMeshFileClothChunk::FindFirstGroup() const
{
    CCountedPointer<const C3DMeshFileGroupChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileGroupChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00AC6520.
CCountedPointer<const C3DMeshFileGroupChunk> C3DMeshFileClothChunk::FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileGroupChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}
