struct CCharString {
    int d;
    CCharString(const char* lit, int len);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x123d538, -1);
}