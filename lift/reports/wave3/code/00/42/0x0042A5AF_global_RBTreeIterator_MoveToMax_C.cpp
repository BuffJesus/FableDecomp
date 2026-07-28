#include <cstdint>

extern "C" std::uint32_t __cdecl RBTree__GetMaxNode(std::uint32_t node);

extern "C" std::uint32_t* __fastcall RBTreeIterator_MoveToMax_C(std::uint32_t* iterator)
{
    const std::uint32_t maxNode = RBTree__GetMaxNode(*iterator);
    *iterator = maxNode;
    return iterator;
}