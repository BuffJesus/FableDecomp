class CGameScriptInterface
{
public:
    void __thiscall SetPlayerUsingWillDummies(bool enabled);
};

extern bool DAT_013bae1a;

void __thiscall CGameScriptInterface::SetPlayerUsingWillDummies(bool enabled)
{
    DAT_013bae1a = enabled;
}