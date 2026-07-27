#include <streams.h>
#include <stdio.h>

int main()
{
    const unsigned int baseRendererSize =
        static_cast<unsigned int>(sizeof(CBaseRenderer));
    const unsigned int baseVideoRendererSize =
        static_cast<unsigned int>(sizeof(CBaseVideoRenderer));
    printf(
        "DIRECTSHOW_BASECLASSES_LAYOUT "
        "CBaseRenderer=0x%X CBaseVideoRenderer=0x%X\n",
        baseRendererSize,
        baseVideoRendererSize);
    return baseVideoRendererSize == 0x160 ? 0 : 1;
}
