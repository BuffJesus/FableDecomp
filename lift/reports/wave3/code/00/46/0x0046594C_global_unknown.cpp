#include <algorithm>
#include <cstddef>
#include <cstdint>

struct VectorInsertElementsReallocOverlay {
    std::byte pad_00[0x04];
    unsigned long* finish; // 0x04
    unsigned long* end;    // 0x08
};

static_assert(offsetof(VectorInsertElementsReallocOverlay, finish) == 0x04);
static_assert(offsetof(VectorInsertElementsReallocOverlay, end) == 0x08);

extern void __thiscall Vector_Reallocate(
    VectorInsertElementsReallocOverlay* self,
    unsigned long* position,
    unsigned long* valuePtr,
    void* countHighByte,
    unsigned int originalCount,
    int zero
);

void __thiscall Vector_InsertElements_Realloc(
    VectorInsertElementsReallocOverlay* self,
    unsigned long* position,
    unsigned int count,
    unsigned long* valuePtr
) {
    auto addr32 = [](const void* p) -> std::int32_t {
        return static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(p));
    };

    unsigned int originalCount = count;
    if (count != 0) {
        if (static_cast<unsigned int>((addr32(self->end) - addr32(self->finish)) >> 2) < count) {
            count &= 0x00FFFFFFu;
            Vector_Reallocate(
                self,
                position,
                valuePtr,
                static_cast<void*>(reinterpret_cast<std::byte*>(&count) + 3),
                originalCount,
                0
            );
        } else {
            const unsigned long fillValue = *valuePtr;
            unsigned long* const finish = self->finish;
            originalCount = static_cast<unsigned int>((addr32(self->finish) - addr32(position)) >> 2);

            if (count < originalCount) {
                unsigned long* const tailStart = finish - count;
                unsigned long* dst = finish;

                for (unsigned long* src = tailStart; src != finish; ++src) {
                    if (dst != nullptr) {
                        *dst = *src;
                    }
                    ++dst;
                }

                self->finish = self->finish + count;

                int moveCount = (addr32(tailStart) - addr32(position)) >> 2;
                if (0 < moveCount) {
                    const std::int32_t delta = addr32(tailStart) - addr32(finish);
                    unsigned long* back = finish;
                    do {
                        --back;
                        --moveCount;
                        *back = *reinterpret_cast<unsigned long*>(
                            static_cast<std::uintptr_t>(delta + addr32(back))
                        );
                    } while (moveCount != 0);
                }

                unsigned long* const insertedEnd = position + count;
                for (; position != insertedEnd; ++position) {
                    *position = fillValue;
                }
            } else {
                const unsigned int extraCount = count - originalCount;
                count = originalCount;

                std::fill_n(finish, extraCount, fillValue);
                self->finish = self->finish + extraCount;

                unsigned long* dst = self->finish;
                for (unsigned long* src = position; src != finish; ++src) {
                    if (dst != nullptr) {
                        *dst = *src;
                    }
                    ++dst;
                }

                self->finish = self->finish + count;

                for (; position != finish; ++position) {
                    *position = fillValue;
                }
            }
        }
    }
}