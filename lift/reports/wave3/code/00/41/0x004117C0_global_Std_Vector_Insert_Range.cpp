#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CameraTagPair_0x1C {
    std::uint32_t dword0;
    std::uint32_t dword1;
    std::uint32_t dword2;
    std::uint32_t dword3;
    std::uint32_t dword4;
    std::uint32_t dword5;
    std::uint32_t dword6;
};
static_assert(sizeof(CameraTagPair_0x1C) == 0x1C);

struct StdVector_CameraTagPair_Overlay {
    CameraTagPair_0x1C* first;
    CameraTagPair_0x1C* last;
    CameraTagPair_0x1C* end;
};
static_assert(offsetof(StdVector_CameraTagPair_Overlay, first) == 0x0);
static_assert(offsetof(StdVector_CameraTagPair_Overlay, last) == 0x4);
static_assert(offsetof(StdVector_CameraTagPair_Overlay, end) == 0x8);
static_assert(sizeof(StdVector_CameraTagPair_Overlay) == 0xC);

extern CameraTagPair_0x1C* __cdecl Std_Copy_Impl_CameraTagPair(
    CameraTagPair_0x1C* first,
    CameraTagPair_0x1C* last,
    void* dest,
    char* copyState);

extern CameraTagPair_0x1C* __cdecl Std_Fill_CameraTagPair(
    CameraTagPair_0x1C* dest,
    std::uint32_t count,
    const CameraTagPair_0x1C* value);

void __thiscall Std_Vector_Insert_Range(
    StdVector_CameraTagPair_Overlay* self,
    CameraTagPair_0x1C* insertPos,
    const CameraTagPair_0x1C* value,
    std::uint32_t /*unused*/,
    std::uint32_t insertCount,
    char copyState)
{
    const std::uint32_t oldSize =
        static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(self->last) -
                                    reinterpret_cast<std::uintptr_t>(self->first)) /
                                   0x1C);

    const std::uint32_t newCapacity =
        oldSize + ((insertCount <= oldSize) ? oldSize : insertCount);

    void* newStorage;
    if (newCapacity == 0) {
        newStorage = nullptr;
    } else {
        newStorage = std::malloc(static_cast<std::size_t>(newCapacity) * 0x1C);
    }

    CameraTagPair_0x1C* write =
        Std_Copy_Impl_CameraTagPair(self->first, insertPos, newStorage, &copyState);

    if (insertCount == 1) {
        if (write != nullptr) {
            write->dword0 = value->dword0;
            write->dword1 = value->dword1;
            write->dword2 = value->dword2;
            write->dword3 = value->dword3;
            write->dword4 = value->dword4;
            write->dword5 = value->dword5;
            write->dword6 = value->dword6;
        }
        write = reinterpret_cast<CameraTagPair_0x1C*>(
            reinterpret_cast<std::uintptr_t>(write) + 0x1C);
    } else {
        write = Std_Fill_CameraTagPair(write, insertCount, value);
    }

    if (copyState == '\0') {
        write = Std_Copy_Impl_CameraTagPair(insertPos, self->last, write, &copyState);
    }

    if (self->first != nullptr) {
        std::free(self->first);
    }

    self->first = static_cast<CameraTagPair_0x1C*>(newStorage);
    self->last = write;
    self->end = reinterpret_cast<CameraTagPair_0x1C*>(
        reinterpret_cast<std::uintptr_t>(newStorage) + static_cast<std::uintptr_t>(newCapacity) * 0x1C);
}