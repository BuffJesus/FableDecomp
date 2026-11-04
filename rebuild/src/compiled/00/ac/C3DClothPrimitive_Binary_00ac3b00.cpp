// C3DClothPrimitive::SaveBinary (retail 0x00AC3B00, Ego_r 0x00C4B620) and LoadBinary (retail 0x00AC3BD0, Ego_r
// 0x00C4B6F0): the primitive index, material index, then the particle program serialised to memory and written as one
// compressed block (its uncompressed length first). LoadBinary reads the block into a scoped allocation from the given
// pool, parses a fresh particle program from it into the cloth memory pool, and drops the program if that fails.
#include "fable_3d_cloth_primitive.h"
#include "fable_counted_pointer_inline.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_handle_memory_pool.h"

void C3DClothPrimitive::SaveBinary(CDataOutputStream& stream) const
{
    stream.WriteULONG(PrimitiveIndex);
    stream.WriteULONG(MaterialIndex);
    CMemoryDataOutputStream program;
    ParticleProgram->WriteToFile(program);
    stream.WriteULONG(program.GetLength());
    stream.WriteCompressed(program.PeekData(), program.GetLength());
}

bool C3DClothPrimitive::LoadBinary(CMemoryDataInputStream& stream, CMovableResourceMemoryPool* pool)
{
    PrimitiveIndex = stream.ReadULONG();
    MaterialIndex = stream.ReadULONG();
    unsigned long size = stream.ReadULONG();
    CScopedMemoryHandle memory;
    if (!pool->Allocate(memory, size, 0))
        return false;
    void* data = memory.GetPointer();
    stream.ReadCompressed(data, size);
    CMemoryDataInputStream programStream(data, size);
    ParticleProgram.Reset(new CParticleProgram);
    if (!ParticleProgram->ReadFromFile(programStream, *N3DMeshAllocators::ClothMemoryPool))
    {
        ParticleProgram.ReleaseData();
        return false;
    }
    return true;
}
