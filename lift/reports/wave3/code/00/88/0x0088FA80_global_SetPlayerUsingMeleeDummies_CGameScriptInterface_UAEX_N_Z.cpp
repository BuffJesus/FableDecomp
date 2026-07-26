class CGameScriptInterface
{
public:
    void __thiscall SetPlayerUsingMeleeDummies(bool enabled);
};

extern bool DAT_013bae18;

void __thiscall CGameScriptInterface::SetPlayerUsingMeleeDummies(bool enabled)
{
    DAT_013bae18 = enabled;
}