#include <cstdint>

extern bool DAT_013bae1c;

class CGameScriptInterface
{
public:
    bool __thiscall GetQuitTavernGame();
};

bool __thiscall CGameScriptInterface::GetQuitTavernGame()
{
    return DAT_013bae1c;
}