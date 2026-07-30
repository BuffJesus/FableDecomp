struct CContainmentVolume;

struct CCountedPointer_CContainmentVolume_
{
    std::uint32_t sortableValue;
    std::uint8_t padding[8];
};
static_assert(sizeof(CCountedPointer_CContainmentVolume_) == 0x0C);
static_assert(offsetof(CCountedPointer_CContainmentVolume_, sortableValue) == 0x00);

struct ContainmentVolumeRefCountOverlay
{
    std::int32_t refCount;
    std::uintptr_t destroyThunk;
    void* object;
};
static_assert(sizeof(ContainmentVolumeRefCountOverlay) == 0x0C);
static_assert(offsetof(ContainmentVolumeRefCountOverlay, refCount) == 0x00);
static_assert(offsetof(ContainmentVolumeRefCountOverlay, destroyThunk) == 0x04);
static_assert(offsetof(ContainmentVolumeRefCountOverlay, object) == 0x08);

CCountedPointer_CContainmentVolume_* __fastcall Heap_SiftDown_ContainmentVolume(
    CCountedPointer_CContainmentVolume_* first,
    CCountedPointer_CContainmentVolume_* last,
    int value,
    std::uint32_t /*unused*/,
    ContainmentVolumeRefCountOverlay* refCount)
{
    for (;;)
    {
        int current = first->sortableValue;
        while (current < value)
        {
            CCountedPointer_CContainmentVolume_* const next = first + 1;
            first = next;
            current = next->sortableValue;
        }

        current = (last - 1)->sortableValue;
        CCountedPointer_CContainmentVolume_* scan = last;
        while (value < current)
        {
            current = (scan - 2)->sortableValue;
            scan = scan - 1;
        }
        last = scan - 1;

        if (last <= first)
            break;

        std::swap(*first, *last);
        ++first;
    }

    if (refCount != nullptr && --refCount->refCount == 0)
    {
        reinterpret_cast<void(__thiscall*)(void*)>(refCount->destroyThunk)(refCount->object);
        operator delete(refCount);
    }

    return first;
}