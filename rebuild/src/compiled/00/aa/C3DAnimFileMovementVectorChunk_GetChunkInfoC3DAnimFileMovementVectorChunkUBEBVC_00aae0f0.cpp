struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct CCreatureActionBase {
    CCharString GetStrikeEventHelperName();
};

CCharString CCreatureActionBase::GetStrikeEventHelperName()
{
    return CCharString((const char*)0x122d70e, -1);
}