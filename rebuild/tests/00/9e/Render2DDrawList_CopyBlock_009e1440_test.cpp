#include <stdio.h>
#include <string.h>

#include "../../../src/compiled/00/9e/Render2DDrawList_CopyBlock_009e1440.cpp"

static void Fill(
    Render2DDrawListBlockView& block,
    fable_u8 value)
{
    memset(&block, value, sizeof(block));
}

int main()
{
    Render2DDrawListBlockView source[6];
    Render2DDrawListBlockView destination[4];
    Render2DDrawListBlockView expected[3];
    Render2DDrawListBlockView controller;
    for (fable_i32 index = 0; index < 6; ++index)
    {
        Fill(source[index], static_cast<fable_u8>(0x20 + index));
    }
    for (fable_i32 index = 0; index < 4; ++index)
    {
        Fill(destination[index], 0xCC);
    }
    memset(&controller, 0, sizeof(controller));
    memcpy(expected, source + 2, sizeof(expected));

    controller.end04 = source + 5;
    Render2DDrawListBlockView* result =
        controller.CopyBlock(destination, source + 2);
    if (result != destination ||
        memcmp(destination, expected, sizeof(expected)) != 0 ||
        controller.end04 != destination + 3)
    {
        printf("FABLETLC_RENDER2D_COPY_BLOCK_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    Render2DDrawListBlockView emptyDestination[1];
    Fill(emptyDestination[0], 0xDD);
    controller.end04 = source + 4;
    result = controller.CopyBlock(emptyDestination, source + 4);
    if (result != emptyDestination ||
        controller.end04 != emptyDestination)
    {
        printf("FABLETLC_RENDER2D_COPY_BLOCK_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    controller.end04 = source + 2;
    result = controller.CopyBlock(emptyDestination, source + 3);
    if (result != emptyDestination ||
        controller.end04 != emptyDestination)
    {
        printf("FABLETLC_RENDER2D_COPY_BLOCK_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_RENDER2D_COPY_BLOCK_BEHAVIOR PASS\n");
    return 0;
}
