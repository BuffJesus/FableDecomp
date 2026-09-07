#include <cstddef>
#include <cstdint>

class CDataOutputStream {
public:
    void WriteCBYTE(char value);
};

struct BinarySearchTreeNodeOverlay {
    std::byte pad_00[0x08];
    std::uint32_t field_08;
    std::byte pad_0C[0x04];
    std::uint32_t field_10;
    std::uint32_t field_14;
};

static_assert(offsetof(BinarySearchTreeNodeOverlay, field_08) == 0x08);
static_assert(offsetof(BinarySearchTreeNodeOverlay, field_10) == 0x10);
static_assert(offsetof(BinarySearchTreeNodeOverlay, field_14) == 0x14);

struct BinarySearchTreeRangeOverlay {
    std::uint32_t header_node;
    std::uint32_t field_04;
};

static_assert(offsetof(BinarySearchTreeRangeOverlay, header_node) == 0x00);
static_assert(offsetof(BinarySearchTreeRangeOverlay, field_04) == 0x04);

extern "C" std::uint32_t __cdecl FUN_004293D0(std::uint32_t node);

void __fastcall BinarySearchTree_WriteRangeToStream(
    CDataOutputStream* stream,
    const BinarySearchTreeRangeOverlay* range)
{
    stream->WriteCBYTE(static_cast<char>(range->field_04));

    std::uint32_t node =
        reinterpret_cast<const BinarySearchTreeNodeOverlay*>(range->header_node)->field_08;
    if (node != range->header_node) {
        do {
            const auto* const current = reinterpret_cast<const BinarySearchTreeNodeOverlay*>(node);
            stream->WriteCBYTE(static_cast<char>(current->field_10));
            stream->WriteCBYTE(static_cast<char>(current->field_14));
            node = FUN_004293D0(node);
        } while (node != range->header_node);
    }
}