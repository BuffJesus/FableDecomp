#pragma optimize("s",on)
#include <cstdlib>

struct MapEraseState
{
    unsigned char* allocatorBase;
    long count;
    void BinarySearchTree_EraseNode(unsigned long node);
};

extern "C" void* __cdecl EraseTreeNode(
    unsigned long node,
    unsigned char* linkA,
    unsigned char* linkB,
    unsigned char* linkC);

void MapEraseState::BinarySearchTree_EraseNode(unsigned long node)
{
    unsigned char* base = allocatorBase;
    void* memory = EraseTreeNode(node, base + 4, base + 8, base + 12);
    if (memory != 0)
        std::free(memory);
    --count;
}