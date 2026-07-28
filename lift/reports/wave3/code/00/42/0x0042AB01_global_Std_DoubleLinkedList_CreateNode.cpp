#include <cstddef>
#include <cstdlib>

struct Std_DoubleLinkedList_NodeOverlay {
    Std_DoubleLinkedList_NodeOverlay* back;
    Std_DoubleLinkedList_NodeOverlay* forward;
    std::byte padding_08[0x0C - 0x08];
};

static_assert(offsetof(Std_DoubleLinkedList_NodeOverlay, back) == 0x0);
static_assert(offsetof(Std_DoubleLinkedList_NodeOverlay, forward) == 0x4);
static_assert(sizeof(Std_DoubleLinkedList_NodeOverlay) == 0x0C);

void** __fastcall Std_DoubleLinkedList_CreateNode(void** param_1)
{
    *param_1 = nullptr;

    void* const pvVar1 = std::malloc(0x0C);
    static_cast<Std_DoubleLinkedList_NodeOverlay*>(pvVar1)->back =
        static_cast<Std_DoubleLinkedList_NodeOverlay*>(pvVar1);
    static_cast<Std_DoubleLinkedList_NodeOverlay*>(pvVar1)->forward =
        static_cast<Std_DoubleLinkedList_NodeOverlay*>(pvVar1);

    *param_1 = pvVar1;
    return param_1;
}