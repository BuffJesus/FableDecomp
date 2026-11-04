// C3DGroup2::C3DGroup2(const C3DMeshFileGroupChunk&) (retail 0x00AC4150, Ego_r 0x00C4BBE0): copy a mesh file's "VGRP"
// vertex-group chunk into the runtime group - its bone index and every vertex blend, in order.
#include <new>
#include "fable_chunked_file.h"
#include "fable_stlport_vector_impl.h"
#include "fable_3d_group2.h"

C3DGroup2::C3DGroup2(const C3DMeshFileGroupChunk& chunk)
{
    BoneIndex = chunk.GetBoneIndex();
    unsigned long count = chunk.GetCount();
    const C3DVertexBlend2* blends = chunk.GetList();
    for (unsigned long i = 0; i < count; ++i)
        VertexBlends.push_back(blends[i]);
}
