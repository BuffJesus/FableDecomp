#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

struct LTextBinNode;
struct LTextGroup;
struct LTextTreeWalkThrough;

template <typename T>
class LTextBinTree;

template <>
class LTextBinTree<LTextGroup*>
{
public:
    class LTextTreeWalkThrough
    {
    public:
        static void __thiscall BuildTreeArray(
            ::LTextTreeWalkThrough* self,
            LTextBinNode* node);
    };
};

namespace
{
struct GossipCategorySetOverlay
{
    std::byte unknown00[0x04];
    LTextBinNode* root_list_head; // +0x04
    std::uint32_t begin;          // +0x08
    std::uint32_t end;            // +0x0C
};

static_assert(offsetof(GossipCategorySetOverlay, root_list_head) == 0x04);
static_assert(offsetof(GossipCategorySetOverlay, begin) == 0x08);
static_assert(offsetof(GossipCategorySetOverlay, end) == 0x0C);

extern "C" std::uint32_t DAT_013bae38;
extern "C" std::int32_t DAT_013bae3c;
}

void __thiscall CGameScriptInterface::ClearIsGossipForPlayer()
{
    if (DAT_013bae3c != 0) {
        GossipCategorySetOverlay* const gossip_category_set =
            reinterpret_cast<GossipCategorySetOverlay*>(DAT_013bae38);

        LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray(
            reinterpret_cast<::LTextTreeWalkThrough*>(&DAT_013bae38),
            *reinterpret_cast<LTextBinNode**>(DAT_013bae38 + 0x04));

        gossip_category_set->begin = DAT_013bae38;
        gossip_category_set->root_list_head = nullptr;
        gossip_category_set->end = DAT_013bae38;
        DAT_013bae3c = 0;
    }
}