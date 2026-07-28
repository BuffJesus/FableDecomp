struct CCharString;

struct BinarySearchTreeNodeOverlay
{
    std::uint8_t _pad0[0x04];
    BinarySearchTreeNodeOverlay* initial_search_node; // 0x04
    BinarySearchTreeNodeOverlay* left;                // 0x08
    BinarySearchTreeNodeOverlay* right;               // 0x0C
    CCharString key;                                  // 0x10
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, initial_search_node) == 0x04);
static_assert(offsetof(BinarySearchTreeNodeOverlay, left) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, right) == 0x0C);
static_assert(offsetof(BinarySearchTreeNodeOverlay, key) == 0x10);

struct BinarySearchTreeOverlay
{
    BinarySearchTreeNodeOverlay* header; // 0x00
    std::uint8_t _pad4[0x04];
    std::less<CCharString> comp;         // 0x08
};

static_assert(offsetof(BinarySearchTreeOverlay, header) == 0x00);
static_assert(offsetof(BinarySearchTreeOverlay, comp) == 0x08);

BinarySearchTreeNodeOverlay* __thiscall BinarySearchTree_LowerBound(
    BinarySearchTreeOverlay* self,
    CCharString* value)
{
    BinarySearchTreeNodeOverlay* const header = self->header;
    BinarySearchTreeNodeOverlay* current = header->initial_search_node;
    BinarySearchTreeNodeOverlay* candidate = header;

    while (current != nullptr)
    {
        if (self->comp(current->key, *value))
        {
            current = current->right;
        }
        else
        {
            candidate = current;
            current = current->left;
        }
    }

    if (candidate != header && !self->comp(*value, candidate->key))
    {
        return candidate;
    }

    return header;
}