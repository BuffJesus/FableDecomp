// Open one definition from a cached compressed block (retail 0x009AD5F0).
#include "fable_definition_manager.h"
#include "fable_data_input_stream.h"
#include "engine/CCompressorZlib.h"
extern double FABLE_FASTCALL GFGetTime();
extern const CDefinitionManager* g_DefCacheManager_0138E190;
extern unsigned long g_DefCacheOffset_0138E194;
extern unsigned long g_DefCacheSize_0138E198;
extern unsigned char g_DefCacheData_0138E19C[0x8000];
void CDefinitionManager::DecompressDefinitionData(CMemoryDataInputStream& stream, unsigned long globalIndex) const
{
    const _STL::pair<unsigned long,unsigned long>* next = CompressedOffsets.UpperBound(globalIndex);
    unsigned long firstIndex = (next-1)->first;
    unsigned long offset = (next-1)->second;
    unsigned long compressedSize = next->second - offset;
    unsigned long count = next->first - firstIndex;
    if (g_DefCacheManager_0138E190 != this || g_DefCacheOffset_0138E194 != offset)
    {
        GFGetTime();
        CCompressorZlib compressor;
        g_DefCacheSize_0138E198 = compressor.InternalDecompress(CompressedDefData.begin() + offset,
            compressedSize, g_DefCacheData_0138E19C, sizeof(g_DefCacheData_0138E19C));
        g_DefCacheManager_0138E190 = this;
        g_DefCacheOffset_0138E194 = offset;
        GFGetTime();
    }
    unsigned long index = globalIndex - firstIndex;
    unsigned short* offsets = reinterpret_cast<unsigned short*>(g_DefCacheData_0138E19C);
    unsigned long start = offsets[index];
    unsigned long end = index + 1 < count ? offsets[index+1] : g_DefCacheSize_0138E198;
    stream.OpenMemorySource(g_DefCacheData_0138E19C + start, end - start);
}