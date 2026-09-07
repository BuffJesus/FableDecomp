#include <cstddef>
#include <cstdint>
#include <algorithm>

struct VectorOverlay {
    std::byte pad0[4];
    unsigned long* finish;       // +0x04
    unsigned long* endOfStorage; // +0x08
};

static_assert(offsetof(VectorOverlay, finish) == 0x04);
static_assert(offsetof(VectorOverlay, endOfStorage) == 0x08);

extern void __thiscall Vector_Reallocate(
    VectorOverlay* self,
    unsigned long* position,
    unsigned long* valuePtr,
    void* countEnd,
    unsigned int count,
    int unused
);

void __thiscall Vector_Insert(
    VectorOverlay* self,
    unsigned long* position,
    unsigned int count,
    unsigned long* valuePtr
) {
    unsigned int originalCount = count;

    if (count != 0) {
        if (static_cast<unsigned int>(self->endOfStorage - self->finish) < count) {
            count &= 0x00FFFFFF;
            Vector_Reallocate(
                self,
                position,
                valuePtr,
                reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(&count) + 3),
                originalCount,
                0
            );
        } else {
            const unsigned long fillValue = *valuePtr;
            unsigned long* finish = self->finish;
            const unsigned int tailCount = static_cast<unsigned int>(self->finish - position);

            if (count < tailCount) {
                unsigned long* split = finish - count;
                unsigned long* dst = finish;

                for (unsigned long* src = split; src != finish; ++src) {
                    if (dst != nullptr) {
                        *dst = *src;
                    }
                    ++dst;
                }

                self->finish = self->finish + count;

                int moveCount = static_cast<int>(split - position);
                if (0 < moveCount) {
                    const std::ptrdiff_t delta =
                        reinterpret_cast<char*>(split) - reinterpret_cast<char*>(finish);

                    do {
                        --finish;
                        --moveCount;
                        *finish = *reinterpret_cast<unsigned long*>(
                            reinterpret_cast<char*>(finish) + delta
                        );
                    } while (moveCount != 0);
                }

                unsigned long* fillEnd = position + count;
                for (; position != fillEnd; ++position) {
                    *position = fillValue;
                }
            } else {
                const unsigned int extraCount = count - tailCount;
                count = tailCount;

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