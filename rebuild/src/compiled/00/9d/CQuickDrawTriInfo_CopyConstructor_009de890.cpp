#include "fable_quick_draw_info.h"
CQuickDrawTriInfo::CQuickDrawTriInfo(const CQuickDrawTriInfo& source)
    : Line(source.Line), IsText(source.IsText), Window(source.Window),
      String(source.String), UploadedTexture(source.UploadedTexture),
      BlendMode(source.BlendMode), ZAverage(source.ZAverage),
      StringX(source.StringX), StringY(source.StringY), StringZ(source.StringZ),
      StringScale(source.StringScale), StringFlags(source.StringFlags),
      Colour(source.Colour.R, source.Colour.G, source.Colour.B, source.Colour.A), PFont(source.PFont)
{
}
