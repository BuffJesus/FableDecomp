#include <cstddef>
#include <cstdint>

struct CNavConnectorNode;

template <typename T>
struct CCountedPointer;

template <>
struct CCountedPointer<CNavConnectorNode>
{
    std::uint32_t m_opaque0;
    int* m_refCount;
};

static_assert(sizeof(CCountedPointer<CNavConnectorNode>) == 0x8);
static_assert(offsetof(CCountedPointer<CNavConnectorNode>, m_opaque0) == 0x0);
static_assert(offsetof(CCountedPointer<CNavConnectorNode>, m_refCount) == 0x4);

struct NavConnectorHeapElement
{
    long m_key;
    CCountedPointer<CNavConnectorNode> m_value;
};

static_assert(sizeof(NavConnectorHeapElement) == 0xC);
static_assert(offsetof(NavConnectorHeapElement, m_key) == 0x0);
static_assert(offsetof(NavConnectorHeapElement, m_value) == 0x4);

extern "C" void __fastcall std::_Adjust_heap(
    int first,
    int holeIndex,
    int length,
    long key,
    std::uint32_t countedPtrWord0,
    int* countedPtrRefCount,
    std::uint32_t compareLess);

void __fastcall Heap_MakeHeap_NavConnector(int first, int last, std::uint32_t compareLess)
{
    const int length = (last - first) / 0xC;
    if (1 < length) {
        int parent = (length - 2) / 2;
        std::uint32_t* puVar6 = reinterpret_cast<std::uint32_t*>(first + 4 + parent * 0xC);

        for (;;) {
            const long key = static_cast<long>(puVar6[-1]);
            const std::uint32_t countedPtrWord0 = puVar6[0];
            int* const countedPtrRefCount = reinterpret_cast<int*>(puVar6[1]);

            if (countedPtrRefCount != nullptr) {
                *countedPtrRefCount = *countedPtrRefCount + 1;
            }

            std::_Adjust_heap(
                first,
                parent,
                length,
                key,
                countedPtrWord0,
                countedPtrRefCount,
                compareLess);

            if (parent == 0) {
                break;
            }

            parent = parent - 1;
            puVar6 -= 3;
        }
    }
}