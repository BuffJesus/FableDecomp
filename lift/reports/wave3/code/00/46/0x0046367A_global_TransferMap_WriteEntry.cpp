struct TransferMapNodeOverlay {
    std::byte _tree_node[0x10];
    CCharString key;
    std::uint32_t value;
};

static_assert(offsetof(TransferMapNodeOverlay, key) == 0x10);
static_assert(offsetof(TransferMapNodeOverlay, value) == 0x14);

struct TransferMapOverlay {
    TransferMapNodeOverlay* head;
    std::uint32_t entryCount;
};

static_assert(offsetof(TransferMapOverlay, head) == 0x0);
static_assert(offsetof(TransferMapOverlay, entryCount) == 0x4);

extern "C" TransferMapNodeOverlay* __cdecl TreeSuccessor_004292C0(TransferMapNodeOverlay* node);

void __fastcall TransferMap_WriteEntry(CDataOutputStream* stream, const TransferMapOverlay* mapEntry)
{
    CCharString unusedLifetimeGuard;

    stream->WriteCBYTE(static_cast<char>(mapEntry->entryCount));

    auto* node = *reinterpret_cast<TransferMapNodeOverlay**>(
        reinterpret_cast<std::byte*>(mapEntry->head) + 0x8);

    if (node != mapEntry->head) {
        do {
            stream->WriteNullTerminatedString(&node->key);
            stream->WriteCBYTE(static_cast<char>(node->value));
            node = TreeSuccessor_004292C0(node);
        } while (node != mapEntry->head);
    }
}