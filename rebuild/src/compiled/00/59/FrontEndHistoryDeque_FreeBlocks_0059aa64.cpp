// Front-end history deque block-array cleanup helper @ 0x0059AA64.
//
// The catalog's generic StdArray_Free_Elements label omits the useful type
// relationship.  The paired allocator at 0x0059AA43 allocates 0x80-byte
// blocks, matching the 32-pointer block size used by the front-end history
// deque distance helper at 0x0059A8F3.  This routine deletes each non-null
// block pointer in the half-open map-slot range [first, last).

extern "C" void __cdecl
FrontEndHistoryBlockDelete_0059aa64(void* block);

extern "C" __declspec(naked)
void __fastcall FrontEndHistoryDeque_FreeBlocks_0059aa64(
    void*,
    void*,
    void**,
    void**)
{
    __asm
    {
        push esi
        mov esi, dword ptr [esp + 8]
        jmp check_slot

    free_slot:
        mov eax, dword ptr [esi]
        test eax, eax
        je next_slot
        push eax
        call FrontEndHistoryBlockDelete_0059aa64
        pop ecx

    next_slot:
        add esi, 4

    check_slot:
        cmp esi, dword ptr [esp + 0ch]
        jb free_slot
        pop esi
        ret 8

        // The complete retail symbol region carries one padding return before
        // the following ShareData helper.
        ret
    }
}
