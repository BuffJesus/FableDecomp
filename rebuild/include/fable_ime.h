#pragma once

#include "rebuild_abi.h"

struct HWND__;
class CPersistContext;

namespace NUISystem
{
    class CIME
    {
    public:
        static void FABLE_FASTCALL LoadIMESettings(
            CPersistContext& context);
        void Initialise(HWND__* window);

    private:
        bool enableIme_;
        fable_u8 padding01_[3];
        HWND__* window_;
        bool initialised_;
        fable_u8 padding09_[3];
        fable_u8 appearance_[0x40];
        void* editBox_;
        bool cancelOnOkClick_;
        fable_u8 padding51_[3];
    };
}

FABLE_STATIC_ASSERT(sizeof(NUISystem::CIME) == 0x54);
