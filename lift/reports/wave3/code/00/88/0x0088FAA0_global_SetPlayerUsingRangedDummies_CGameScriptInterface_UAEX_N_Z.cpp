class CGameScriptInterface
{
public:
    void __thiscall SetPlayerUsingRangedDummies(bool enabled);
};

extern bool DAT_013bae19;

void __thiscall CGameScriptInterface::SetPlayerUsingRangedDummies(bool enabled)
{
    DAT_013bae19 = enabled;
}