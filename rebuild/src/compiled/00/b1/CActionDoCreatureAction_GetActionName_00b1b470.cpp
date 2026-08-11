// CActionDoCreatureAction::GetActionName -> CCharString by value
// retail 0x00b1b470

struct CCharString {
    char* m_data;
    CCharString(const char* s, int n);
};

// address 0x129f608 = a global string literal's address (push imm)
extern char g_actionName_str[];

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(g_actionName_str, -1);
}