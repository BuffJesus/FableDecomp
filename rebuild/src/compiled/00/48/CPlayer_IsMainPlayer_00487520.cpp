// Retail 0x00487520. PDB names identify +0x10 as PlayerManager and
// +0x28 as this player's Number; main-player status is an ID comparison.
struct CPlayerManager { long GetMainPlayerNumber() const; };
typedef unsigned char FableBool;
struct CPlayer {
    char m_leading[0x10];
    CPlayerManager* PlayerManager;
    char m_between14And28[0x14];
    long Number;
    FableBool IsMainPlayer() const;
};
FableBool CPlayer::IsMainPlayer() const {
    long playerNumber = Number;
    return playerNumber == PlayerManager->GetMainPlayerNumber();
}