#include <cstdint>

extern std::int32_t DAT_013bae20;

class CGameScriptInterface
{
public:
    virtual void __thiscall SetBettingAmount(long amount);
};

void __thiscall CGameScriptInterface::SetBettingAmount(long amount)
{
    DAT_013bae20 = static_cast<std::int32_t>(amount);
}