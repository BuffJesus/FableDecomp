#include "fable_player_manager.h"
#include "fable_stlport_algo.h"
bool CPlayer::HasMode(EPlayerMode mode) const
{
    return _STL::find(PlayerModes.begin(), PlayerModes.end(), mode) != PlayerModes.end();
}
