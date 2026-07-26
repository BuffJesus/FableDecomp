#include <cstddef>
#include <cstdint>
#include <windows.h>

struct CEngineLandscapePatch_AllocateTask_Overlay
{
    std::byte pad_0000[0x08];
    CRITICAL_SECTION field_0008;
};

static_assert(offsetof(CEngineLandscapePatch_AllocateTask_Overlay, field_0008) == 0x08);

void* __cdecl operator_new(std::size_t size);

int __thiscall FUN_00CAF230(
    void* self,
    CEngineLandscapePatch* patch,
    int zero,
    std::uint32_t* out_value,
    std::uint32_t param_3,
    std::uint64_t param_4,
    std::uint32_t param_5,
    std::uint32_t param_6,
    std::uint32_t param_7,
    std::uint32_t param_8,
    std::uint32_t param_9,
    std::uint32_t param_10,
    std::uint32_t param_11,
    std::uint32_t param_12);

std::uint32_t __thiscall CEngineLandscapePatch::AllocateTask(
    int* param_2,
    std::uint32_t param_3,
    std::uint64_t param_4,
    std::uint32_t param_5,
    std::uint32_t param_6,
    std::uint32_t param_7,
    std::uint32_t param_8,
    std::uint32_t param_9,
    std::uint32_t param_10,
    std::uint32_t param_11,
    std::uint32_t param_12)
{
    auto* const self = reinterpret_cast<CEngineLandscapePatch_AllocateTask_Overlay*>(this);
    LPCRITICAL_SECTION const lpCriticalSection = &self->field_0008;

    EnterCriticalSection(lpCriticalSection);

    std::uint32_t local_14 = 0;
    *param_2 = 0;

    void* const pvVar1 = operator_new(0x50);
    std::uint32_t uVar3;

    if (pvVar1 != nullptr)
    {
        const int iVar2 = FUN_00CAF230(
            pvVar1,
            this,
            0,
            &local_14,
            param_3,
            param_4,
            param_5,
            param_6,
            param_7,
            param_8,
            param_9,
            param_10,
            param_11,
            param_12);

        if (iVar2 != 0)
        {
            *param_2 = iVar2;
            uVar3 = local_14;
            LeaveCriticalSection(lpCriticalSection);
            return uVar3;
        }
    }

    uVar3 = 0x8007000E;
    LeaveCriticalSection(lpCriticalSection);
    return uVar3;
}

REVERSED_FUNCTION: CEngineLandscapePatch::AllocateTask (0x00CAF680)