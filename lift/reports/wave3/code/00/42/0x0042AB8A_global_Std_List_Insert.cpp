#include <cstddef>
#include <cstdint>

struct StdListNodeOverlay
{
    StdListNodeOverlay* next; // 0x00
    StdListNodeOverlay* prev; // 0x04
};

static_assert(offsetof(StdListNodeOverlay, next) == 0x00);
static_assert(offsetof(StdListNodeOverlay, prev) == 0x04);
static_assert(sizeof(StdListNodeOverlay) == 0x08);

extern "C" void* __cdecl Std_List_AllocateNode(std::uint32_t value);

extern "C" StdListNodeOverlay** __cdecl Std_List_Insert(
    StdListNodeOverlay** param_1,
    StdListNodeOverlay* param_2,
    std::uint32_t param_3)
{
    StdListNodeOverlay* const piVar2 =
        static_cast<StdListNodeOverlay*>(Std_List_AllocateNode(param_3));
    StdListNodeOverlay* const piVar1 = param_2->prev;

    piVar2->next = param_2;
    piVar2->prev = piVar1;
    piVar1->next = piVar2;
    param_2->prev = piVar2;
    *param_1 = piVar2;
    return param_1;
}