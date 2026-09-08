struct CDataOutputStream { void Write(long value); };
struct CAnimComponentCombatComboChain {
    unsigned char Reserved[8];
    long ComboStage;
    long ComboID;
    void TransferBinaryOut(CDataOutputStream& output) const;
};
void CAnimComponentCombatComboChain::TransferBinaryOut(CDataOutputStream& output) const
{
    output.Write(ComboStage);
    output.Write(ComboID);
}