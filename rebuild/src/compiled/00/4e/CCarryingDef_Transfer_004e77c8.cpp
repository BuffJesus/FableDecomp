#pragma optimize("s",on)

struct CLongVectorRetail { unsigned char Storage[0x0c]; };
struct CDefStringRetail { unsigned char Storage[0x04]; };
struct CPersistContext {
    void Transfer(CLongVectorRetail&);
    void Transfer(CDefStringRetail&);
};
struct CCarryingDefRetail {
    unsigned char Base[0x28];
    CLongVectorRetail AvailableCarrySlots;
    CDefStringRetail OverriddenDummyObject;
    void Transfer(CPersistContext& context);
};
void CCarryingDefRetail::Transfer(CPersistContext& context) {
    context.Transfer(AvailableCarrySlots);
    context.Transfer(OverriddenDummyObject);
}