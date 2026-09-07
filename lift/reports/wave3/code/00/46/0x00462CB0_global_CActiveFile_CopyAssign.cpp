#include <cstddef>
#include <cstdint>

struct CActiveFile;

struct ActiveFileEntry12 {
    std::uint32_t dword0;
    std::uint32_t dword1;
    std::uint32_t dword2;
};
static_assert(sizeof(ActiveFileEntry12) == 0x0C);

struct CActiveFile_Overlay {
    ActiveFileEntry12* begin;        // 0x00
    ActiveFileEntry12* end;          // 0x04
    ActiveFileEntry12* capacityEnd;  // 0x08
};

static_assert(sizeof(void*) == 4);
static_assert(offsetof(CActiveFile_Overlay, begin) == 0x00);
static_assert(offsetof(CActiveFile_Overlay, end) == 0x04);
static_assert(offsetof(CActiveFile_Overlay, capacityEnd) == 0x08);
static_assert(sizeof(CActiveFile_Overlay) == 0x0C);

extern ActiveFileEntry12* __cdecl Vector_AllocateAndCopy_12(
    std::uint32_t count,
    const ActiveFileEntry12* first,
    const ActiveFileEntry12* last);

extern void __thiscall OnReadFinished(CActiveFile* self);

CActiveFile* __thiscall CActiveFile_CopyAssign(CActiveFile* self, CActiveFile* other)
{
    auto& lhs = *reinterpret_cast<CActiveFile_Overlay*>(self);
    auto& rhs = *reinterpret_cast<CActiveFile_Overlay*>(other);

    if (other != self) {
        const std::uint32_t rhsCount =
            static_cast<std::uint32_t>(rhs.end - rhs.begin);

        if (static_cast<std::uint32_t>(lhs.capacityEnd - lhs.begin) < rhsCount) {
            ActiveFileEntry12* const newBegin =
                Vector_AllocateAndCopy_12(rhsCount, rhs.begin, rhs.end);
            OnReadFinished(self);
            lhs.begin = newBegin;
            lhs.capacityEnd = reinterpret_cast<ActiveFileEntry12*>(
                reinterpret_cast<std::uintptr_t>(newBegin) + rhsCount * 0x0Cu);
        } else {
            const std::uint32_t lhsSize =
                static_cast<std::uint32_t>(lhs.end - lhs.begin);

            if (lhsSize < rhsCount) {
                int copyCount = static_cast<int>((lhsSize * 0x0Cu) / 0x0Cu);
                const ActiveFileEntry12* src = rhs.begin;
                ActiveFileEntry12* dst = lhs.begin;

                if (0 < copyCount) {
                    do {
                        --copyCount;
                        *dst = *src;
                        ++src;
                        ++dst;
                    } while (copyCount != 0);
                }

                ActiveFileEntry12* dstTail = lhs.end;
                for (const ActiveFileEntry12* srcTail =
                         rhs.begin + (((lhs.end - lhs.begin) * 0x0C) / 0x0C);
                     srcTail != rhs.end;
                     ++srcTail) {
                    if (dstTail != nullptr) {
                        *dstTail = *srcTail;
                    }
                    ++dstTail;
                }
            } else {
                int copyCount = static_cast<int>(rhs.end - rhs.begin);
                ActiveFileEntry12* dst = lhs.begin;
                const ActiveFileEntry12* src = rhs.begin;

                if (0 < copyCount) {
                    do {
                        *dst = *src;
                        ++src;
                        --copyCount;
                        ++dst;
                    } while (copyCount != 0);
                }
            }
        }

        lhs.end = lhs.begin + rhsCount;
    }

    return self;
}