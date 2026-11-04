#pragma once
// Hand-owned (retail model): C3DGroup2, a mesh bone group - the bone index and the vertex blends it drives. Ego_r.pdb
// names, members and access (0x14 there; retail's STLport CArray is 12 bytes, so 0x10). Partial view: only members
// with a retail body are declared.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_chunked_file.h"   // C3DVertexBlend2

class C3DMeshFileGroupChunk;

class C3DGroup2
{
public:
    // Ego_r ??0C3DGroup2@@QAE@ABVC3DMeshFileGroupChunk@@@Z (retail 0x00AC4150, not yet reconstructed).
    explicit C3DGroup2(const C3DMeshFileGroupChunk& chunk);
    // Ego_r ??0C3DGroup2@@QAE@XZ (retail 0x00AC40C0): no bone (-1), no blends.
    C3DGroup2();
    // Ego_r ?GetVertexBlends@C3DGroup2@@QAEPAVC3DVertexBlend2@@XZ (retail 0x00AC4080) and
    // ?PeekVertexBlends@C3DGroup2@@QBEPBVC3DVertexBlend2@@XZ (retail 0x00AC40A0).
    C3DVertexBlend2* GetVertexBlends();
    const C3DVertexBlend2* PeekVertexBlends() const;
    // Ego_r ?GetBoneIndex@C3DGroup2@@QBEJXZ (inline in retail) and ?SetBoneIndex@C3DGroup2@@QAEXJ@Z (retail 0x00AC4070).
    long GetBoneIndex() const { return BoneIndex; }
    void SetBoneIndex(long index);
    // Ego_r ?GetVertexBlendCount@C3DGroup2@@QBEJXZ and ?PeekVertexBlend@C3DGroup2@@QBEABVC3DVertexBlend2@@K@Z (inline in
    // retail, CParticleProgram::AddGroup 0x00AE37D0) and ?AddVertexBlend@C3DGroup2@@QAEXABVC3DVertexBlend2@@@Z (0x00AC4110).
    long GetVertexBlendCount() const { return (long)VertexBlends.size(); }
    const C3DVertexBlend2& PeekVertexBlend(unsigned long index) const { return VertexBlends[index]; }
    void AddVertexBlend(const C3DVertexBlend2& blend);
    // Ego_r ?Resize@C3DGroup2@@QAEXK@Z (retail 0x00AC41D0, not yet reconstructed): size the blend list.
    void Resize(unsigned long count);

private:
    friend struct C3DGroup2LayoutCheck;
    long BoneIndex;                           // +0x00
    CArray<C3DVertexBlend2> VertexBlends;     // +0x04
};

struct C3DGroup2LayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(C3DGroup2, VertexBlends) == 0x04);
};
FABLE_STATIC_ASSERT(sizeof(C3DGroup2) == 0x10);
