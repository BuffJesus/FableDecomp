#include <cstddef>
#include <cstdint>

struct CList_Entry
{
    std::uint32_t value0;
    std::uint32_t value1;
};
static_assert(sizeof(CList_Entry) == 0x8);

struct CList_Overlay
{
    std::uint8_t pad_0x00[0x04];
    CList_Entry* write;
    CList_Entry* end;
};
static_assert(offsetof(CList_Overlay, write) == 0x04);
static_assert(offsetof(CList_Overlay, end) == 0x08);

extern "C" void __cdecl Vector_ReallocateAndFill(
    CList_Entry* currentWrite,
    CList_Entry* value,
    CList_Entry** valuePtr,
    int count,
    int growCount);

void __thiscall CList_PushBack(void* self, CList_Entry* value)
{
    auto* const list = static_cast<CList_Overlay*>(self);
    CList_Entry* write = list->write;

    if (write != list->end)
    {
        if (write != nullptr)
        {
            write->value0 = value->value0;

            auto* const refCount = reinterpret_cast<int*>(value->value1);
            write->value1 = reinterpret_cast<std::uint32_t>(refCount);

            if (refCount != nullptr)
            {
                *refCount = *refCount + 1;
            }
        }

        list->write = reinterpret_cast<CList_Entry*>(
            reinterpret_cast<std::uintptr_t>(list->write) + 8);
        return;
    }

    Vector_ReallocateAndFill(write, value, &value, 1, 1);
}