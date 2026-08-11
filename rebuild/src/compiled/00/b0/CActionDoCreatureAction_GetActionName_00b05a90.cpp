// CActionDoCreatureAction::GetActionName (returns CCharString by value)
// retail 0x00b05a90: build CCharString from literal @0x129f534 with len -1.

struct CCharString {
    char* m_data;
    CCharString(const char* s, int len);
};

// ctor @0x99ebf0
__declspec(noinline) CCharString::CCharString(const char* s, int len) {
    m_data = (char*)s;
    if (len) m_data = (char*)s + 1;
}

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x129f534, -1);
}