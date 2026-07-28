#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct C2DLineF {
    std::byte _pad[0x10];
};
static_assert(sizeof(C2DLineF) == 0x10);

struct StdVectorC2DLineF {
    C2DLineF* m_First;    // 0x00
    C2DLineF* m_Last;     // 0x04
    C2DLineF* m_End;      // 0x08
};

static_assert(offsetof(StdVectorC2DLineF, m_First) == 0x00);
static_assert(offsetof(StdVectorC2DLineF, m_Last) == 0x04);
static_assert(offsetof(StdVectorC2DLineF, m_End) == 0x08);
static_assert(sizeof(StdVectorC2DLineF) == 0x0C);

StdVectorC2DLineF* __thiscall std_vector_C2DLineF_Copy(
    StdVectorC2DLineF* self,
    const StdVectorC2DLineF* other)
{
    C2DLineF* newFirst = nullptr;
    const std::int32_t count =
        static_cast<std::int32_t>(
            (reinterpret_cast<std::uintptr_t>(other->m_Last) -
             reinterpret_cast<std::uintptr_t>(other->m_First)) >> 4);

    self->m_First = nullptr;
    self->m_Last = nullptr;
    self->m_End = nullptr;

    if (count != 0) {
        newFirst = static_cast<C2DLineF*>(std::malloc(static_cast<std::size_t>(count) << 4));
    }

    self->m_First = newFirst;
    self->m_Last = newFirst;
    self->m_End = reinterpret_cast<C2DLineF*>(
        reinterpret_cast<std::uintptr_t>(newFirst) +
        (static_cast<std::uintptr_t>(count) << 4));

    newFirst = std::_Copy_impl<C2DLineF*, C2DLineF*>(other->m_First, other->m_Last, newFirst);
    self->m_Last = newFirst;
    return self;
}