#include <cstddef>
#include <cstdint>

struct CTCCarryableOverlay {
    std::uint8_t typeByte;                  // 0x00
    std::uint8_t pad_01[0x03];             // 0x01
    CTCCarryableOverlay* parentOrPrev;     // 0x04
    CTCCarryableOverlay* nextSibling;      // 0x08
    CTCCarryableOverlay* firstChild;       // 0x0C
    std::uint8_t payload[0];               // 0x10
};

static_assert(offsetof(CTCCarryableOverlay, typeByte) == 0x00);
static_assert(offsetof(CTCCarryableOverlay, parentOrPrev) == 0x04);
static_assert(offsetof(CTCCarryableOverlay, nextSibling) == 0x08);
static_assert(offsetof(CTCCarryableOverlay, firstChild) == 0x0C);
static_assert(offsetof(CTCCarryableOverlay, payload) == 0x10);

extern "C" void* TreeNode_AllocData(void* data);

extern "C" CTCCarryableOverlay* CTCCarryable_CloneDeep(
    CTCCarryableOverlay* source,
    CTCCarryableOverlay* parent)
{
    auto* clone = static_cast<CTCCarryableOverlay*>(TreeNode_AllocData(source->payload));
    const std::uint8_t typeByte = source->typeByte;
    clone->nextSibling = nullptr;
    clone->firstChild = nullptr;
    clone->typeByte = typeByte;
    clone->parentOrPrev = parent;

    if (source->firstChild != nullptr) {
        clone->firstChild = CTCCarryable_CloneDeep(source->firstChild, clone);
    }

    parent = clone;
    for (CTCCarryableOverlay* sibling = source->nextSibling;
         sibling != nullptr;
         sibling = sibling->nextSibling) {
        auto* siblingClone =
            static_cast<CTCCarryableOverlay*>(TreeNode_AllocData(sibling->payload));
        const std::uint8_t siblingTypeByte = sibling->typeByte;
        siblingClone->nextSibling = nullptr;
        siblingClone->firstChild = nullptr;
        siblingClone->typeByte = siblingTypeByte;
        parent->nextSibling = siblingClone;
        siblingClone->parentOrPrev = parent;

        if (sibling->firstChild != nullptr) {
            siblingClone->firstChild = CTCCarryable_CloneDeep(sibling->firstChild, siblingClone);
        }

        parent = siblingClone;
    }

    return clone;
}