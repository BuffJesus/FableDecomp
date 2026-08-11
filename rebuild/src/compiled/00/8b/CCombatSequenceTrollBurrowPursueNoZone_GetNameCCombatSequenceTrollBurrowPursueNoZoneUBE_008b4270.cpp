// CCombatSequence_TrollBurrowPursueNoZone::GetName  @ 0x008b4270
// Returns a CCharString by value built from a literal name pointer.

class CCharString {
public:
    CCharString(const char* s, int len);
    char* m_data;
};

class CCombatSequence_TrollBurrowPursueNoZone {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollBurrowPursueNoZone::GetName() const {
    return CCharString((const char*)0x1278ac0, -1);
}