#include <cstddef>
#include <cstdint>

namespace
{
struct CPersistInfo_InsertionSortStep_PairOverlay
{
    std::int32_t field00;  // +0x00
    std::uint32_t field04; // +0x04
    std::uint32_t field08; // +0x08
};
static_assert(sizeof(CPersistInfo_InsertionSortStep_PairOverlay) == 0x0C);
static_assert(offsetof(CPersistInfo_InsertionSortStep_PairOverlay, field00) == 0x00);
static_assert(offsetof(CPersistInfo_InsertionSortStep_PairOverlay, field04) == 0x04);
static_assert(offsetof(CPersistInfo_InsertionSortStep_PairOverlay, field08) == 0x08);

void __fastcall std___Move_backward(
    CPersistInfo_InsertionSortStep_PairOverlay* first,
    CPersistInfo_InsertionSortStep_PairOverlay* last,
    CPersistInfo_InsertionSortStep_PairOverlay* dst,
    void* scratch,
    int zero);

void __thiscall std__pair_insertion_sort_shift(
    CPersistInfo_InsertionSortStep_PairOverlay* hole,
    CPersistInfo_InsertionSortStep_PairOverlay value,
    std::uint32_t scratch);
}

void __fastcall CPersistInfo_InsertionSortStep_0086fa00(
    CPersistInfo_InsertionSortStep_PairOverlay* first,
    CPersistInfo_InsertionSortStep_PairOverlay* hole,
    std::int32_t valueField00,
    std::uint32_t valueField04,
    std::uint32_t valueField08,
    std::uint32_t scratch)
{
    if (valueField00 < first->field00)
    {
        std___Move_backward(first, hole, hole + 1, &scratch, 0);
        first->field00 = valueField00;
        first->field04 = valueField04;
        first->field08 = valueField08;
        return;
    }

    const CPersistInfo_InsertionSortStep_PairOverlay value{
        valueField00,
        valueField04,
        valueField08,
    };
    std__pair_insertion_sort_shift(hole, value, scratch);
}