struct StdMapNode_LowerBoundOverlay {
    std::uint8_t _pad0[0x8];
    StdMapNode_LowerBoundOverlay* left;
    StdMapNode_LowerBoundOverlay* right;
    CCharString value;
};

static_assert(offsetof(StdMapNode_LowerBoundOverlay, left) == 0x8);
static_assert(offsetof(StdMapNode_LowerBoundOverlay, right) == 0xC);
static_assert(offsetof(StdMapNode_LowerBoundOverlay, value) == 0x10);

struct StdMapHeader_LowerBoundOverlay {
    std::uint8_t _pad0[0x4];
    StdMapNode_LowerBoundOverlay* root;
};

static_assert(offsetof(StdMapHeader_LowerBoundOverlay, root) == 0x4);

int __thiscall StdMap_LowerBound(int* self, CCharString* key)
{
    auto* candidate = reinterpret_cast<StdMapNode_LowerBoundOverlay*>(*self);
    auto* const header = reinterpret_cast<StdMapHeader_LowerBoundOverlay*>(candidate);

    if (header->root != nullptr) {
        auto* node = header->root;
        do {
            const bool less_result =
                std::less<CCharString>::operator()(
                    reinterpret_cast<std::less<CCharString>*>(self + 2),
                    &node->value,
                    key);

            if (less_result) {
                node = node->right;
            } else {
                candidate = node;
                node = node->left;
            }
        } while (node != nullptr);
    }

    return reinterpret_cast<int>(candidate);
}