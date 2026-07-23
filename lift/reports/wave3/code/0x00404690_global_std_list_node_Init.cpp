#include <cstddef>
#include <cstdlib>

#pragma pack(push, 1)
struct StdListNode
{
    StdListNode* next;
    StdListNode* prev;
    std::byte reserved[0x8];
};
#pragma pack(pop)

static_assert(offsetof(StdListNode, next) == 0x0);
static_assert(offsetof(StdListNode, prev) == 0x4);
static_assert(sizeof(StdListNode) == 0x10);

void** __fastcall std_list_node_Init(void** nodeRef)
{
    *nodeRef = nullptr;

    auto* const node = static_cast<StdListNode*>(std::malloc(0x10));
    node->next = node;
    node->prev = node;

    *nodeRef = node;
    return nodeRef;
}