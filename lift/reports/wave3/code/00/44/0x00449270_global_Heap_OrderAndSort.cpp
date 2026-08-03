#include <cstddef>
#include <cstdint>

struct HeapTripleOverlay {
    std::int32_t key;          // +0x00
    std::int32_t value;        // +0x04
    std::int32_t* refCount;    // +0x08
};

static_assert(offsetof(HeapTripleOverlay, key) == 0x00);
static_assert(offsetof(HeapTripleOverlay, value) == 0x04);
static_assert(offsetof(HeapTripleOverlay, refCount) == 0x08);
static_assert(sizeof(HeapTripleOverlay) == 0x0C);

extern void __cdecl Heap_MakeHeap_NavConnector(std::uint32_t compare, int zero0, int zero1);
extern void __fastcall _Pop_heap(
    std::int32_t* heapRoot,
    std::int32_t* first,
    HeapTripleOverlay* hole,
    std::int32_t key,
    std::int32_t value,
    std::int32_t* refCount,
    std::uint32_t compare,
    int zero);
extern void __fastcall _Sort_heap(
    std::int32_t* heapRoot,
    std::int32_t* first,
    std::uint32_t compare);

void __fastcall Heap_OrderAndSort(
    std::int32_t* param_1,
    std::int32_t* param_2,
    std::int32_t* param_3,
    std::uint32_t /*param_4*/,
    std::uint32_t param_5)
{
    Heap_MakeHeap_NavConnector(param_5, 0, 0);

    for (auto* piVar4 = reinterpret_cast<HeapTripleOverlay*>(param_2);
         reinterpret_cast<std::int32_t*>(piVar4) < param_3;
         piVar4 = reinterpret_cast<HeapTripleOverlay*>(reinterpret_cast<std::int32_t*>(piVar4) + 3)) {
        const std::int32_t iVar1 = piVar4->key;
        if (iVar1 < *param_1) {
            const std::int32_t iVar2 = piVar4->value;
            std::int32_t* const piVar3 = piVar4->refCount;
            if (piVar3 != nullptr) {
                *piVar3 = *piVar3 + 1;
            }

            _Pop_heap(param_1, param_2, piVar4, iVar1, iVar2, piVar3, param_5, 0);
        }
    }

    _Sort_heap(param_1, param_2, param_5);
}