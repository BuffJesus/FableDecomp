#include <cstddef>
#include <cstdlib>

struct LinkedListNodeOverlay {
    LinkedListNodeOverlay* prev; // 0x00
    LinkedListNodeOverlay* next; // 0x04
};

static_assert(offsetof(LinkedListNodeOverlay, prev) == 0x00);
static_assert(offsetof(LinkedListNodeOverlay, next) == 0x04);

void __stdcall LinkedList_RemoveNode(LinkedListNodeOverlay** param_1, LinkedListNodeOverlay* param_2)
{
    LinkedListNodeOverlay* const piVar1 = param_2->next;
    LinkedListNodeOverlay* const iVar2 = param_2->prev;

    piVar1->prev = iVar2;
    iVar2->next = piVar1;
    std::free(param_2);
    *param_1 = iVar2;
}