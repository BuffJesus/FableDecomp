#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    void __thiscall DisplayMoneyBag(bool display) const;
};

namespace
{
struct DisplayMoneyBagGlobalOverlay
{
    std::byte pad00[0x29E];
    std::uint8_t field_29E; // 0x29E
};

static_assert(offsetof(DisplayMoneyBagGlobalOverlay, field_29E) == 0x29E);

extern "C" std::uint32_t DAT_013b8790;
}

void __thiscall CGameScriptInterface::DisplayMoneyBag(bool display) const
{
    reinterpret_cast<DisplayMoneyBagGlobalOverlay*>(DAT_013b8790)->field_29E =
        static_cast<std::uint8_t>(display);
}