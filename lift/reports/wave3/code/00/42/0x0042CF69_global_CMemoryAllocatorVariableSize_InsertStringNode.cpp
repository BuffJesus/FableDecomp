struct CMemoryAllocatorVariableSize_InsertStringNode_TreeOverlay final {
    unsigned long header;
    unsigned long nodeCount;
    std::less<CCharString> less;
};

static_assert(offsetof(CMemoryAllocatorVariableSize_InsertStringNode_TreeOverlay, header) == 0x00);
static_assert(offsetof(CMemoryAllocatorVariableSize_InsertStringNode_TreeOverlay, nodeCount) == 0x04);
static_assert(offsetof(CMemoryAllocatorVariableSize_InsertStringNode_TreeOverlay, less) == 0x08);

struct CMemoryAllocatorVariableSize_InsertStringNode_NodeOverlay final {
    unsigned long field_0x00;
    unsigned long field_0x04;
    unsigned long field_0x08;
    int field_0x0C;
    CCharString key;
};

static_assert(offsetof(CMemoryAllocatorVariableSize_InsertStringNode_NodeOverlay, field_0x08) == 0x08);
static_assert(offsetof(CMemoryAllocatorVariableSize_InsertStringNode_NodeOverlay, field_0x0C) == 0x0C);
static_assert(offsetof(CMemoryAllocatorVariableSize_InsertStringNode_NodeOverlay, key) == 0x10);

unsigned long* __thiscall ::CMemoryAllocatorVariableSize_InsertStringNode(
    CMemoryAllocatorVariableSize_InsertStringNode_TreeOverlay* const self,
    unsigned long* const result,
    unsigned long hintNode,
    CCharString* const stringKey)
{
    auto* const headerNode =
        reinterpret_cast<CMemoryAllocatorVariableSize_InsertStringNode_NodeOverlay*>(self->header);

    auto node_from = [](unsigned long nodeAddress)
        -> CMemoryAllocatorVariableSize_InsertStringNode_NodeOverlay* {
        return reinterpret_cast<CMemoryAllocatorVariableSize_InsertStringNode_NodeOverlay*>(nodeAddress);
    };

    unsigned long parentOrNeighbor = hintNode;

    if (hintNode == headerNode->field_0x08) {
        if (self->nodeCount == 0) {
            unsigned long insertedNodeStorage{};
            unsigned long* const inserted =
                RBTree_InsertStringKey(reinterpret_cast<unsigned char*>(&insertedNodeStorage), stringKey);
            *result = *inserted;
            return result;
        }

        if (self->less(*stringKey, node_from(hintNode)->key)) {
            BinaryTree_InsertCString(result, hintNode, parentOrNeighbor, stringKey, 0);
            return result;
        }

        if (!self->less(node_from(hintNode)->key, *stringKey)) {
            *result = hintNode;
            return result;
        }

        unsigned long successor = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(
            reinterpret_cast<CMemoryAllocatorVariableSize*>(hintNode));
        if (successor == self->header) {
            BinaryTree_InsertCString(result, 0, parentOrNeighbor, stringKey, hintNode);
            return result;
        }

        if (!self->less(*stringKey, node_from(successor)->key)) {
            unsigned long insertedNodeStorage{};
            unsigned long* const inserted =
                RBTree_InsertStringKey(reinterpret_cast<unsigned char*>(&insertedNodeStorage), stringKey);
            *result = *inserted;
            return result;
        }

        if (node_from(hintNode)->field_0x0C != 0) {
            parentOrNeighbor = successor;
            BinaryTree_InsertCString(result, successor, parentOrNeighbor, stringKey, 0);
            return result;
        }

        BinaryTree_InsertCString(result, 0, parentOrNeighbor, stringKey, hintNode);
        return result;
    }

    if (hintNode == self->header) {
        parentOrNeighbor = headerNode->field_0x0C;
        if (self->less(node_from(parentOrNeighbor)->key, *stringKey)) {
            BinaryTree_InsertCString(result, 0, parentOrNeighbor, stringKey, hintNode);
            return result;
        }

        unsigned long insertedNodeStorage{};
        unsigned long* const inserted =
            RBTree_InsertStringKey(reinterpret_cast<unsigned char*>(&insertedNodeStorage), stringKey);
        *result = *inserted;
        return result;
    }

    unsigned long maxNode = RBTree__GetMaxNode(hintNode);
    const bool keyLessThanHint = self->less(*stringKey, node_from(hintNode)->key);

    if (!keyLessThanHint || !self->less(node_from(maxNode)->key, *stringKey)) {
        unsigned long successor = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(
            reinterpret_cast<CMemoryAllocatorVariableSize*>(hintNode));
        bool localDecision = !keyLessThanHint;

        if (!keyLessThanHint) {
            localDecision = self->less(node_from(hintNode)->key, *stringKey);
            if (!localDecision) {
                *result = hintNode;
                return result;
            }

            if (successor == self->header || self->less(*stringKey, node_from(successor)->key)) {
                if (node_from(hintNode)->field_0x0C != 0) {
                    BinaryTree_InsertCString(result, successor, successor, stringKey, 0);
                    return result;
                }

                BinaryTree_InsertCString(result, 0, parentOrNeighbor, stringKey, hintNode);
                return result;
            }
        }

        if (keyLessThanHint == localDecision) {
            *result = hintNode;
            return result;
        }

        unsigned long insertedNodeStorage{};
        unsigned long* const inserted =
            RBTree_InsertStringKey(reinterpret_cast<unsigned char*>(&insertedNodeStorage), stringKey);
        *result = *inserted;
        return result;
    }

    if (node_from(maxNode)->field_0x0C == 0) {
        BinaryTree_InsertCString(result, 0, maxNode, stringKey, maxNode);
        return result;
    }

    BinaryTree_InsertCString(result, hintNode, parentOrNeighbor, stringKey, 0);
    return result;
}