#include <cstddef>
#include <cstdint>

namespace
{
#pragma pack(push, 1)
struct VectorOverlay
{
    std::uint8_t pad_00[0x04];
    std::uint32_t finish;       // +0x04
    std::uint32_t end_storage;  // +0x08
};
#pragma pack(pop)

static_assert(offsetof(VectorOverlay, finish) == 0x04);
static_assert(offsetof(VectorOverlay, end_storage) == 0x08);

extern void __thiscall Vector_Reallocate(
    unsigned long* position,
    unsigned long* value,
    void* count_end_byte,
    unsigned int original_count,
    int zero);

namespace std
{
template <class T, class Size, class U>
T _Fill_n(T first, Size count, const U* value);
}
}

void __thiscall Vector_Insert(void* self, unsigned long* position, unsigned int count, unsigned long* value)
{
    auto* const vec = static_cast<VectorOverlay*>(self);
    unsigned int original_count = count;

    if (count != 0) {
        if (((vec->end_storage - vec->finish) / 0x0Cu) < count) {
            count &= 0x00FFFFFFu;
            Vector_Reallocate(
                position,
                value,
                reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(&count) + 3),
                original_count,
                0);
        }
        else {
            unsigned long local_10 = value[0];
            unsigned long uStack_c = value[1];
            unsigned long uStack_8 = value[2];

            unsigned int tail_count =
                (vec->finish - static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(position))) / 0x0Cu;
            auto* finish = reinterpret_cast<unsigned long*>(static_cast<std::uintptr_t>(vec->finish));

            if (count < tail_count) {
                auto* tail_src = finish - count * 3;
                auto* dst = finish;

                for (auto* src = tail_src; src != finish; src += 3) {
                    if (dst != nullptr) {
                        dst[0] = src[0];
                        dst[1] = src[1];
                        dst[2] = src[2];
                    }
                    dst += 3;
                }

                int prefix_count = static_cast<int>(
                    (reinterpret_cast<std::uintptr_t>(tail_src) - reinterpret_cast<std::uintptr_t>(position)) / 0x0Cu);

                vec->finish = vec->finish + count * 0x0Cu;

                if (0 < prefix_count) {
                    do {
                        prefix_count = prefix_count + -1;
                        finish[-3] = tail_src[-3];
                        finish[-2] = tail_src[-2];
                        finish[-1] = tail_src[-1];
                        tail_src = tail_src - 3;
                        finish = finish - 3;
                    } while (prefix_count != 0);
                }

                auto* fill_end = position + count * 3;
                for (; position != fill_end; position += 3) {
                    position[0] = local_10;
                    position[1] = uStack_c;
                    position[2] = uStack_8;
                }
            }
            else {
                unsigned int extra_count = count - tail_count;
                count = tail_count;
                auto* old_finish = finish;

                std::_Fill_n<unsigned long*, unsigned int, unsigned long>(finish, extra_count, &local_10);
                vec->finish = vec->finish + extra_count * 0x0Cu;

                auto* dst = reinterpret_cast<unsigned long*>(static_cast<std::uintptr_t>(vec->finish));
                for (auto* src = position; src != finish; src += 3) {
                    if (dst != nullptr) {
                        dst[0] = src[0];
                        dst[1] = src[1];
                        dst[2] = src[2];
                        finish = old_finish;
                    }
                    dst += 3;
                }

                vec->finish = vec->finish + count * 0x0Cu;

                if (position != old_finish) {
                    do {
                        position[0] = local_10;
                        auto* last_word = position + 2;
                        position[1] = uStack_c;
                        position = position + 3;
                        *last_word = uStack_8;
                    } while (position != old_finish);
                }
            }
        }
    }
}