#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint8_t __thiscall GetCheapHeadLooking() const;
};

extern std::uint8_t DAT_013bae1b;

std::uint8_t __thiscall CGameScriptInterface::GetCheapHeadLooking() const
{
    return DAT_013bae1b;
}