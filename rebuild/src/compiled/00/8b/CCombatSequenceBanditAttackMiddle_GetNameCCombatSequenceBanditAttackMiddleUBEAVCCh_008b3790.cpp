// CCombatSequence_BanditAttackMiddle::GetName -> CCharString (by value, virtual)

class CCharString {
public:
    // ctor at 0x99ebf0: CCharString(const char* s, int n)
    CCharString(const char* s, int n);
private:
    void* m_data;
};

class CActionDoCreatureAction; // param type (unused in body)

class CCombatSequence_BanditAttackMiddle {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditAttackMiddle::GetName() const
{
    return CCharString((const char*)0x1278790, -1);
}