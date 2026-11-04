// Behaviour fixture for CACompressor's default virtuals: each is the real
// compiled provider, called non-virtually on compressor storage, and must
// leave the compressor, the buffers and the compression info untouched.
#include "engine/CACompressor.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static union { double align; unsigned char bytes[sizeof(CACompressor) + 16]; } object, before;
    static unsigned char in[32], out[32], info[32], inBefore[32], outBefore[32], infoBefore[32];
    int cases = 0;
    for (int seed = 0; seed < 6; ++seed)
    {
        for (unsigned i = 0; i < sizeof(object.bytes); ++i) object.bytes[i] = (unsigned char)(i * 5 + seed);
        for (unsigned i = 0; i < 32; ++i) { in[i] = (unsigned char)(i + seed); out[i] = (unsigned char)~i; info[i] = (unsigned char)(i * 3); }
        before = object;
        memcpy(inBefore, in, 32); memcpy(outBefore, out, 32); memcpy(infoBefore, info, 32);
        CACompressor* c = reinterpret_cast<CACompressor*>(&object);
        CCompressionInfo& ci = *reinterpret_cast<CCompressionInfo*>(info);
        c->CACompressor::CompressAsImage(in, 32, out, 32, seed, -seed, 7, ci);
        c->CACompressor::Compress(in, 32, out, 32, ci);
        c->CACompressor::Decompress(out, 32, in, 32, ci);
        c->CACompressor::SetQuality(seed * 0.25f);
        if (memcmp(&object, &before, sizeof(object)) || memcmp(in, inBefore, 32) || memcmp(out, outBefore, 32) || memcmp(info, infoBefore, 32))
        {
            printf("COMPRESSOR_DEFAULTS_FAIL seed=%d\n", seed);
            return 1;
        }
        ++cases;
    }
    printf("COMPRESSOR_DEFAULTS_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
