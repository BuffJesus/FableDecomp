#pragma once

#include "fable_init_base_class.h"

class CGameComponent;

// FableWin.pdb: CGame : CInitBaseClass. The retail singleton is 0x013B83D0.
class CGame : public CInitBaseClass
{
public:
    // 0x00413120: base Initialise, clear Quit and the current component, then
    // install a fresh progress display.
    bool Initialise();
    void Play();
    // 0x00412F60: forwards to CInitBaseClass::Uninitialise (tail jump).
    void Uninitialise();

private:
    CGameComponent* CurrentGameComponent;
    fable_u8 ParameterBuffer[0x200];
    bool Quit;
};

extern CGame g_Game_013B83D0;

FABLE_STATIC_ASSERT(sizeof(CGame) == 0x210);
