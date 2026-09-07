#include "engine/CTCOracleMinigame.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTCOracleMinigame_Methods : CTCOracleMinigame {
    bool OutputsFiring();
};

bool CTCOracleMinigame_Methods::OutputsFiring()
{
    for (int i = 0; i < 4; i++)
    {
        if (this->SymbolsFiring[i] != 0)
            return true;
    }
    return false;
}