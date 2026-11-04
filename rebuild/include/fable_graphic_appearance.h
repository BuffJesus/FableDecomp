#pragma once

#include "rebuild_abi.h"

class CGraphicDataBank;

namespace NGraphicAppearance
{
    // Only the static bank setter is modelled (the instance layout is
    // engine/_quarantine/CTCGraphicAppearance.h).
    class CTCGraphicAppearance
    {
    public:
        // ?SetGraphicBank@CTCGraphicAppearance@NGraphicAppearance@@SIXAAVCGraphicDataBank@@@Z,
        // retail 0x004BBFC0 (`mov [0x013B8A08], ecx; ret`). Caller:
        // CMainGameComponent::InitGraphics 0x00416C8A.
        static void FABLE_FASTCALL SetGraphicBank(CGraphicDataBank& bank);
    };
}
