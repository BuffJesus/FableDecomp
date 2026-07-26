#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint8_t __thiscall GetBettingActive() const;
};

extern std::uint8_t DAT_013bae1d;

std::uint8_t __thiscall CGameScriptInterface::GetBettingActive() const
{
    return DAT_013bae1d;
}