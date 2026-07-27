#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

template <typename T>
struct CBasicString;

template <>
struct CBasicString<char> {
    static long __cdecl Compare(const char* lhs, const char* rhs);
};

struct CCharStringData {
    char* m_psz;
    std::int32_t m_nLength;
};
static_assert(offsetof(CCharStringData, m_psz) == 0x0);
static_assert(offsetof(CCharStringData, m_nLength) == 0x4);

class CCharString {
public:
    CCharStringData* m_pString;
};
static_assert(offsetof(CCharString, m_pString) == 0x0);

struct CGlobalInterfaceRootOverlay {
    std::byte pad_00[0x1C];
    void* field_1C;
};
static_assert(offsetof(CGlobalInterfaceRootOverlay, field_1C) == 0x1C);

struct CInterfaceRegistryOverlay {
    std::byte pad_00[0x3C];
    std::uint32_t flags_3C;
    std::byte pad_40[0x04];
    void* map_44;
    std::int32_t* mapEnd_48;
};
static_assert(offsetof(CInterfaceRegistryOverlay, flags_3C) == 0x3C);
static_assert(offsetof(CInterfaceRegistryOverlay, map_44) == 0x44);
static_assert(offsetof(CInterfaceRegistryOverlay, mapEnd_48) == 0x48);

struct CTCBaseOverlay {
    std::byte pad_00[0x0C];
    std::uint8_t flag_0C;
    std::byte pad_0D[0x0F];
    std::uint8_t flag_1C;
    std::byte pad_1D[0x03];
    std::uint32_t value_20;
    std::byte pad_24[0x2C];
    CCharStringData* string_50;
};
static_assert(offsetof(CTCBaseOverlay, flag_0C) == 0x0C);
static_assert(offsetof(CTCBaseOverlay, flag_1C) == 0x1C);
static_assert(offsetof(CTCBaseOverlay, value_20) == 0x20);
static_assert(offsetof(CTCBaseOverlay, string_50) == 0x50);

bool CGameScriptInterface::IsQuestStartScreenActive(CCharString& value) const
{
    (void)this;

    using GetInterfaceOwnerFn = void* (__thiscall*)(void*);
    using GetRegistryFn = CInterfaceRegistryOverlay* (__thiscall*)(void*);
    using LowerBoundFn = std::int32_t* (__thiscall*)(void*, const std::int32_t*);

    auto* const globalRoot =
        *reinterpret_cast<CGlobalInterfaceRootOverlay* const*>(0x013B86A0);

    auto* const interfaceOwner =
        reinterpret_cast<GetInterfaceOwnerFn>(0x00449970)(globalRoot->field_1C);

    auto* const registry =
        reinterpret_cast<GetRegistryFn>(0x00487DBE)(interfaceOwner);

    if ((registry->flags_3C & 0x10000000u) != 0) {
        const std::int32_t questStartScreenType = 0xFC;

        std::int32_t* entry =
            reinterpret_cast<LowerBoundFn>(0x0040F021)(&registry->map_44, &questStartScreenType);

        if ((entry == registry->mapEnd_48) || (0xFC < entry[0])) {
            entry = registry->mapEnd_48;
        }

        auto* const tcBase = reinterpret_cast<CTCBaseOverlay*>(entry[1]);

        if ((tcBase->flag_0C != 0) && (tcBase->flag_1C == 0) && (tcBase->value_20 == 0)) {
            CCharStringData* const lhs = tcBase->string_50;
            CCharStringData* const rhs = value.m_pString;

            if (lhs == rhs) {
                return true;
            }

            if ((lhs != nullptr) && (rhs != nullptr) && (lhs->m_nLength == rhs->m_nLength)) {
                const long cmp = CBasicString<char>::Compare(lhs->m_psz, rhs->m_psz);
                const std::uint8_t isEqual =
                    static_cast<std::uint8_t>(1u - static_cast<std::uint8_t>(cmp != 0));
                if (isEqual != 0) {
                    return true;
                }
            }
        }
    }

    return false;
}