#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CEngineInternalPrimitiveBase;
struct CEnginePrimitive;
struct CGuiControlTreePane;

struct AddChildPrimitiveResultOverlay
{
    std::byte pad_00[0x60];
    CGuiControlTreePane* tree_pane;
};

static_assert(offsetof(AddChildPrimitiveResultOverlay, tree_pane) == 0x60);

extern int DAT_01375080;

extern AddChildPrimitiveResultOverlay* __fastcall CEngineInternalPrimitiveBase_AddChildPrimitive(
    CEngineInternalPrimitiveBase* self,
    CEnginePrimitive* unaff_EDI,
    unsigned long unaff_ESI);

extern void __thiscall CGuiControlTreePane_RebuildTreeFromArray(
    CGuiControlTreePane* self,
    int** array_begin_out);

int __fastcall Engine_FindPrimitiveInArray(
    CEngineInternalPrimitiveBase* param_1,
    CEnginePrimitive* unaff_EDI,
    unsigned long unaff_ESI)
{
    int* local_c = nullptr;
    int* local_8 = nullptr;
    std::uint32_t local_4 = 0;

    (void)local_4;

    AddChildPrimitiveResultOverlay* const add_child_result =
        CEngineInternalPrimitiveBase_AddChildPrimitive(param_1, unaff_EDI, unaff_ESI);

    CGuiControlTreePane_RebuildTreeFromArray(add_child_result->tree_pane, &local_c);

    if (DAT_01375080 == -1)
    {
        const int iVar2 =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(local_8) -
                             reinterpret_cast<std::uintptr_t>(local_c));

        if (local_c != nullptr)
        {
            std::free(local_c);
        }

        return (iVar2 >> 2) + -1;
    }

    int iVar2 = -1;
    int* piVar1 = local_c;

    if (local_c != local_8)
    {
        do
        {
            if (DAT_01375080 < *piVar1)
            {
                break;
            }

            piVar1 = piVar1 + 1;
            iVar2 = iVar2 + 1;
        } while (piVar1 != local_8);

        if (iVar2 != -1)
        {
            if (local_c == nullptr)
            {
                return iVar2;
            }

            std::free(local_c);
            return iVar2;
        }
    }

    if (local_c != nullptr)
    {
        std::free(local_c);
    }

    return 0;
}