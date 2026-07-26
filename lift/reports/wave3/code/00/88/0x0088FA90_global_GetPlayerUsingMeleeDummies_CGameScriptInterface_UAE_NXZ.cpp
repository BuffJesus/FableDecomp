#include <cstdint>

extern bool DAT_013bae18;

class CGameScriptInterface
{
public:
    bool __thiscall GetPlayerUsingMeleeDummies();
};

bool __thiscall CGameScriptInterface::GetPlayerUsingMeleeDummies()
{
    return DAT_013bae18;
}