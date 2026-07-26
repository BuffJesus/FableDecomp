#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

enum EMusicSetType : std::int32_t;

namespace
{
struct CGameScriptInterface_CacheMusicSet_Overlay
{
    std::byte pad_0000[0x04];
    void* field_0004;
};

static_assert(offsetof(CGameScriptInterface_CacheMusicSet_Overlay, field_0004) == 0x04);

struct CGameScriptInterface_CacheMusicSet_TargetOverlay
{
    std::byte pad_0000[0x6C];
    void* field_006C;
};

static_assert(offsetof(CGameScriptInterface_CacheMusicSet_TargetOverlay, field_006C) == 0x6C);
}

class CGameScriptInterface
{
public:
    void __thiscall CacheMusicSet(EMusicSetType musicSetType) const;
};

void __thiscall CGameScriptInterface::CacheMusicSet(const EMusicSetType musicSetType) const
{
    auto* const overlay = reinterpret_cast<const CGameScriptInterface_CacheMusicSet_Overlay*>(this);
    auto* const target_overlay =
        reinterpret_cast<const CGameScriptInterface_CacheMusicSet_TargetOverlay*>(overlay->field_0004);

    using CacheMusicSet_TargetFn = void(__thiscall*)(void* this_ptr, EMusicSetType musicSetType);
    reinterpret_cast<CacheMusicSet_TargetFn>(0x006BD890)(target_overlay->field_006C, musicSetType);
}