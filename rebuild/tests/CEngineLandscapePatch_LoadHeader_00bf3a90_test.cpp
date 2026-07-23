#include <stdio.h>
#include <string.h>
#include "candidates/landscape_patch_compiled.h"

struct TestInputLayout
{
    fable_u8 pad00[0x04];
    fable_u32 bytesRead;
    fable_u8 pad08[0x04];
    fable_u32* cursor32;
    fable_u8 pad10[0x04];
    fable_i32 bytesRemaining;
};

FABLE_STATIC_ASSERT(offsetof(TestInputLayout, bytesRead) == 0x04);
FABLE_STATIC_ASSERT(offsetof(TestInputLayout, cursor32) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(TestInputLayout, bytesRemaining) == 0x14);
FABLE_STATIC_ASSERT(sizeof(TestInputLayout) == 0x18);

static fable_u8 g_overflowPayload[0x24];
static int g_overflowCalls;

void CDataInputStream::ReadWithSrcChunkOverflow(
    CDataInputStream*,
    fable_u8* destination,
    int byteCount)
{
    ++g_overflowCalls;
    memcpy(destination, g_overflowPayload, byteCount);
}
static int CheckHeaderWords(const CEngineLandscapePatch& patch, const fable_u32* expected)
{
    const fable_u32* actual = reinterpret_cast<const fable_u32*>(
        reinterpret_cast<const fable_u8*>(&patch) + 0x18);
    for (int index = 0; index != 8; ++index)
    {
        if (actual[index] != expected[index])
            return index + 1;
    }
    return 0;
}

static int TestBufferedRead()
{
    fable_u32 words[9] = {
        0x11111111, 0x22222222, 0x33333333,
        0x44444444, 0x55555555, 0x66666666,
        0x77777777, 0x88888888, 0x00000001
    };
    TestInputLayout input;
    CEngineLandscapePatch patch;
    memset(&input, 0, sizeof(input));
    memset(&patch, 0, sizeof(patch));
    input.bytesRead = 7;
    input.cursor32 = words;
    input.bytesRemaining = 100;
    patch.m_Flags = 0xA5;

    patch.LoadHeader(*reinterpret_cast<CMemoryDataInputStream*>(&input));

    if (input.bytesRead != 43) return 101;
    if (input.bytesRemaining != 64) return 102;
    if (input.cursor32 != words + 9) return 103;
    if (patch.m_Flags != 0xB5) return 104;
    const int headerResult = CheckHeaderWords(patch, words);
    return headerResult == 0 ? 0 : 110 + headerResult;
}

static int TestOverflowRead()
{
    fable_u32 words[9] = {
        0x91, 0x92, 0x93, 0x94, 0x95,
        0x96, 0x97, 0x98, 0x00000000
    };
    memcpy(g_overflowPayload, words, sizeof(words));
    g_overflowCalls = 0;

    TestInputLayout input;
    CEngineLandscapePatch patch;
    memset(&input, 0, sizeof(input));
    memset(&patch, 0, sizeof(patch));
    input.bytesRemaining = 8;
    patch.m_Flags = 0xB7;

    patch.LoadHeader(*reinterpret_cast<CMemoryDataInputStream*>(&input));

    if (g_overflowCalls != 1) return 201;
    if (patch.m_Flags != 0xA7) return 202;
    const int headerResult = CheckHeaderWords(patch, words);
    return headerResult == 0 ? 0 : 210 + headerResult;
}

int main()
{
    const int buffered = TestBufferedRead();
    if (buffered != 0)
    {
        printf("LOADHEADER_TEST FAIL buffered=%d\n", buffered);
        return buffered;
    }
    const int overflow = TestOverflowRead();
    if (overflow != 0)
    {
        printf("LOADHEADER_TEST FAIL overflow=%d\n", overflow);
        return overflow;
    }
    printf("LOADHEADER_TEST PASS\n");
    return 0;
}
