// ?IsActionReadyForInterruption@CCombatActionBase@@UBE_NXZ  0x00856170
// bool __fastcall CCombatActionBase::IsActionReadyForInterruption(void) const
struct CCombatActionBase {
    virtual bool IsActionReadyForInterruption() const; // vptr at +0
    char pad[0xd8 - 4];  // vptr occupies +0..+3
    bool m_flagA;        // +0xd8
    char pad2[2];        // +0xd9, +0xda
    bool m_flagB;        // +0xdb
};

bool CCombatActionBase::IsActionReadyForInterruption() const
{
    if (m_flagA && m_flagB)
        return true;
    return false;
}