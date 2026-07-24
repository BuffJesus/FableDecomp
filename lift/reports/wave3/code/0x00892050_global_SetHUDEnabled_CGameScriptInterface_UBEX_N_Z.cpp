class CPlayerGui
{
public:
    static void __cdecl ForceOpen(CPlayerGui* playerGui);
    static void __cdecl ForceClose(CPlayerGui* playerGui);
};

class CGameScriptInterface
{
public:
    void __thiscall SetHUDEnabled(bool enabled) const;
};

extern CPlayerGui* DAT_013b8790;

void __thiscall CGameScriptInterface::SetHUDEnabled(bool enabled) const
{
    if (enabled)
    {
        CPlayerGui::ForceOpen(DAT_013b8790);
        return;
    }

    CPlayerGui::ForceClose(DAT_013b8790);
}