#include <cstddef>
#include <cstdint>

struct CPersistContext_Overlay {
    std::byte pad_00[0x18];
    std::int32_t mode;                     // 0x18
    std::byte pad_1C[0x08];
    CMemoryDataInputStream* inputStream;   // 0x24
    CDataOutputStream* outputStream;       // 0x28
};

static_assert(offsetof(CPersistContext_Overlay, mode) == 0x18);
static_assert(offsetof(CPersistContext_Overlay, inputStream) == 0x24);
static_assert(offsetof(CPersistContext_Overlay, outputStream) == 0x28);

void __thiscall COpinionPersonalityDef_TransferArray(CPersistContext* context, int* traitsArrayPtr)
{
    CPersistContext::CheckCRC(context, "");

    const auto* const persist = reinterpret_cast<const CPersistContext_Overlay*>(context);

    if (persist->mode == 2) {
        CMemoryDataInputStream* const input = persist->inputStream;
        std::uint32_t offset = 0;

        do {
            COpinionPersonalityTraits::TransferBinaryIn(
                reinterpret_cast<COpinionPersonalityTraits*>(*traitsArrayPtr + offset),
                input);
            offset = offset + 0x24;
        } while (offset < 0xB4);
    } else if (persist->mode == 3) {
        CDataOutputStream* const output = persist->outputStream;
        std::uint32_t offset = 0;

        do {
            COpinionPersonalityTraits::TransferBinaryOut(
                reinterpret_cast<COpinionPersonalityTraits*>(*traitsArrayPtr + offset),
                output);
            offset = offset + 0x24;
        } while (offset < 0xB4);
    }
}