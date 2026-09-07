#include <cstddef>
#include <cstdint>

struct SerializeAllocationAreas_Entry {
    std::byte pad_00[0x10];
    std::uint32_t byte_10;
    // Verified from the machine code, not the Ghidra pseudo-C:
    // `d9 46 14` / `d9 1c 24` loads a 32-bit float from +0x14 and marshals it
    // onto the stack for the following call with `ecx = stream`.
    float float_14;
};
static_assert(offsetof(SerializeAllocationAreas_Entry, byte_10) == 0x10);
static_assert(offsetof(SerializeAllocationAreas_Entry, float_14) == 0x14);

struct SerializeAllocationAreas_ListHead {
    std::byte pad_00[0x08];
    SerializeAllocationAreas_Entry* next;
};
static_assert(offsetof(SerializeAllocationAreas_ListHead, next) == 0x08);

struct SerializeAllocationAreas_State {
    SerializeAllocationAreas_ListHead* head;
    std::uint32_t area_count;
};
static_assert(offsetof(SerializeAllocationAreas_State, head) == 0x00);
static_assert(offsetof(SerializeAllocationAreas_State, area_count) == 0x04);

void __fastcall SerializeAllocationAreas(
    CDataOutputStream* stream,
    SerializeAllocationAreas_State* state)
{
    using WriteFloatHelper = void(__thiscall*)(CDataOutputStream*, float);
    using AdvanceEntryRaw = SerializeAllocationAreas_Entry*(__cdecl*)(SerializeAllocationAreas_Entry*);

    static constexpr auto WriteFloat =
        reinterpret_cast<WriteFloatHelper>(0x00994EE0);
    static constexpr auto AdvanceEntry =
        reinterpret_cast<AdvanceEntryRaw>(0x004292C0);

    CDataOutputStream::WriteCBYTE(stream, static_cast<char>(state->area_count));

    SerializeAllocationAreas_Entry* entry = state->head->next;
    if (entry != reinterpret_cast<SerializeAllocationAreas_Entry*>(state->head)) {
        do {
            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(entry->byte_10));

            // Ghidra printed `WriteCBYTE(..., *(... + 0x14))`, but the bytes show:
            //   push ecx
            //   mov  ecx, stream
            //   fld  dword ptr [entry+0x14]
            //   fstp dword ptr [esp]
            //   call 0x00994EE0
            // so +0x14 is treated as a float argument to a helper call here.
            WriteFloat(stream, entry->float_14);

            entry = AdvanceEntry(entry);
        } while (entry != reinterpret_cast<SerializeAllocationAreas_Entry*>(state->head));
    }
}