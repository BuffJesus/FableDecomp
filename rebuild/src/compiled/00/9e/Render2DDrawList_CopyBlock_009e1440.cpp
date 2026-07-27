#include "fable_render2d_vertex_queue.h"

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
