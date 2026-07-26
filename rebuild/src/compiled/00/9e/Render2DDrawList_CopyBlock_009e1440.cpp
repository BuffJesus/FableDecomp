#include "rebuild_abi.h"

struct Render2DDrawListBlockView
{
    Render2DDrawListBlockView* begin00;
    Render2DDrawListBlockView* end04;
    fable_u8 unknown08[0x18];

    Render2DDrawListBlockView* CopyBlock(
        Render2DDrawListBlockView* destination,
        const Render2DDrawListBlockView* source);
};

Render2DDrawListBlockView* Render2DDrawListBlockView::CopyBlock(
    Render2DDrawListBlockView* destination,
    const Render2DDrawListBlockView* source)
{
    Render2DDrawListBlockView* output = destination;
    const Render2DDrawListBlockView* input = source;
    for (fable_i32 count =
             (reinterpret_cast<const fable_u8*>(end04) -
              reinterpret_cast<const fable_u8*>(input)) >> 5;
         count > 0;
         --count)
    {
        *output++ = *input++;
    }
    end04 = output;
    return destination;
}
