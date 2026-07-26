class CGameScriptInterface
{
public:
    void __thiscall SetBettingAccept(bool bettingAccept);
};

namespace
{
bool& DAT_013bae1e = *reinterpret_cast<bool*>(0x013BAE1E);
}

void __thiscall CGameScriptInterface::SetBettingAccept(bool bettingAccept)
{
    DAT_013bae1e = bettingAccept;
}