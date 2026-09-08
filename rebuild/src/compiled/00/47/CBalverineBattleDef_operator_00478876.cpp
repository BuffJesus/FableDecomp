#pragma optimize("s", on)
struct CBalverineBattleDefBaseRetail {
    unsigned char State[0x3c];
    CBalverineBattleDefBaseRetail& operator=(const CBalverineBattleDefBaseRetail& other);
};
// Ego_r places these PDB-named fields at +0x40/+0x44/+0x48. Retail's
// definition base is four bytes smaller, placing them at +0x3c/+0x40/+0x44.
struct CBalverineBattleDefRetail : CBalverineBattleDefBaseRetail {
    float SecondsBetweenLunges;
    float LungeAttackDamage;
    float TimeInTrees;
    CBalverineBattleDefRetail& operator=(const CBalverineBattleDefRetail& other);
};
CBalverineBattleDefRetail& CBalverineBattleDefRetail::operator=(const CBalverineBattleDefRetail& other)
{
    CBalverineBattleDefBaseRetail::operator=(other);
    SecondsBetweenLunges = other.SecondsBetweenLunges;
    LungeAttackDamage = other.LungeAttackDamage;
    TimeInTrees = other.TimeInTrees;
    return *this;
}