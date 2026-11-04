#include "fable_player_manager.h"
bool CPlayer::IsMainPlayer() const
{
    long playerNumber = Number;
    bool isMain = playerNumber == PlayerManager->GetMainPlayerNumber();
    return isMain;
}
