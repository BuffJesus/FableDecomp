struct CCharString {
    int d;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x126554c, -1);
}