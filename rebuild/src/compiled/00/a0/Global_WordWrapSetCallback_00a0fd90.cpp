#include "rebuild_abi.h"

typedef fable_u32 (FABLE_FASTCALL *FableWordWidthCallback)(fable_u16 value);
typedef fable_u32 (FABLE_FASTCALL *FableLineHeightCallback)();

extern FableWordWidthCallback g_FableWordWidthCallback_013CE758;
extern FableLineHeightCallback g_FableLineHeightCallback_013CE75C;

void FABLE_FASTCALL WordWrap_SetCallback(
    FableWordWidthCallback wordWidth,
    FableLineHeightCallback lineHeight)
{
    if (wordWidth != 0)
    {
        g_FableWordWidthCallback_013CE758 = wordWidth;
    }
    if (lineHeight != 0)
    {
        g_FableLineHeightCallback_013CE75C = lineHeight;
    }
}
