#include <cstddef>
#include <cstdint>
#include <malloc.h>

struct Batch_InvokeWithStackArgs_c8c730_EaxOverlay {
    std::byte pad_00[0x58];
    std::int32_t count_at_0x58;
    std::byte pad_5c[0x1E0 - 0x5C];
    CBankFile bank_file;
};

static_assert(offsetof(Batch_InvokeWithStackArgs_c8c730_EaxOverlay, count_at_0x58) == 0x58);
static_assert(offsetof(Batch_InvokeWithStackArgs_c8c730_EaxOverlay, bank_file) == 0x1E0);

// Unresolved implicit inputs observed by the decompile/callsite context.
extern CWorldMap* Batch_InvokeWithStackArgs_c8c730_GetImplicitWorldMapThis();
extern long Batch_InvokeWithStackArgs_c8c730_GetImplicitRouteState();

int __stdcall Batch_InvokeWithStackArgs_c8c730(
    std::uint32_t param_1,
    std::uint32_t param_2,
    void* param_3)
{
    auto* const eax_object =
        reinterpret_cast<Batch_InvokeWithStackArgs_c8c730_EaxOverlay*>(__readeax());

    const std::uint32_t route_arg = static_cast<std::uint32_t>(__readedi());
    CWorldMap* const world_map_this =
        Batch_InvokeWithStackArgs_c8c730_GetImplicitWorldMapThis();
    const long route_state =
        Batch_InvokeWithStackArgs_c8c730_GetImplicitRouteState();

    if (eax_object->count_at_0x58 <= 1) {
        return -0x83;
    }

    int result = CreatureHitNotification_WaitForCompletion();
    if (result != 0) {
        return result;
    }

    auto* edit_world = static_cast<CEditWorld*>(Indexed_GetElementOffsetSafe());
    const long next_region =
        CWorldMap::GetNextRegionOnRouteTo(world_map_this, route_arg, route_state);

    const int element_count =
        *reinterpret_cast<int*>(reinterpret_cast<std::byte*>(edit_world) + 0x04);

    auto* const draw_world_map = CEditWorld::DrawGetWorldMap(edit_world);
    const int shifted_world_map =
        static_cast<int>(reinterpret_cast<std::uintptr_t>(draw_world_map)) >>
        ((static_cast<unsigned char>(next_region) + 1U) & 0x1F);

    auto* const bank_file =
        reinterpret_cast<CBankFile*>(reinterpret_cast<std::byte*>(eax_object) + 0x1E0);

    const std::uint32_t first_entry_size = CBankFile::GetEntryDataSize(bank_file, 0);

    auto** const stack_blocks = static_cast<std::uint8_t**>(
        _alloca(static_cast<std::size_t>(element_count) * sizeof(std::uint8_t*)));

    std::uint8_t* current_block = reinterpret_cast<std::uint8_t*>(stack_blocks);
    if (element_count > 0) {
        for (int index = 0; index < element_count; ++index) {
            current_block += shifted_world_map * -4;
            stack_blocks[index] = current_block;
        }
    }

    VertexBuffer_FillData(
        eax_object,
        edit_world,
        bank_file,
        stack_blocks,
        shifted_world_map);

    __asm {
        sub esp, 8
        fld qword ptr [param_1]
        mov ecx, eax_object
        fstp qword ptr [esp]
        call param_3
        mov result, eax
    }

    if (result != 0) {
        return result;
    }

    result = VertexBuffer_WaitAndAllocate();
    if (result != 0) {
        return result;
    }

    edit_world = static_cast<CEditWorld*>(Indexed_GetElementOffsetSafe());
    const std::uint32_t current_count =
        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(edit_world) + 0x04);

    auto* const draw_world_map_after_wait = CEditWorld::DrawGetWorldMap(edit_world);
    const int shifted_world_map_after_wait =
        static_cast<int>(reinterpret_cast<std::uintptr_t>(draw_world_map_after_wait)) >>
        ((static_cast<unsigned char>(next_region) + 1U) & 0x1F);

    const std::uint32_t second_entry_size = CBankFile::GetEntryDataSize(bank_file, 0);

    std::uint32_t swap_output;
    reinterpret_cast<void(__fastcall*)(CBankFile*, std::uint32_t*)>(Buffer_SwapElements)(
        bank_file,
        &swap_output);

    __asm {
        push second_entry_size
        push current_count
        push element_count
        push stack_blocks
        push swap_output
        mov edx, first_entry_size
        mov eax, shifted_world_map_after_wait
        mov ecx, shifted_world_map
        call Audio_ConvolveChannels
    }

    return 0;
}