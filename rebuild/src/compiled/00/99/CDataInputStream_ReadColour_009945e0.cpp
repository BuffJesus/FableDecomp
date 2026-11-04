// lib_data_stream.cpp, retail 0x009945E0 / 0x00994670 (Ego_r ReadColour,
// ReadPixelFormat). ReadColour:
// the four stored bytes (B, G, R, A, the colour's memory order) through the
// inline Read, rebuilt as a colour; zeros if the read is refused.
#include <string.h>
#include "fable_data_input_stream.h"
#include "fable_rgb_colour.h"
#include "fable_pixel_format_init.h"
#include "fable_surface_types.h"

CRGBColour CDataInputStream::ReadColour()
{
    unsigned char data[4] = {0, 0, 0, 0};
    Read(data, 4);
    return CRGBColour(data[2], data[1], data[0], data[3]);
}

// Retail 00994670: the six-byte CPixelFormatInit (zeroed, then read), matched
// to a format by Initialise.
CPixelFormat CDataInputStream::ReadPixelFormat()
{
    CPixelFormatInit init;
    memset(&init, 0, sizeof(init));
    Read(&init, 6);
    CPixelFormat format(init);
    return format;
}
