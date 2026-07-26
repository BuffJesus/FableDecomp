#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void __thiscall OracleSymbolPushBack(const CCharString& scriptBrush);
};
}

namespace
{
struct CGameScriptInterface_ApplyScriptBrush_Overlay
{
    std::byte pad_0000[0x04];
    void* field_0004;
};

static_assert(offsetof(CGameScriptInterface_ApplyScriptBrush_Overlay, field_0004) == 0x04);

struct CGameScriptInterface_ApplyScriptBrush_TargetOverlay
{
    void** vftable;
};

static_assert(offsetof(CGameScriptInterface_ApplyScriptBrush_TargetOverlay, vftable) == 0x00);

using RawGetDrawQuestInfoFn = NPlayerGui::CDrawQuestInfo*(__thiscall*)(
    CGameScriptInterface_ApplyScriptBrush_TargetOverlay*,
    const CCharString&,
    int);
}

class CGameScriptInterface
{
public:
    void __thiscall ApplyScriptBrush(const CCharString& scriptBrush) const;
};

void __thiscall CGameScriptInterface::ApplyScriptBrush(const CCharString& scriptBrush) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_ApplyScriptBrush_Overlay*>(this);
    auto* const target =
        reinterpret_cast<CGameScriptInterface_ApplyScriptBrush_TargetOverlay*>(self->field_0004);

    const auto getDrawQuestInfo =
        reinterpret_cast<RawGetDrawQuestInfoFn>(target->vftable[0x30 / sizeof(void*)]);
    NPlayerGui::CDrawQuestInfo* const drawQuestInfo = getDrawQuestInfo(target, scriptBrush, 1);

    drawQuestInfo->OracleSymbolPushBack(scriptBrush);
}