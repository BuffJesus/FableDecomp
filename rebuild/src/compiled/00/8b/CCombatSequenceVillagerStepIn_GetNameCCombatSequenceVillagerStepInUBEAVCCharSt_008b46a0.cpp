// CCombatSequence_VillagerStepIn::GetName  0x008b46a0
// Returns a CCharString by value, constructed from a string literal via
// CCharString::CCharString(char const*, int)  (call 0x99ebf0, arg -1).

class CCharString {
public:
    CCharString(const char* s, int len);   // declared only -> real call emitted
    void* buf;
};

class CCombatSequence_VillagerStepIn {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_VillagerStepIn::GetName() const {
    return CCharString("Villager Step In", -1);
}