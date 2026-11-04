// C3DClothPrimitive::C3DClothPrimitive(const C3DMeshFileClothChunk&) (retail 0x00AC3E00, Ego_r 0x00C4B900) and the
// out-of-line CCountedPointer<CParticleProgram>::Reset it calls (retail 0x00AC3FF0) with its DeleteData (0x00AC3FD0). The chunk's compiled particle code
// is read into a fresh CParticleProgram (allocated from the cloth movable-resource pool), the chunk's material index is
// kept, and every group sub-chunk becomes one of the program's groups.
#include "fable_counted_pointer_inline.h"   // retail expands the group pointer copies here
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_3d_group2.h"
#include "fable_3d_cloth_primitive.h"

template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, Data);
}

template void CCountedPointer<CParticleProgram>::Reset(CParticleProgram* data);
template void CCountedPointer<CParticleProgram>::DeleteData(void*);

C3DClothPrimitive::C3DClothPrimitive(const C3DMeshFileClothChunk& chunk)
{
    ParticleProgram.Reset(new CParticleProgram);
    CMemoryDataInputStream stream(chunk.CompiledCode.empty() ? (const void*)&chunk.CompiledCode : &chunk.CompiledCode[0],
                                  chunk.CompiledCode.size());
    ParticleProgram->ReadFromFile(stream, *N3DMeshAllocators::ClothMemoryPool);
    MaterialIndex = chunk.MaterialIndex;
    ParticleProgram->ReserveGroups(chunk.CountSubChunksOfType(C3DMeshFileGroupChunk::GetStaticChunkType()));
    CCountedPointer<const C3DMeshFileGroupChunk> group = chunk.FindFirstGroup();
    while (!group.IsNull())
    {
        C3DGroup2 group2(*group);
        ParticleProgram->AddGroup(group2);
        group = chunk.FindNextGroup(group);
    }
}
