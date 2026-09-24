#include "fable_ui_transform.h"
#include <stdio.h>
#include <string.h>

unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateSourceExtent, FableUiCoordinateDestinationExtent;
void* FableUiScaleContext;
static float Float(unsigned value) { float result; memcpy(&result, &value, 4); return result; }
static unsigned Bits(float value) { unsigned result; memcpy(&result, &value, 4); return result; }
int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    unsigned value[6];
    while (fscanf(input, "%u %u %u %u %u %u", &value[0], &value[1], &value[2], &value[3], &value[4], &value[5]) == 6)
    {
        FableUiScaleContext = value[0] ? reinterpret_cast<void*>(0x1234) : 0;
        FableUiCoordinateConversionEnabled = static_cast<unsigned char>(value[1]);
        FableUiCoordinateSourceExtent.x = Float(value[2]); FableUiCoordinateSourceExtent.y = Float(value[3]);
        FableUiCoordinateDestinationExtent.x = Float(value[4]); FableUiCoordinateDestinationExtent.y = Float(value[5]);
        FableUiStateVector2 result;
        if (FableUiGetManagerScale(0, 0, &result) != &result) return 4;
        printf("%u %u %u %u\n", Bits(result.x), Bits(result.y), Bits(FableUiGetCoordinateWidth()), Bits(FableUiGetCoordinateHeight()));
    }
    fclose(input); return 0;
}
