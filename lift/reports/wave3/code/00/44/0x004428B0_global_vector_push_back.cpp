#include <cstddef>
#include <cstdint>

struct VectorPushBackElement {
    std::uint32_t dword0;
    std::uint32_t dword1;
};

struct VectorPushBackOverlay {
    std::byte pad_0[0x4];
    std::uint32_t* end;
    std::uint32_t* capacity;
};

static_assert(offsetof(VectorPushBackOverlay, end) == 0x4);
static_assert(offsetof(VectorPushBackOverlay, capacity) == 0x8);
static_assert(sizeof(VectorPushBackElement) == 0x8);

extern void __cdecl VectorReallocateDouble(
    std::uint32_t* currentEnd,
    const VectorPushBackElement* value,
    const VectorPushBackElement** valuePtrAddr,
    int a4,
    int a5
);

void __thiscall vector_push_back(VectorPushBackOverlay* self, const VectorPushBackElement* value)
{
    std::uint32_t* currentEnd = self->end;

    if (currentEnd != self->capacity) {
        if (currentEnd != nullptr) {
            currentEnd[0] = value->dword0;
            currentEnd[1] = value->dword1;
        }

        self->end = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(self->end) + 8
        );
        return;
    }

    VectorReallocateDouble(currentEnd, value, &value, 1, 1);
}