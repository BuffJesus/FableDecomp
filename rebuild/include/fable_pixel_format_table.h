#pragma once

#include "rebuild_abi.h"

struct FablePixelFormatRecord
{
    fable_i32 d3dFormat00;
    fable_i32 formatClass04;
    fable_i32 colourDepth08;
    fable_i32 alphaBits0C;
    fable_i32 redBits10;
    fable_i32 greenBits14;
    fable_i32 blueBits18;
    fable_i32 auxiliaryBits1C;
    fable_i32 linear20;
};

enum
{
    FABLE_PIXEL_FORMAT_COUNT = 46,
    FABLE_PIXEL_FORMAT_TABLE_COUNT = 47
};

extern FablePixelFormatRecord
    g_FablePixelFormatTable[FABLE_PIXEL_FORMAT_TABLE_COUNT];

FABLE_STATIC_ASSERT(sizeof(FablePixelFormatRecord) == 0x24);
