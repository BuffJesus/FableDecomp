#include "engine/CCompressorZlib.h"

void CCompressorZlib::CompressAsImage(const void* input, unsigned long inputSize,
                                     void* output, unsigned long outputSize,
                                     long, long, long, CCompressionInfo& info)
{
    Compress(input, inputSize, output, outputSize, info);
}
