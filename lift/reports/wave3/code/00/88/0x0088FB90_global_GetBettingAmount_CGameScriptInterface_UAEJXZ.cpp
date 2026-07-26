#include <cstdint>

extern std::int32_t DAT_013bae20;

class CGameScriptInterface
{
public:
    long __thiscall GetBettingAmount();
};

long __thiscall CGameScriptInterface::GetBettingAmount()
{
    return DAT_013bae20;
}