// CActionDoCreatureAction::GetActionName  @ 0x00903640
// Returns a CCharString by value: constructs it from a fixed literal + (-1).
// __fastcall struct-return: the hidden return buffer is a stack arg cleaned by ret 4.

struct CCharString {
    char* m_data;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("CACTION_DO_CREATURE_ACTION", -1);
}