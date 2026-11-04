#include "engine/CCompressorZlib.h"
#include <stdio.h>

struct FakeCompressor
{
    virtual ~FakeCompressor() {}
    virtual void Slot1() {}
    virtual void Slot2() {}
    virtual void Slot3() {}
    virtual void Slot4() {}
    virtual void Compress(const void* input, unsigned long inputSize,
                          void* output, unsigned long outputSize,
                          CCompressionInfo& info)
    {
        ++calls;
        gotSelf = this;
        gotInput = input;
        gotInputSize = inputSize;
        gotOutput = output;
        gotOutputSize = outputSize;
        gotInfo = &info;
    }
    int calls;
    FakeCompressor* gotSelf;
    const void* gotInput;
    unsigned long gotInputSize;
    void* gotOutput;
    unsigned long gotOutputSize;
    CCompressionInfo* gotInfo;
};

int main()
{
    char input[16];
    char output[16];
    CCompressionInfo info;
    int cases = 0;
    for (unsigned long a = 0; a != 8; ++a) {
        for (unsigned long b = 0; b != 4; ++b) {
            FakeCompressor fake;
            fake.calls = 0;
            CCompressorZlib* compressor = reinterpret_cast<CCompressorZlib*>(&fake);
            compressor->CCompressorZlib::CompressAsImage(
                input + a, a * 37, output + b, b * 101,
                static_cast<long>(a), -static_cast<long>(b),
                static_cast<long>(a + b), info);
            if (fake.calls != 1 || fake.gotSelf != &fake ||
                fake.gotInput != input + a || fake.gotInputSize != a * 37 ||
                fake.gotOutput != output + b || fake.gotOutputSize != b * 101 ||
                fake.gotInfo != &info) {
                printf("FAIL a=%lu b=%lu calls=%d\n", a, b, fake.calls);
                return 1;
            }
            ++cases;
        }
    }
    printf("ZLIB_COMPRESS_AS_IMAGE_PASS cases=%d\n", cases);
    return 0;
}
