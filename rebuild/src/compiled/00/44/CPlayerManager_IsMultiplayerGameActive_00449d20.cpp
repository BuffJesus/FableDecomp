struct PlayerActivity
{
    unsigned char unused00[0x91];
    unsigned char flags91;
};

class CPlayer
{
public:
    unsigned char unused00[0x28];
    int playerIndex;
    PlayerActivity* GetActivity() const;
};

struct PlayerVector
{
    CPlayer** begin;
    CPlayer** end;
    CPlayer* operator[](int index) const { return begin[index]; }
    unsigned long size() const { return end - begin; }
};

class CPlayerManager
{
public:
    unsigned char unused00[0x0C];
    PlayerVector players;
    unsigned char unused14[0x08];
    int neutralPlayerIndex;

    CPlayer* GetPlayer(int slot) const;
    bool IsMultiplayerGameActive() const;
};

bool CPlayerManager::IsMultiplayerGameActive() const
{
    int slot = 0;
    do
    {
        int playerCount = (int)players.size();
        int playerIndex = 0;
        if (playerCount > 0)
        {
            do
            {
                if (players[playerIndex]->playerIndex == slot)
                {
                    CPlayer* player = GetPlayer(slot);
                    if (player->playerIndex != neutralPlayerIndex)
                    {
                        player = GetPlayer(slot);
                        PlayerActivity* activity = player->GetActivity();
                        if (activity != 0 && (activity->flags91 & 1) == 0)
                            return true;
                    }
                    break;
                }
                ++playerIndex;
            }
            while (playerIndex < playerCount);
        }
        ++slot;
    }
    while (slot < 4);
    return false;
}