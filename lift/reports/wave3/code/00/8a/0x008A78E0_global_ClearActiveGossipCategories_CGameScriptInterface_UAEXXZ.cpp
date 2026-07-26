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
struct GossipCategorySetHeader
{
    std::byte color;      // +0x00
    std::byte pad01[0x03];
    void* root_list_head; // +0x04
    void* begin;          // +0x08
    void* end;            // +0x0C
};
static_assert(sizeof(GossipCategorySetHeader) == 0x10);
static_assert(offsetof(GossipCategorySetHeader, root_list_head) == 0x04);
static_assert(offsetof(GossipCategorySetHeader, begin) == 0x08);
static_assert(offsetof(GossipCategorySetHeader, end) == 0x0C);

extern "C" GossipCategorySetHeader DAT_013bae2c;
extern "C" std::int32_t DAT_013bae30;
}

void __thiscall CGameScriptInterface::ClearActiveGossipCategories()
{
    if (DAT_013bae30 != 0) {
        LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray(
            reinterpret_cast<LTextTreeWalkThrough*>(&DAT_013bae2c),
            *reinterpret_cast<LTextBinNode**>(
                reinterpret_cast<std::byte*>(&DAT_013bae2c) + 0x04));

        DAT_013bae2c.begin = &DAT_013bae2c;
        DAT_013bae2c.root_list_head = nullptr;
        DAT_013bae2c.end = &DAT_013bae2c;
        DAT_013bae30 = 0;
    }
}