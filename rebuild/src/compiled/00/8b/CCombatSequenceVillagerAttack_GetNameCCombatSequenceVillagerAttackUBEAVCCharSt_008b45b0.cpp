// CCombatSequence_VillagerAttack::GetName -> CCharString by value
// retail 0x008b45b0

class CCharString {
public:
    CCharString(const char* s, int n);   // 0x99ebf0
private:
    char* m_data;
};

class CCombatSequence_VillagerAttack {
public:
    CCharString GetName() const;
};

CCharString CCombatSequence_VillagerAttack::GetName() const {
    return CCharString("VillagerAttack", -1);
}