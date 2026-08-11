// CActionDoCreatureAction::GetActionName  @ 007d5ef0
// Returns a CCharString by value built from a fixed string-literal address.

class CCharString {
public:
    CCharString(const char* str, int len);
    void* m_data;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123d550, -1);
}