#pragma once
#include "fable_display_geometry.h"
#include "fable_string.h"
#include "fable_surface_types.h"
#include "fable_rgb_colour.h"
class CFontBank;
// PDB CQuickDrawTriInfo and retail vector destruction at 009C2F50.
class CQuickDrawTriInfo
{
public:
    CQuickDrawTriInfo() {}
    CQuickDrawTriInfo(const CQuickDrawTriInfo& other); // retail 009DE890
    unsigned char Line : 1;
    unsigned char IsText : 1;
    C2DBoxF Window;
    CWideString String;
    CTexture UploadedTexture;
    union {
        struct { unsigned long BlendMode; float ZAverage; };
        struct {
            float StringX, StringY, StringZ, StringScale;
            unsigned long StringFlags;
        };
    };
    CRGBColour Colour;
    const CFontBank* PFont;
};
typedef char FableQuickDrawInfoSize[(sizeof(CQuickDrawTriInfo)==60)?1:-1];
