inline const long& Minimum(const long& left, const long& right)
{
    return left < right ? left : right;
}
// Retail places DaySomeoneWillMoveIn at +0x18; Ego_r places the same PDB
// member at +0x14 because the owning build has a different base layout.
struct CTCBuyableHouseRetail {
    unsigned char Reserved[0x18];
    long DaySomeoneWillMoveIn;
    void MovedInNotification(long currentDay);
};
void CTCBuyableHouseRetail::MovedInNotification(long currentDay)
{
    DaySomeoneWillMoveIn = Minimum(currentDay + 1, DaySomeoneWillMoveIn);
}