#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CActiveFileCopyConstructElement
{
    std::uint32_t value0;
    float value4;
};

static_assert(sizeof(CActiveFileCopyConstructElement) == 0x8);
static_assert(offsetof(CActiveFileCopyConstructElement, value0) == 0x0);
static_assert(offsetof(CActiveFileCopyConstructElement, value4) == 0x4);

CActiveFileCopyConstructElement* __stdcall CActiveFile_CopyConstruct(
    int count,
    const CActiveFileCopyConstructElement* srcBegin,
    const CActiveFileCopyConstructElement* srcEnd)
{
    void* storage;

    if (count == 0) {
        storage = nullptr;
    } else {
        const std::uint32_t allocationSize = static_cast<std::uint32_t>(count) << 3;
        storage = std::malloc(static_cast<std::size_t>(allocationSize));
    }

    if (srcBegin != srcEnd) {
        const std::int32_t delta =
            static_cast<std::int32_t>(
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(storage)) -
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(srcBegin)));

        do {
            auto* const dst =
                reinterpret_cast<CActiveFileCopyConstructElement*>(
                    static_cast<std::uintptr_t>(
                        static_cast<std::uint32_t>(delta) +
                        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(srcBegin))));

            if (dst != nullptr) {
                dst->value0 = srcBegin->value0;
                dst->value4 = srcBegin->value4;
            }

            srcBegin += 1;
        } while (srcBegin != srcEnd);
    }

    return static_cast<CActiveFileCopyConstructElement*>(storage);
}