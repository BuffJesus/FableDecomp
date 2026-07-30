#include <cstddef>
#include <cstdint>

struct RefCountedBlock {
    std::int32_t ref_count;                  // 0x00
    void (__thiscall* destroy_fn)(void*);   // 0x04
    void* destroy_this;                     // 0x08
};

static_assert(offsetof(RefCountedBlock, ref_count) == 0x00);
static_assert(offsetof(RefCountedBlock, destroy_fn) == 0x04);
static_assert(offsetof(RefCountedBlock, destroy_this) == 0x08);
static_assert(sizeof(RefCountedBlock) == 0x0C);

struct VectorDestroyEntry {
    std::int32_t value0;          // 0x00
    std::int32_t value1;          // 0x04
    RefCountedBlock* ref_block;   // 0x08
};

static_assert(offsetof(VectorDestroyEntry, value0) == 0x00);
static_assert(offsetof(VectorDestroyEntry, value1) == 0x04);
static_assert(offsetof(VectorDestroyEntry, ref_block) == 0x08);
static_assert(sizeof(VectorDestroyEntry) == 0x0C);

void Vector_DestroyRange(
    VectorDestroyEntry* cursor,
    std::int32_t target_value0,
    std::int32_t replacement_value1,
    RefCountedBlock* replacement_ref_block)
{
    std::int32_t current_value0 = (cursor - 1)->value0;

    while (target_value0 < current_value0) {
        cursor->value0 = current_value0;

        RefCountedBlock* const previous_ref_block = (cursor - 1)->ref_block;
        RefCountedBlock* const current_ref_block = cursor->ref_block;
        current_value0 = (cursor - 1)->value1;

        if (current_ref_block != previous_ref_block) {
            if (current_ref_block != nullptr) {
                current_ref_block->ref_count = current_ref_block->ref_count + -1;
                if (current_ref_block->ref_count == 0) {
                    current_ref_block->destroy_fn(current_ref_block->destroy_this);
                    ::operator delete(current_ref_block);
                }
            }

            cursor->value1 = current_value0;
            cursor->ref_block = previous_ref_block;

            if (previous_ref_block != nullptr) {
                previous_ref_block->ref_count = previous_ref_block->ref_count + 1;
            }
        }

        current_value0 = (cursor - 2)->value0;
        cursor = cursor - 1;
    }

    cursor->value0 = target_value0;

    RefCountedBlock* const current_ref_block = cursor->ref_block;
    if (current_ref_block != replacement_ref_block) {
        if (current_ref_block != nullptr) {
            current_ref_block->ref_count = current_ref_block->ref_count + -1;
            if (current_ref_block->ref_count == 0) {
                current_ref_block->destroy_fn(current_ref_block->destroy_this);
                ::operator delete(current_ref_block);
            }
        }

        cursor->value1 = replacement_value1;
        cursor->ref_block = replacement_ref_block;

        if (replacement_ref_block == nullptr) {
            return;
        }

        replacement_ref_block->ref_count = replacement_ref_block->ref_count + 1;
    }

    if ((replacement_ref_block != nullptr) &&
        ((replacement_ref_block->ref_count = replacement_ref_block->ref_count + -1),
         replacement_ref_block->ref_count == 0)) {
        replacement_ref_block->destroy_fn(replacement_ref_block->destroy_this);
        ::operator delete(replacement_ref_block);
    }
}