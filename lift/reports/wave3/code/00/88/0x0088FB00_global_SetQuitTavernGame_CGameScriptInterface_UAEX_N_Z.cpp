class CGameScriptInterface
{
public:
    void __thiscall SetQuitTavernGame(bool enabled);
};

extern bool DAT_013bae1c;

void __thiscall CGameScriptInterface::SetQuitTavernGame(bool enabled)
{
    DAT_013bae1c = enabled;
}