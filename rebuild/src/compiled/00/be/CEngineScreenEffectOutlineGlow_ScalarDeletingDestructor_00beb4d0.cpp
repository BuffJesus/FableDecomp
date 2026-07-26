#include <new>
#include "candidates/outline_glow_compiled.h"

CEngineScreenEffectOutlineGlow*
CEngineScreenEffectOutlineGlow::ScalarDeletingDestructor(unsigned int freeFlags)
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

    if ((freeFlags & 1U) != 0)
    {
        CEngineLandscapeMemoryManager* memoryManager =
            *reinterpret_cast<CEngineLandscapeMemoryManager**>(DAT_01436ea8 + 0x6B0);
        memoryManager->FreeVirtual(this);
    }

    return this;
}
