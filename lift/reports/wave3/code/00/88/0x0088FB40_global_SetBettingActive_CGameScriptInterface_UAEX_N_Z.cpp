class CGameScriptInterface
{
public:
    void __thiscall SetBettingActive(bool bettingActive);
};

namespace
{
bool& DAT_013bae1d = *reinterpret_cast<bool*>(0x013BAE1D);
}

void __thiscall CGameScriptInterface::SetBettingActive(bool bettingActive)
{
    DAT_013bae1d = bettingActive;
}