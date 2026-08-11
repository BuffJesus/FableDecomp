struct CCharString {
    void* p;
    CCharString(const char* s, int n);
};
struct CActionDoCreatureAction {
    CCharString GetActionName();
};
CCharString CActionDoCreatureAction::GetActionName(){
    return CCharString((const char*)0x123a514, -1);
}