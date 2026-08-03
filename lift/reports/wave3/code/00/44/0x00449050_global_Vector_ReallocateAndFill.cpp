#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CCountedPointer_CActiveEntityScriptBase_Overlay {
    std::int32_t* rep;
};
static_assert(sizeof(CCountedPointer_CActiveEntityScriptBase_Overlay) == 0x4);

struct ActiveEntityScriptPairOverlay {
    std::uint32_t first;
    CCountedPointer_CActiveEntityScriptBase_Overlay second;
};
static_assert(offsetof(ActiveEntityScriptPairOverlay, first) == 0x0);
static_assert(offsetof(ActiveEntityScriptPairOverlay, second) == 0x4);
static_assert(sizeof(ActiveEntityScriptPairOverlay) == 0x8);

struct VectorActiveEntityScriptPairOverlay {
    ActiveEntityScriptPairOverlay* first;
    ActiveEntityScriptPairOverlay* last;
    ActiveEntityScriptPairOverlay* end;
};
static_assert(offsetof(VectorActiveEntityScriptPairOverlay, first) == 0x0);
static_assert(offsetof(VectorActiveEntityScriptPairOverlay, last) == 0x4);
static_assert(offsetof(VectorActiveEntityScriptPairOverlay, end) == 0x8);
static_assert(sizeof(VectorActiveEntityScriptPairOverlay) == 0xC);

static ActiveEntityScriptPairOverlay* UninitCopyActiveEntityScriptPair(
    const ActiveEntityScriptPairOverlay* src_first,
    const ActiveEntityScriptPairOverlay* src_last,
    ActiveEntityScriptPairOverlay* dst,
    char* /*constructed_flag*/)
{
    std::uintptr_t src = reinterpret_cast<std::uintptr_t>(src_first);
    const std::uintptr_t src_end = reinterpret_cast<std::uintptr_t>(src_last);
    std::uintptr_t out = reinterpret_cast<std::uintptr_t>(dst);

    while (src != src_end) {
        const auto* const in_elem =
            reinterpret_cast<const ActiveEntityScriptPairOverlay*>(src);
        auto* const out_elem =
            reinterpret_cast<ActiveEntityScriptPairOverlay*>(out);

        out_elem->first = in_elem->first;

        std::int32_t* const rep = in_elem->second.rep;
        out_elem->second.rep = rep;
        if (rep != nullptr) {
            ++*rep;
        }

        src += 8;
        out += 8;
    }

    return reinterpret_cast<ActiveEntityScriptPairOverlay*>(out);
}

static ActiveEntityScriptPairOverlay* UninitFillNActiveEntityScriptPair(
    ActiveEntityScriptPairOverlay* dst,
    unsigned int count,
    const ActiveEntityScriptPairOverlay* value,
    char* /*constructed_flag*/)
{
    std::uintptr_t out = reinterpret_cast<std::uintptr_t>(dst);

    while (count != 0) {
        auto* const out_elem =
            reinterpret_cast<ActiveEntityScriptPairOverlay*>(out);

        out_elem->first = value->first;

        std::int32_t* const rep = value->second.rep;
        out_elem->second.rep = rep;
        if (rep != nullptr) {
            ++*rep;
        }

        out += 8;
        --count;
    }

    return reinterpret_cast<ActiveEntityScriptPairOverlay*>(out);
}

static void DestroyRangeActiveEntityScriptPairOpaque(
    ActiveEntityScriptPairOverlay* first,
    ActiveEntityScriptPairOverlay* last,
    char* constructed_flag);

void __thiscall Vector_ReallocateAndFill(
    VectorActiveEntityScriptPairOverlay* self,
    ActiveEntityScriptPairOverlay* insert_pos,
    const ActiveEntityScriptPairOverlay* value,
    std::uint32_t /*unused*/,
    unsigned int count,
    char constructed_tail_flag)
{
    const std::uintptr_t first_addr =
        reinterpret_cast<std::uintptr_t>(self->first);
    const std::uintptr_t last_addr =
        reinterpret_cast<std::uintptr_t>(self->last);
    unsigned int old_count =
        static_cast<unsigned int>((last_addr - first_addr) >> 3);

    unsigned int growth = count;
    if (count <= old_count) {
        growth = old_count;
    }

    const unsigned int new_count = growth + old_count;

    void* new_storage;
    if (new_count == 0) {
        new_storage = nullptr;
    } else {
        new_storage = std::malloc(new_count * 8);
    }

    ActiveEntityScriptPairOverlay* new_cur = UninitCopyActiveEntityScriptPair(
        self->first,
        insert_pos,
        static_cast<ActiveEntityScriptPairOverlay*>(new_storage),
        &constructed_tail_flag);

    if (count == 1) {
        if (new_cur != nullptr) {
            new_cur->first = value->first;

            std::int32_t* const rep = value->second.rep;
            new_cur->second.rep = rep;
            if (rep != nullptr) {
                ++*rep;
            }
        }

        new_cur = reinterpret_cast<ActiveEntityScriptPairOverlay*>(
            reinterpret_cast<std::uintptr_t>(new_cur) + 8);
    } else {
        new_cur = UninitFillNActiveEntityScriptPair(
            new_cur,
            count,
            value,
            &constructed_tail_flag);
    }

    if (constructed_tail_flag == '\0') {
        new_cur = UninitCopyActiveEntityScriptPair(
            insert_pos,
            self->last,
            new_cur,
            &constructed_tail_flag);
    }

    DestroyRangeActiveEntityScriptPairOpaque(
        self->first,
        self->last,
        &constructed_tail_flag);

    if (self->first != nullptr) {
        std::free(self->first);
    }

    self->first = static_cast<ActiveEntityScriptPairOverlay*>(new_storage);
    self->last = new_cur;
    self->end = reinterpret_cast<ActiveEntityScriptPairOverlay*>(
        reinterpret_cast<std::uintptr_t>(new_storage) + new_count * 8);
}