#include <cstddef>

class CPlayer;

struct CPlayerOverlay
{
    unsigned char pad_00[0x28];
    int playerIndex;
};
static_assert(offsetof(CPlayerOverlay, playerIndex) == 0x28);

struct PlayerActivityOverlay
{
    unsigned char pad_00[0x91];
    unsigned char flags91;
};
static_assert(offsetof(PlayerActivityOverlay, flags91) == 0x91);

using GetPlayerActivityFn = PlayerActivityOverlay *(__thiscall *)(CPlayer *);
static constexpr GetPlayerActivityFn kGetPlayerActivity =
    reinterpret_cast<GetPlayerActivityFn>(0x00487DC0);

bool CPlayerManager::IsMultiplayerGameActive() const
{
    int slot = 0;

    do
    {
        const int playerCount = static_cast<int>(this->Players.size());
        int playerIndex = 0;

        if (0 < playerCount)
        {
            do
            {
                if (reinterpret_cast<const CPlayerOverlay *>(this->Players[playerIndex])->playerIndex == slot)
                {
                    CPlayer *player = this->GetPlayer(slot);
                    if (reinterpret_cast<const CPlayerOverlay *>(player)->playerIndex != this->PlayerNeutral)
                    {
                        player = this->GetPlayer(slot);
                        PlayerActivityOverlay *const activity = kGetPlayerActivity(player);
                        if (activity != nullptr && (activity->flags91 & 1) == 0)
                        {
                            return true;
                        }
                    }
                    break;
                }

                ++playerIndex;
            } while (playerIndex < playerCount);
        }

        ++slot;
        if (3 < slot)
        {
            return false;
        }
    } while (true);
}