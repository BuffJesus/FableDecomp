#include "engine/CGameTimeManager.h"

bool __fastcall CGameTimeManager_IsDay(const CGameTimeManager* self)
{
    const bool isNight =
        self->Time > (1.0f / 6.0f) && self->Time <= (7.0f / 8.0f);
    return !isNight;
}