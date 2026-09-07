#include <cstddef>
#include <cstdint>

struct LinkedListNodeTransferBinaryMetadataOverlay {
    std::byte pad_00[0x08];
    LinkedListNodeTransferBinaryMetadataOverlay* next;
    std::byte pad_0C[0x04];
    std::uint32_t byteValue;
    float floatValue;
};

static_assert(offsetof(LinkedListNodeTransferBinaryMetadataOverlay, next) == 0x08);
static_assert(offsetof(LinkedListNodeTransferBinaryMetadataOverlay, byteValue) == 0x10);
static_assert(offsetof(LinkedListNodeTransferBinaryMetadataOverlay, floatValue) == 0x14);

struct LinkedListTransferBinaryMetadataOverlay {
    LinkedListNodeTransferBinaryMetadataOverlay* head;
    std::uint32_t listByte;
};

static_assert(offsetof(LinkedListTransferBinaryMetadataOverlay, head) == 0x00);
static_assert(offsetof(LinkedListTransferBinaryMetadataOverlay, listByte) == 0x04);

void __fastcall LinkedList_TransferBinaryMetadata(
    CDataOutputStream* stream,
    unsigned long* rawList
) {
    auto* const list =
        reinterpret_cast<LinkedListTransferBinaryMetadataOverlay*>(rawList);

    using WriteFloatFn = void(__thiscall*)(CDataOutputStream*, float);
    using GetNextNodeFn =
        LinkedListNodeTransferBinaryMetadataOverlay*(__cdecl*)(
            LinkedListNodeTransferBinaryMetadataOverlay*);

    auto const WriteFloat_00993EE0 =
        reinterpret_cast<WriteFloatFn>(0x00993EE0);
    auto const GetNextNode_004292C0 =
        reinterpret_cast<GetNextNodeFn>(0x004292C0);

    stream->WriteCBYTE(static_cast<char>(list->listByte));

    LinkedListNodeTransferBinaryMetadataOverlay* node = list->head->next;
    if (node != list->head) {
        do {
            stream->WriteCBYTE(static_cast<char>(node->byteValue));
            WriteFloat_00993EE0(stream, node->floatValue);
            node = GetNextNode_004292C0(node);
        } while (node != list->head);
    }
}