#include <cstddef>
#include <cstdint>

struct CScriptDataBase;
struct CGameScriptInterfaceBase;
struct CScriptBase;

namespace NScript {
struct CExpression_DigScript {
    static CScriptBase* __fastcall Alloc(
        CScriptDataBase* script_data,
        CGameScriptInterfaceBase* game_script_interface,
        void* alloc_source);
};
}

namespace {
struct DigScriptNodeOverlay {
    std::uint8_t value_00;
    std::byte pad_01[3];
    DigScriptNodeOverlay* link_04;
    DigScriptNodeOverlay* link_08;
    DigScriptNodeOverlay* link_0C;
    std::byte alloc_source_10;
};

static_assert(offsetof(DigScriptNodeOverlay, value_00) == 0x00);
static_assert(offsetof(DigScriptNodeOverlay, link_04) == 0x04);
static_assert(offsetof(DigScriptNodeOverlay, link_08) == 0x08);
static_assert(offsetof(DigScriptNodeOverlay, link_0C) == 0x0C);
static_assert(offsetof(DigScriptNodeOverlay, alloc_source_10) == 0x10);
}

CScriptBase* __fastcall CExpression_DigScript_CloneTree(
    CScriptDataBase* script_data,
    CGameScriptInterfaceBase* game_script_interface,
    CScriptBase* node,
    CScriptBase* link_04)
{
    auto* const src = reinterpret_cast<DigScriptNodeOverlay*>(node);
    auto* const clone = reinterpret_cast<DigScriptNodeOverlay*>(
        NScript::CExpression_DigScript::Alloc(
            script_data,
            game_script_interface,
            &src->alloc_source_10));

    clone->link_08 = nullptr;
    clone->link_0C = nullptr;
    clone->value_00 = src->value_00;
    clone->link_04 = reinterpret_cast<DigScriptNodeOverlay*>(link_04);

    if (src->link_0C != nullptr) {
        clone->link_0C = reinterpret_cast<DigScriptNodeOverlay*>(
            CExpression_DigScript_CloneTree(
                script_data,
                game_script_interface,
                reinterpret_cast<CScriptBase*>(src->link_0C),
                reinterpret_cast<CScriptBase*>(clone)));
    }

    auto* tail = clone;
    for (auto* sibling = src->link_08; sibling != nullptr; sibling = sibling->link_08) {
        auto* const sibling_clone = reinterpret_cast<DigScriptNodeOverlay*>(
            NScript::CExpression_DigScript::Alloc(
                script_data,
                game_script_interface,
                &sibling->alloc_source_10));

        sibling_clone->link_08 = nullptr;
        sibling_clone->link_0C = nullptr;
        sibling_clone->value_00 = sibling->value_00;
        tail->link_08 = sibling_clone;
        sibling_clone->link_04 = tail;

        if (sibling->link_0C != nullptr) {
            sibling_clone->link_0C = reinterpret_cast<DigScriptNodeOverlay*>(
                CExpression_DigScript_CloneTree(
                    script_data,
                    game_script_interface,
                    reinterpret_cast<CScriptBase*>(sibling->link_0C),
                    reinterpret_cast<CScriptBase*>(sibling_clone)));
        }

        tail = sibling_clone;
    }

    return reinterpret_cast<CScriptBase*>(clone);
}