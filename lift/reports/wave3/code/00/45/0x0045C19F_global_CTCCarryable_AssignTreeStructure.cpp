struct CTCCarryable;
struct CCreatureAction_NavToPos;

void __thiscall CTCCarryable_OnKill(CTCCarryable* self);
CCreatureAction_NavToPos* __cdecl CCreatureAction_NavToPos__CloneWithTreeStructure(
    CCreatureAction_NavToPos* source,
    void* treeOwner
);

struct CTCCarryable_TreeOverlay {
    std::byte pad_00[0x04];
    CCreatureAction_NavToPos* root; // 0x04
    void* terminalAt08;             // 0x08
    void* terminalAt0C;             // 0x0C
};
static_assert(offsetof(CTCCarryable_TreeOverlay, root) == 0x04);
static_assert(offsetof(CTCCarryable_TreeOverlay, terminalAt08) == 0x08);
static_assert(offsetof(CTCCarryable_TreeOverlay, terminalAt0C) == 0x0C);

struct CTCCarryable_Overlay {
    CTCCarryable_TreeOverlay* tree; // 0x00
    void* linkAt04;                 // 0x04
};
static_assert(offsetof(CTCCarryable_Overlay, tree) == 0x00);
static_assert(offsetof(CTCCarryable_Overlay, linkAt04) == 0x04);

struct CCreatureAction_NavToPos_Overlay {
    std::byte pad_00[0x08];
    CCreatureAction_NavToPos* nextAt08; // 0x08
    CCreatureAction_NavToPos* nextAt0C; // 0x0C
};
static_assert(offsetof(CCreatureAction_NavToPos_Overlay, nextAt08) == 0x08);
static_assert(offsetof(CCreatureAction_NavToPos_Overlay, nextAt0C) == 0x0C);

CTCCarryable* __thiscall CTCCarryable_AssignTreeStructure(CTCCarryable* self, CTCCarryable* other)
{
    auto* const selfOverlay = reinterpret_cast<CTCCarryable_Overlay*>(self);
    auto* const otherOverlay = reinterpret_cast<CTCCarryable_Overlay*>(other);

    if (self != other) {
        CTCCarryable_OnKill(self);
        selfOverlay->linkAt04 = nullptr;

        if (otherOverlay->tree->root == nullptr) {
            selfOverlay->tree->root = nullptr;
            selfOverlay->tree->terminalAt08 = selfOverlay->tree;
            selfOverlay->tree->terminalAt0C = selfOverlay->tree;
        } else {
            CTCCarryable_TreeOverlay* const selfTree = selfOverlay->tree;
            selfTree->root = CCreatureAction_NavToPos__CloneWithTreeStructure(otherOverlay->tree->root, selfTree);

            auto* node08 = selfTree->root;
            CCreatureAction_NavToPos* last08;
            do {
                last08 = node08;
                node08 = reinterpret_cast<CCreatureAction_NavToPos_Overlay*>(last08)->nextAt08;
            } while (node08 != nullptr);
            selfTree->terminalAt08 = last08;

            auto* node0C = selfTree->root;
            CCreatureAction_NavToPos* last0C;
            do {
                last0C = node0C;
                node0C = reinterpret_cast<CCreatureAction_NavToPos_Overlay*>(last0C)->nextAt0C;
            } while (node0C != nullptr);
            selfTree->terminalAt0C = last0C;

            selfOverlay->linkAt04 = otherOverlay->linkAt04;
        }
    }

    return self;
}