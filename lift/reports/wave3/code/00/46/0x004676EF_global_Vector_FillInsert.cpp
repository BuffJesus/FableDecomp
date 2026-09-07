#include <cstddef>
#include <cstdint>
#include <cstring>

struct VectorOverlay {
    std::uint8_t* unknown_0x00;
    std::uint8_t* finish; // 0x04
    std::uint8_t* end;    // 0x08
};

static_assert(offsetof(VectorOverlay, finish) == 0x04);
static_assert(offsetof(VectorOverlay, end) == 0x08);
static_assert(sizeof(VectorOverlay) == 0x0C);

extern void VectorInsert_ExpandWithFill(void* insertPos, const void* value, const void* valueEnd, std::uint32_t count, int zero);
extern void Vector_CopyRange(void* dst, const void* srcEnd);

void __thiscall Vector_FillInsert(VectorOverlay* self, std::uint8_t* position, std::uint32_t count, const std::uint8_t* value)
{
    std::uint32_t originalCount = count;

    if (count != 0) {
        if (static_cast<std::uint32_t>(self->end - self->finish) < count) {
            std::uint32_t count24 = count & 0x00FFFFFFu;
            VectorInsert_ExpandWithFill(position, value, reinterpret_cast<const std::uint8_t*>(&count24) + 3, originalCount, 0);
        } else {
            std::uint32_t tailCount = static_cast<std::uint32_t>(self->finish - position);
            const std::uint8_t fillByte = *value;
            std::uint8_t* oldFinish = self->finish;

            if (count < tailCount) {
                std::uint32_t count24 = count & 0x00FFFFFFu;
                Vector_CopyRange(oldFinish, reinterpret_cast<const std::uint8_t*>(&count24) + 3);

                self->finish = self->finish + originalCount;

                const std::intptr_t moveSize =
                    reinterpret_cast<std::intptr_t>(oldFinish) +
                    (-static_cast<std::intptr_t>(originalCount) - reinterpret_cast<std::intptr_t>(position));

                if (0 < moveSize) {
                    std::memmove(oldFinish - moveSize, position, static_cast<std::size_t>(moveSize));
                }

                oldFinish = position + originalCount;
            } else {
                std::uint32_t fillCount = count - tailCount;

                if (fillCount != 0) {
                    std::uint32_t* dst32 = reinterpret_cast<std::uint32_t*>(oldFinish);
                    const std::uint32_t repeated =
                        static_cast<std::uint32_t>(fillByte) |
                        (static_cast<std::uint32_t>(fillByte) << 8) |
                        (static_cast<std::uint32_t>(fillByte) << 16) |
                        (static_cast<std::uint32_t>(fillByte) << 24);

                    for (std::uint32_t dwords = fillCount >> 2; dwords != 0; --dwords) {
                        *dst32 = repeated;
                        ++dst32;
                    }

                    for (std::uint32_t bytes = fillCount & 3; bytes != 0; --bytes) {
                        *reinterpret_cast<std::uint8_t*>(dst32) = fillByte;
                        dst32 = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(dst32) + 1);
                    }
                }

                self->finish = self->finish + (count - tailCount);

                std::uint32_t count24 = count & 0x00FFFFFFu;
                Vector_CopyRange(self->finish, reinterpret_cast<const std::uint8_t*>(&count24) + 3);

                self->finish = self->finish + tailCount;
            }

            if (position != oldFinish) {
                std::uint32_t* dst32 = reinterpret_cast<std::uint32_t*>(position);
                const std::uint32_t repeated =
                    static_cast<std::uint32_t>(fillByte) |
                    (static_cast<std::uint32_t>(fillByte) << 8) |
                    (static_cast<std::uint32_t>(fillByte) << 16) |
                    (static_cast<std::uint32_t>(fillByte) << 24);

                for (std::uint32_t dwords = static_cast<std::uint32_t>(oldFinish - position) >> 2; dwords != 0; --dwords) {
                    *dst32 = repeated;
                    ++dst32;
                }

                for (std::uint32_t bytes = static_cast<std::uint32_t>(oldFinish - position) & 3; bytes != 0; --bytes) {
                    *reinterpret_cast<std::uint8_t*>(dst32) = fillByte;
                    dst32 = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(dst32) + 1);
                }
            }
        }
    }
}