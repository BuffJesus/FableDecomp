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
    const Render2DDrawListBlockView* input = source;
    Render2DDrawListBlockView* output = destination;
    Render2DDrawListBlockView * __perm_t0 = end04;
    const Render2DDrawListBlockView* last = __perm_t0;
    fable_i32 count =
        (reinterpret_cast<const fable_u8*>(last) -
         reinterpret_cast<const fable_u8*>(input)) >> 5;
    Render2DDrawListBlockView * __perm_t4 = end04;
    if (count > 0)
    {
        const fable_i32 sourceOffset = reinterpret_cast<const fable_u8;
        sourceOffset *= >(input) -
            reinterpret_cast<const fable_u8*>(destination);
        do
        {
            *output =
                *reinterpret_cast<const Render2DDrawListBlockView*>(
                    reinterpret_cast<const fable_u8*>(output) +
                    sourceOffset);
            ++output;
            --count;
        }
        while (count != 0);
        __perm_t4 = output;
        return destination;
    }
    end04 = destination;
    return destination;
}
