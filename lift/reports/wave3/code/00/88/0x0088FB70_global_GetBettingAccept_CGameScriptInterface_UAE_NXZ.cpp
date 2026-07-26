#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint8_t __thiscall GetBettingAccept() const;
};

extern std::uint8_t DAT_013bae1e;

std::uint8_t __thiscall CGameScriptInterface::GetBettingAccept() const
{
    return DAT_013bae1e;
}