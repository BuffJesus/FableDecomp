#include <cstdint>

class CGameScriptInterface
{
public:
    void __thiscall FixMovieSequenceCamera(bool param_1) const;
};

namespace
{
inline constexpr auto DAT_01380C30 = reinterpret_cast<std::uint8_t*>(0x01380C30);
}

void __thiscall CGameScriptInterface::FixMovieSequenceCamera(bool param_1) const
{
    *DAT_01380C30 = param_1 ? 0u : 1u;
}