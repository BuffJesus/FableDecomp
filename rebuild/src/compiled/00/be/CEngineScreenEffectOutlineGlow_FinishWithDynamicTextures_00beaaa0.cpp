#include <new>
#include "candidates/outline_glow_compiled.h"

CEngineScreenEffectOutlineGlow::~CEngineScreenEffectOutlineGlow()
{
    vfptr = reinterpret_cast<void**>(0x012A8134);

    if (Blur2Texture != 0)
    {
        operator delete(Blur2Texture);
        Blur2Texture = 0;
    }

    if (field_10 != 0)
    {
        LandscapeLayerList* layerList = *reinterpret_cast<LandscapeLayerList**>(DAT_01436ea8 + 0x6B0);
        layerList->InsertNode(field_10);
        field_10 = 0;
    }

    vfptr = reinterpret_cast<void**>(0x0129B860);
}
