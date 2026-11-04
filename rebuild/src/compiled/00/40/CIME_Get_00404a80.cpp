#include "fable_ime.h"

extern NUISystem::CIME g_FableIME_013B7CD8;

NUISystem::CIME& FABLE_FASTCALL NUISystem::CIME::Get()
{
    return g_FableIME_013B7CD8;
}
