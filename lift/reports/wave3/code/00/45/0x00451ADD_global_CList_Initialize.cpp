#include <cstddef>
#include <cstdint>

struct CListNodeOverlay {
    std::uint8_t value0;              // 0x00
    std::byte pad_01[3];              // 0x01
    std::uint32_t value4;             // 0x04
    std::int32_t self_at_08;          // 0x08
    std::int32_t self_at_0C;          // 0x0C
};

static_assert(offsetof(CListNodeOverlay, value0) == 0x00);
static_assert(offsetof(CListNodeOverlay, value4) == 0x04);
static_assert(offsetof(CListNodeOverlay, self_at_08) == 0x08);
static_assert(offsetof(CListNodeOverlay, self_at_0C) == 0x0C);

struct CListOverlay {
    std::int32_t node_ptr;            // 0x00
    std::int32_t count;               // 0x04
};

static_assert(offsetof(CListOverlay, node_ptr) == 0x00);
static_assert(offsetof(CListOverlay, count) == 0x04);

void CList_AllocateNode(std::uint32_t);

std::int32_t* __thiscall CList_Initialize(
    std::int32_t* self,
    std::uint32_t /*unused*/,
    std::uint32_t allocate_arg)
{
    CList_AllocateNode(allocate_arg);

    auto* const list = reinterpret_cast<CListOverlay*>(self);
    list->count = 0;

    auto* const node =
        reinterpret_cast<CListNodeOverlay*>(static_cast<std::uintptr_t>(list->node_ptr));

    node->value0 = 0;
    node->value4 = 0;
    node->self_at_08 = list->node_ptr;
    node->self_at_0C = list->node_ptr;

    return self;
}