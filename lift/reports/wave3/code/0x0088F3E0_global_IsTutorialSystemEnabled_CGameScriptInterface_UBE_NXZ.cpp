#include <cstddef>
#include <cstdint>

class CCodeSectionManager;

template <typename T>
class CTBaseSingleton
{
public:
    static T* __cdecl Get();
};

class CGameScriptInterface
{
public:
    bool __thiscall IsTutorialSystemEnabled() const;
};

namespace
{
struct CCodeSectionManagerTutorialOverlay
{
    std::byte pad_00[0x0C];
    std::uint8_t field_0C;
    std::byte pad_0D[0x17 - 0x0D];
    std::uint8_t field_17;
    std::byte pad_18[0xD7 - 0x18];
    std::uint8_t field_D7;
};
static_assert(offsetof(CCodeSectionManagerTutorialOverlay, field_0C) == 0x0C);
static_assert(offsetof(CCodeSectionManagerTutorialOverlay, field_17) == 0x17);
static_assert(offsetof(CCodeSectionManagerTutorialOverlay, field_D7) == 0xD7);
}

bool __thiscall CGameScriptInterface::IsTutorialSystemEnabled() const
{
    const auto* const codeSectionManager =
        reinterpret_cast<const CCodeSectionManagerTutorialOverlay*>(
            CTBaseSingleton<CCodeSectionManager>::Get());

    if (codeSectionManager->field_0C != 0) {
        return codeSectionManager->field_D7 != 0;
    }

    return codeSectionManager->field_17 != 0;
}