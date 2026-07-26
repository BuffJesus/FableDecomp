#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint8_t __thiscall GetBoastingEnabled() const;
};

extern std::uint8_t DAT_013833d8;

std::uint8_t __thiscall CGameScriptInterface::GetBoastingEnabled() const
{
    return DAT_013833d8;
}