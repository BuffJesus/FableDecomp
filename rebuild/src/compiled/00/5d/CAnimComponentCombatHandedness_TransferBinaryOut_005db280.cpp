struct CDataOutputStream { void Write(long value); };
enum ECombatAnimationHandedness { CombatHandedness_Default = 0 };
struct CAnimComponentCombatHandedness {
    unsigned char Reserved[8];
    ECombatAnimationHandedness StartHandedness;
    ECombatAnimationHandedness EndHandedness;
    void TransferBinaryOut(CDataOutputStream& output) const;
};
void CAnimComponentCombatHandedness::TransferBinaryOut(CDataOutputStream& output) const
{
    output.Write(StartHandedness);
    output.Write(EndHandedness);
}