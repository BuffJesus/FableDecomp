// C3DAnimFileSequenceChunk setters (retail 0x00AAF570..0x00AAF5A0, Ego_r 0x00C43B10..0x00C43B40, same order).
// C3DAnimationBuilder::GetIntersection (retail 0x00AB2160) sets the bone and parent indices back to back.
#include "fable_chunked_file.h"

void C3DAnimFileSequenceChunk::SetBoneName(const CCharString& name)
{
    BoneName = name;
}

void C3DAnimFileSequenceChunk::SetBoneIndex(long index)
{
    BoneIndex = index;
}

void C3DAnimFileSequenceChunk::SetParentIndex(long index)
{
    ParentIndex = index;
}

void C3DAnimFileSequenceChunk::SetSamplesPerSecond(float samplesPerSecond)
{
    SamplesPerSecond = samplesPerSecond;
}
