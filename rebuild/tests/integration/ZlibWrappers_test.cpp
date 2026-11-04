// Behaviour fixture for CCompressorZlib::Compress 0x00A3AC40 and Decompress
// 0x00A3ABE0: the 13-byte block header ("CMP1", uncompressed size, compressed
// size including the header, type 6) around InternalCompress/InternalDecompress,
// which are fixture-side recorders, and the CCompressionInfo they report.
#include "engine/CCompressorZlib.h"
#include <stdio.h>
#include <string.h>

static const void* g_in; static unsigned long g_inSize, g_outCapacity, g_result; static void* g_out; static int g_calls;
unsigned long CCompressorZlib::InternalCompress(const void* data, unsigned long size, void* output, unsigned long capacity)
{
    ++g_calls; g_in = data; g_inSize = size; g_out = output; g_outCapacity = capacity;
    memset(output, 0x77, g_result);
    return g_result;
}
unsigned long CCompressorZlib::InternalDecompress(const void* data, unsigned long size, void* output, unsigned long capacity)
{
    ++g_calls; g_in = data; g_inSize = size; g_out = output; g_outCapacity = capacity;
    return g_result;
}

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("ZLIB_WRAPPERS_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)
static long Long(const unsigned char* p) { long v; memcpy(&v, p, 4); return v; }

int main()
{
    int cases = 0;
    static union { double align; unsigned char bytes[sizeof(CCompressorZlib)]; } storage;
    CCompressorZlib* zlib = reinterpret_cast<CCompressorZlib*>(&storage);
    unsigned char input[64], output[128];
    for (unsigned i = 0; i < sizeof(input); ++i) input[i] = (unsigned char)(i * 3);
    static const unsigned long sizes[] = { 0, 1, 40 };
    static const unsigned long results[] = { 0, 9, 50 };
    for (int s = 0; s < 3; ++s) for (int r = 0; r < 3; ++r)
    {
        memset(output, 0xee, sizeof(output));
        CCompressionInfo info;
        memset(&info, 0xcc, sizeof(info));
        g_calls = 0; g_result = results[r];
        zlib->CCompressorZlib::Compress(input, sizes[s], output, sizeof(output), info);
        CHECK(!memcmp(output, "1PMC", 4) && output[12] == 6, 1);
        if (sizes[s] == 0)
        {
            CHECK(g_calls == 0 && Long(output + 4) == 0 && Long(output + 8) == 13 && output[13] == 0xee, 2);
            CHECK(info.UncompressedSize == 0 && info.CompressedSize == 13 && info.CompressionType == 6, 3);
        }
        else
        {
            CHECK(g_calls == 1 && g_in == input && g_inSize == sizes[s] && g_out == output + 13 && g_outCapacity == sizeof(output) - 13, 4);
            CHECK(Long(output + 4) == (long)sizes[s] && Long(output + 8) == (long)results[r] + 13, 5);
            CHECK(info.UncompressedSize == (long)sizes[s] && info.CompressedSize == (long)results[r] + 13 && info.CompressionType == 6, 6);
        }
        // Read it back.
        unsigned char inflated[64];
        CCompressionInfo back;
        memset(&back, 0xcc, sizeof(back));
        g_calls = 0;
        const unsigned long blockSize = sizes[s] ? results[r] + 13 : 13;
        zlib->CCompressorZlib::Decompress(output, blockSize, inflated, sizeof(inflated), back);
        CHECK(g_calls == (blockSize != 13), 7);
        if (blockSize != 13) CHECK(g_in == output + 13 && g_inSize == blockSize - 13 && g_out == inflated && g_outCapacity == sizeof(inflated), 8);
        CHECK(back.UncompressedSize == info.UncompressedSize && back.CompressedSize == info.CompressedSize && back.CompressionType == 6, 9);
        ++cases;
    }
    if (g_fail) return 1;
    printf("ZLIB_WRAPPERS_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
