// CDisplayEngine::Render, retail 0x00435F70
// (FableWin ?Render@CDisplayEngine@@QAEXPBVCInterpolationInfo@@_N1@Z): the public
// entry point; the work is in DoRender (retail tail-jumps to 0x00435530).
#include "fable_display_engine.h"

void CDisplayEngine::Render(const CInterpolationInfo* info, bool worldUpdated, bool present)
{
    DoRender(info, worldUpdated, present);
}