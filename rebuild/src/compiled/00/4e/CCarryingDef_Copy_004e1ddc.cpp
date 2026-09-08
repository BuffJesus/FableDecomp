#pragma optimize("s",on)
struct CDefClassBase { void Copy(const CDefClassBase* source); };
struct CDefString { unsigned long Value; };
struct CLongVectorRetail {
    unsigned long* Begin;
    unsigned long* End;
    unsigned long* CapacityEnd;
    CLongVectorRetail& operator=(const CLongVectorRetail& source);
};
// Retail's 12-byte vector places OverriddenDummyObject at +0x34;
// Ego_r's debug vector is 16 bytes and places the same PDB member at +0x38.
struct CCarryingDefRetail : CDefClassBase {
    unsigned char Reserved[0x28];
    CLongVectorRetail AvailableCarrySlots;
    CDefString OverriddenDummyObject;
    void Copy(const CDefClassBase* source);
};
void CCarryingDefRetail::Copy(const CDefClassBase* sourceBase)
{
    CDefClassBase::Copy(sourceBase);
    const CCarryingDefRetail* source = (const CCarryingDefRetail*)sourceBase;
    AvailableCarrySlots = source->AvailableCarrySlots;
    OverriddenDummyObject = source->OverriddenDummyObject;
}