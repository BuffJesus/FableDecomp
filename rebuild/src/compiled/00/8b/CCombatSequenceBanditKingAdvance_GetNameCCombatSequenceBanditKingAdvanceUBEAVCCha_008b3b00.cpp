// CCombatSequence_BanditKingAdvance::GetName -- returns CCharString by value.
// Retail: builds a CCharString from a string literal (len = -1) into the
// hidden return buffer and returns it. 'this' is unused.

struct CCharString {
    char* m_ptr;
    CCharString(const char* s, int len);
};

struct CCombatSequence_BanditKingAdvance {
    void* vtbl;
    CCharString GetName() const;
};

CCharString CCombatSequence_BanditKingAdvance::GetName() const {
    return CCharString("BanditKingAdvance", -1);
}