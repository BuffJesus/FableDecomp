// CActionDoCreatureAction::GetActionName -> returns CCharString by value.
// Retail: push -1; push 0x123d584; ecx=ret-buf; call CCharString::CCharString(const char*, int)
// then returns the ret buffer pointer in eax.

class CCharString {
public:
    char* m_data;
    CCharString(const char* s, int n);   // declaration only -> real call emitted
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x123d584, -1);
}