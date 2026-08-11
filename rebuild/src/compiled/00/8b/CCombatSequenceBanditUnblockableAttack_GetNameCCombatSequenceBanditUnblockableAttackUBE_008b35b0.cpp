// CCombatSequence_BanditUnblockableAttack::GetName -> CCharString by value
// retail 0x008b35b0

class CCharString {
public:
    CCharString(const char* s, int n);   // 0x99ebf0
private:
    char* m_data;
};

class CCombatSequence_BanditUnblockableAttack {
public:
    CCharString GetName() const;
};

CCharString CCombatSequence_BanditUnblockableAttack::GetName() const {
    return CCharString("BanditUnblockableAttack", -1);
}