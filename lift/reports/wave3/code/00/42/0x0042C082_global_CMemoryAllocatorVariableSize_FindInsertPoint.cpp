#include <cstddef>
#include <cstdint>

class CMemoryAllocatorVariableSize;

struct RBTreeNodeOverlay
{
    std::uint32_t field_0;
    std::uint32_t field_4;
    std::uint32_t field_8;
    std::uint32_t field_C;
    std::byte payload[4];
};

static_assert(offsetof(RBTreeNodeOverlay, field_8) == 0x08);
static_assert(offsetof(RBTreeNodeOverlay, field_C) == 0x0C);
static_assert(offsetof(RBTreeNodeOverlay, payload) == 0x10);

struct InsertContextOverlay
{
    std::uint32_t* first;
    std::uint32_t* second;
};

static_assert(offsetof(InsertContextOverlay, first) == 0x00);
static_assert(offsetof(InsertContextOverlay, second) == 0x04);

class CMemoryAllocatorVariableSize
{
public:
    static std::uint32_t __thiscall GetNoAllocatedAreas(CMemoryAllocatorVariableSize* self);
};

extern std::uint32_t __cdecl RBTree__GetMaxNode(std::uint32_t node);
extern std::uint32_t* __cdecl RBTree_InsertOrReplace(
    InsertContextOverlay* context,
    CMemoryAllocatorVariableSize* value);
extern void __cdecl BinarySearchTree_InsertAndBalance(
    std::uint32_t* result,
    std::uint32_t child,
    std::uint32_t parent,
    CMemoryAllocatorVariableSize* value,
    std::uint32_t link);

static inline std::uint32_t NodeField8(std::uint32_t node)
{
    return reinterpret_cast<const RBTreeNodeOverlay*>(static_cast<std::uintptr_t>(node))->field_8;
}

static inline std::uint32_t NodeFieldC(std::uint32_t node)
{
    return reinterpret_cast<const RBTreeNodeOverlay*>(static_cast<std::uintptr_t>(node))->field_C;
}

static inline std::uint32_t NodePayloadUInt(std::uint32_t node)
{
    return *reinterpret_cast<const std::uint32_t*>(static_cast<std::uintptr_t>(node) + 0x10);
}

static inline CMemoryAllocatorVariableSize* NodePayloadPointer(std::uint32_t node)
{
    return *reinterpret_cast<CMemoryAllocatorVariableSize* const*>(
        static_cast<std::uintptr_t>(node) + 0x10);
}

static inline std::uint32_t AllocatorField0UInt(const CMemoryAllocatorVariableSize* value)
{
    return *reinterpret_cast<const std::uint32_t*>(value);
}

static inline CMemoryAllocatorVariableSize* AllocatorField0Pointer(
    const CMemoryAllocatorVariableSize* value)
{
    return *reinterpret_cast<CMemoryAllocatorVariableSize* const*>(value);
}

std::uint32_t* __thiscall CMemoryAllocatorVariableSize_FindInsertPoint(
    std::uint32_t* param_1,
    std::uint32_t* param_2,
    std::uint32_t param_3,
    CMemoryAllocatorVariableSize* param_4)
{
    std::uint32_t uVar3;
    std::uint32_t uVar4;
    std::uint32_t* puVar5;
    std::uint32_t uVar2;
    int iVar1;
    bool bVar6;
    bool bVar7;
    InsertContextOverlay local_c;
    CMemoryAllocatorVariableSize* this_value;

    uVar3 = *param_1;
    local_c.first = param_1;
    local_c.second = param_1;

    if (param_3 == NodeField8(uVar3)) {
        if (param_1[1] == 0) {
            goto LAB_0042c1a8;
        }

        this_value = NodePayloadPointer(param_3);
        if (AllocatorField0Pointer(param_4) < this_value) {
            goto LAB_0042c0af;
        }

        if (AllocatorField0Pointer(param_4) <= this_value) {
            goto LAB_0042c0c5;
        }

        uVar3 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_value);
        uVar4 = param_3;
        if (uVar3 != *param_1) {
            if (NodePayloadUInt(uVar3) <= AllocatorField0UInt(param_4)) {
                goto LAB_0042c1a8;
            }

            iVar1 = static_cast<int>(NodeFieldC(param_3));
joined_r0x0042c184:
            if (iVar1 == 0) {
                uVar3 = 0;
                uVar4 = param_3;
            } else {
                uVar4 = uVar3;
                param_3 = 0;
            }
            goto LAB_0042c0b1;
        }
    } else {
        if (param_3 != uVar3) {
            uVar4 = RBTree__GetMaxNode(param_3);
            uVar2 = AllocatorField0UInt(param_4);
            bVar6 = uVar2 < NodePayloadUInt(param_3);
            if ((!bVar6) || (uVar2 <= NodePayloadUInt(uVar4))) {
                uVar3 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(param_4);
                bVar7 = !bVar6;
                if (!bVar6) {
                    bVar7 = NodePayloadUInt(param_3) < uVar2;
                    if (!bVar7) {
                        goto LAB_0042c0c5;
                    }

                    if ((uVar3 == *param_1) || (uVar2 < NodePayloadUInt(uVar3))) {
                        iVar1 = static_cast<int>(NodeFieldC(param_3));
                        goto joined_r0x0042c184;
                    }
                }

                if (bVar6 == bVar7) {
LAB_0042c0c5:
                    *param_2 = param_3;
                    return param_2;
                }

LAB_0042c1a8:
                puVar5 = RBTree_InsertOrReplace(&local_c, param_4);
                *param_2 = *puVar5;
                return param_2;
            }

            if (static_cast<int>(NodeFieldC(uVar4)) == 0) {
                uVar3 = 0;
                param_3 = uVar4;
                goto LAB_0042c0b1;
            }

LAB_0042c0af:
            uVar3 = param_3;
            uVar4 = param_3;
            param_3 = 0;
            goto LAB_0042c0b1;
        }

        uVar4 = NodeFieldC(uVar3);
        if (AllocatorField0UInt(param_4) <= NodePayloadUInt(uVar4)) {
            goto LAB_0042c1a8;
        }
    }

    uVar3 = 0;

LAB_0042c0b1:
    BinarySearchTree_InsertAndBalance(param_2, uVar3, uVar4, param_4, param_3);
    return param_2;
}