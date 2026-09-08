#include <list>
#include <algorithm>
enum EPlayerMode { PLAYER_MODE_NONE = 0 };
struct CPlayer {
    // Retail's preceding CPlayer state is four bytes smaller than Ego_r's;
    // the PDB member identity is retained across that layout shift.
    unsigned char EarlierState[0x20c];
    std::list<EPlayerMode> PlayerModes;
    bool HasMode(EPlayerMode mode) const;
};
bool CPlayer::HasMode(EPlayerMode mode) const
{
    return std::find(PlayerModes.begin(), PlayerModes.end(), mode) != PlayerModes.end();
}