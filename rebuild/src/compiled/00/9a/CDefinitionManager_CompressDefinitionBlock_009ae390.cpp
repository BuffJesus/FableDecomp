#include <new>
#include <string.h>
#include "fable_definition_manager.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"
#include "engine/CCompressorZlib.h"

void CDefinitionManager::CompressDefinitionBlock(CMemoryDataOutputStream& data,
    CArrayVector<unsigned short>& offsets, unsigned long firstGlobalIndex)
{
    CCompressorZlib compressor;
    CMemoryDataOutputStream packed;
    unsigned long tableBytes = offsets.size() * sizeof(unsigned short);
    for (unsigned long i = 0; i < offsets.size(); ++i)
        offsets[i] = static_cast<unsigned short>(offsets[i] + tableBytes);
    packed.Write(offsets.GetAsCArray(), tableBytes);
    packed.Write(data.PeekData(), data.GetLength());

    CArrayVector<unsigned char> scratch(packed.GetLength() + 1024);
    unsigned long length = compressor.InternalCompress(packed.PeekData(), packed.GetLength(),
        scratch.GetAsCArray(), scratch.size());
    unsigned long position = CompressedDefData.size();
    CompressedDefData.resize(position + length, 0);
    memcpy(CompressedDefData.GetAsCArray() + position, scratch.GetAsCArray(), length);
    CompressedOffsets.Insert(firstGlobalIndex, position);

    data.Clear();
    offsets.clear();
}
