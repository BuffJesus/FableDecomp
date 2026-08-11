class CCharString
{
public:
    CCharString(const char* pText, int length);
};

class CActionDoCreatureAction
{
public:
    CCharString GetActionName();
};

namespace
{
const char* const kActionNameLiteral =
    reinterpret_cast<const char*>(0x0123cb08);
}

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionNameLiteral, -1);
}