#include <cstddef>
#include <cstdint>
#include <new>

namespace NInventory
{
    struct CItem
    {
        std::int32_t field_0;
        std::uint32_t field_4;
        void* field_8;

        CItem& operator=(const CItem& rhs);
    };

    static_assert(sizeof(CItem) == 0x0C);
    static_assert(offsetof(CItem, field_0) == 0x00);
    static_assert(offsetof(CItem, field_4) == 0x04);
    static_assert(offsetof(CItem, field_8) == 0x08);
}

struct RefCountReleaseOverlay
{
    std::int32_t ref_count;
    std::uint32_t release_fn;
    void* release_ctx;
};

static_assert(sizeof(RefCountReleaseOverlay) == 0x0C);
static_assert(offsetof(RefCountReleaseOverlay, ref_count) == 0x00);
static_assert(offsetof(RefCountReleaseOverlay, release_fn) == 0x04);
static_assert(offsetof(RefCountReleaseOverlay, release_ctx) == 0x08);

namespace std
{
    template <class T, class U>
    void _Move_backward(T, T, T, U, int);
}

// Observed call shape:
//   ECX = param_2
//   stack arg #1 = CItem payload by value/layout
//   stack arg #2 = trailing dword
extern void __thiscall Vector_DestroyRange(
    NInventory::CItem*,
    NInventory::CItem,
    std::uint32_t);

void __fastcall Vector_InsertWithMove(
    NInventory::CItem* param_1,
    NInventory::CItem* param_2,
    std::int32_t param_3,
    std::uint32_t param_4,
    RefCountReleaseOverlay* param_5,
    std::uint32_t param_6)
{
    RefCountReleaseOverlay* piVar1 = param_5;

    if (param_3 < param_1->field_0)
    {
        std::_Move_backward<NInventory::CItem*, NInventory::CItem*>(
            param_1,
            param_2,
            reinterpret_cast<NInventory::CItem*>(
                reinterpret_cast<std::uint8_t*>(param_2) + 0x0C),
            &param_6,
            0);

        param_1->operator=(*reinterpret_cast<const NInventory::CItem*>(&param_3));
        piVar1 = param_5;
    }
    else
    {
        NInventory::CItem stack_item{
            param_3,
            param_4,
            param_5,
        };

        if (param_5 != nullptr)
        {
            param_5->ref_count = param_5->ref_count + 1;
        }

        Vector_DestroyRange(param_2, stack_item, param_6);
    }

    if ((piVar1 != nullptr) &&
        ((piVar1->ref_count = piVar1->ref_count + -1), piVar1->ref_count == 0))
    {
        reinterpret_cast<void(__thiscall*)(void*)>(piVar1->release_fn)(piVar1->release_ctx);
        ::operator delete(piVar1);
    }
}