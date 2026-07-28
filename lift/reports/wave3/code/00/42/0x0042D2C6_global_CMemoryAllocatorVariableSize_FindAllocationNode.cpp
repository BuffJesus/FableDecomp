#include <cstddef>
#include <cstdint>

struct FindAllocationNodeTreeHeaderOverlay {
    std::byte pad_00[0x08];
    unsigned long field_08;
    unsigned long field_0C;
};
static_assert(offsetof(FindAllocationNodeTreeHeaderOverlay, field_08) == 0x08);
static_assert(offsetof(FindAllocationNodeTreeHeaderOverlay, field_0C) == 0x0C);

struct FindAllocationNodeNodeOverlay {
    std::byte pad_00[0x0C];
    unsigned long field_0C;
    unsigned long field_10;
};
static_assert(offsetof(FindAllocationNodeNodeOverlay, field_0C) == 0x0C);
static_assert(offsetof(FindAllocationNodeNodeOverlay, field_10) == 0x10);

struct FindAllocationNodeKeyOverlay {
    unsigned long field_00;
};
static_assert(offsetof(FindAllocationNodeKeyOverlay, field_00) == 0x00);

extern unsigned long RBTree__GetMaxNode(unsigned long node);
extern void BinaryTree_InsertScriptData(
    unsigned long* outNode,
    unsigned long childOrNull,
    unsigned long parentOrAnchor,
    CMemoryAllocatorVariableSize* key,
    unsigned long sideFlagOrNull);
extern unsigned long* CMemoryAllocatorVariableSize_FindOrInsert(
    unsigned long** treeState,
    CMemoryAllocatorVariableSize* key);

unsigned long* __thiscall CMemoryAllocatorVariableSize_FindAllocationNode(
    unsigned long* param_1,
    unsigned long* param_2,
    unsigned long param_3,
    CMemoryAllocatorVariableSize* param_4)
{
    const auto* const key = reinterpret_cast<const FindAllocationNodeKeyOverlay*>(param_4);
    const unsigned long tree_header_addr = *param_1;
    unsigned long* local_c = param_1;

    auto* const tree_header =
        reinterpret_cast<FindAllocationNodeTreeHeaderOverlay*>(tree_header_addr);

    unsigned long uVar3 = tree_header_addr;
    unsigned long uVar4;

    if (param_3 == tree_header->field_08) {
        if (param_1[1] == 0) {
            goto LAB_0042d3ec;
        }

        auto* const current_node =
            reinterpret_cast<FindAllocationNodeNodeOverlay*>(param_3);
        auto* const this_ptr = reinterpret_cast<CMemoryAllocatorVariableSize*>(current_node->field_10);
        const unsigned long this_as_ulong =
            static_cast<unsigned long>(reinterpret_cast<std::uintptr_t>(this_ptr));

        if (key->field_00 < this_as_ulong) {
            goto LAB_0042d2f3;
        }

        if (key->field_00 <= this_as_ulong) {
            goto LAB_0042d309;
        }

        uVar3 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_ptr);
        uVar4 = param_3;

        if (uVar3 != *param_1) {
            const auto* const no_allocated_areas_node =
                reinterpret_cast<const FindAllocationNodeNodeOverlay*>(uVar3);

            if (no_allocated_areas_node->field_10 <= key->field_00) {
                goto LAB_0042d3ec;
            }

            const int iVar1 = static_cast<int>(current_node->field_0C);
            if (iVar1 == 0) {
                uVar3 = 0;
                uVar4 = param_3;
            } else {
                uVar4 = uVar3;
                param_3 = 0;
            }
            goto LAB_0042d2f5;
        }
    } else {
        if (param_3 != tree_header_addr) {
            uVar4 = RBTree__GetMaxNode(param_3);

            const auto* const current_node =
                reinterpret_cast<const FindAllocationNodeNodeOverlay*>(param_3);
            const auto* const max_node =
                reinterpret_cast<const FindAllocationNodeNodeOverlay*>(uVar4);

            const unsigned int uVar2 = static_cast<unsigned int>(key->field_00);
            const bool bVar6 =
                uVar2 < static_cast<unsigned int>(current_node->field_10);

            if ((!bVar6) || (uVar2 <= static_cast<unsigned int>(max_node->field_10))) {
                uVar3 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(param_4);
                bool bVar7 = !bVar6;

                if (!bVar6) {
                    bVar7 =
                        static_cast<unsigned int>(current_node->field_10) < uVar2;
                    if (!bVar7) {
                        goto LAB_0042d309;
                    }

                    if ((uVar3 == *param_1) ||
                        (uVar2 <
                         static_cast<unsigned int>(
                             reinterpret_cast<const FindAllocationNodeNodeOverlay*>(uVar3)->field_10))) {
                        const int iVar1 = static_cast<int>(current_node->field_0C);
                        if (iVar1 == 0) {
                            uVar3 = 0;
                            uVar4 = param_3;
                        } else {
                            uVar4 = uVar3;
                            param_3 = 0;
                        }
                        goto LAB_0042d2f5;
                    }
                }

                if (bVar6 == bVar7) {
                    goto LAB_0042d309;
                }

                goto LAB_0042d3ec;
            }

            if (static_cast<int>(max_node->field_0C) == 0) {
                uVar3 = 0;
                param_3 = uVar4;
                goto LAB_0042d2f5;
            }

LAB_0042d2f3:
            uVar3 = param_3;
            uVar4 = param_3;
            param_3 = 0;
            goto LAB_0042d2f5;
        }

        uVar4 = tree_header->field_0C;
        const auto* const tail_node =
            reinterpret_cast<const FindAllocationNodeNodeOverlay*>(uVar4);

        if (static_cast<unsigned int>(key->field_00) <=
            static_cast<unsigned int>(tail_node->field_10)) {
            goto LAB_0042d3ec;
        }
    }

    uVar3 = 0;

LAB_0042d2f5:
    BinaryTree_InsertScriptData(param_2, uVar3, uVar4, param_4, param_3);
    return param_2;

LAB_0042d309:
    *param_2 = param_3;
    return param_2;

LAB_0042d3ec:
    {
        unsigned long* const puVar5 =
            CMemoryAllocatorVariableSize_FindOrInsert(&local_c, param_4);
        *param_2 = *puVar5;
        return param_2;
    }
}