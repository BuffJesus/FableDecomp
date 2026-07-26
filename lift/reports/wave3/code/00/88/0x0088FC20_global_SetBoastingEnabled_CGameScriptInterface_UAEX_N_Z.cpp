class CGameScriptInterface
{
public:
    void __thiscall SetBoastingEnabled(bool boastingEnabled);
};

namespace
{
bool& DAT_013833d8 = *reinterpret_cast<bool*>(0x013833D8);
}

void __thiscall CGameScriptInterface::SetBoastingEnabled(bool boastingEnabled)
{
    DAT_013833d8 = boastingEnabled;
}