#pragma optimize("s",on)

struct CPersistContext { void Transfer(bool&); };
struct CTavernTableDefRetail {
    unsigned char Base[0x25];
    bool AddTankards;
    bool HighQualityTankards;
    void Transfer(CPersistContext& context);
};
void CTavernTableDefRetail::Transfer(CPersistContext& context) {
    context.Transfer(AddTankards);
    context.Transfer(HighQualityTankards);
}