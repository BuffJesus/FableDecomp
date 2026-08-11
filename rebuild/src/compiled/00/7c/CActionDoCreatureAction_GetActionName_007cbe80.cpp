// CActionDoCreatureAction::GetActionName  0x007cbe80
// __fastcall, returns CCharString by value.
// Body: return CCharString("DoCreatureAction", -1);
//   push -1 ; push <literal> ; ecx = &retval ; call CCharString::CCharString(char*, long)

class CCharString {
public:
    CCharString(const char* s, long len);
private:
    char* m_data;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}