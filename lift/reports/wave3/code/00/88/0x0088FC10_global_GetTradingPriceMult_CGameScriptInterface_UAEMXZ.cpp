#include <cstdint>

extern float DAT_013833d4;

class CGameScriptInterface
{
public:
    float __thiscall GetTradingPriceMult();
};

float __thiscall CGameScriptInterface::GetTradingPriceMult()
{
    return DAT_013833d4;
}