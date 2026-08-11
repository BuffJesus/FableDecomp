// CCombatSequence_TrollRockPursueNoZone::GetName  @ 0x008b4350
// virtual CCharString GetName() const  -- returns a fixed name string by value.

class CCharString {
public:
    // __fastcall ctor from a C string + explicit length (-1 = strlen).
    CCharString(const char* s, int len);
private:
    char* m_pData;
};

class CCombatSequence_TrollRockPursueNoZone {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockPursueNoZone::GetName() const {
    return CCharString("CCombatSequence_TrollRockPursueNoZone", -1);
}