class CGameScriptInterface
{
public:
    void __thiscall SetPrizeTavernTable(bool enabled);
};

extern bool DAT_013833d0;

void __thiscall CGameScriptInterface::SetPrizeTavernTable(bool enabled)
{
    DAT_013833d0 = enabled;
}