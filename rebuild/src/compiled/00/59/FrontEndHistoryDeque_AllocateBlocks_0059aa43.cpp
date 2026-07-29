// Front-end history deque block-array allocation helper @ 0x0059AA43.
//
// The stale catalog signature describes a byte Fill operation.  Retail
// instead walks a half-open array of map slots and stores one newly allocated
// 0x80-byte block in each slot.  That is 32 pointers per history deque block,
// matching the iterator arithmetic at 0x0059A8F3.

extern "C" void* __cdecl
FrontEndHistoryBlockAllocate_0059aa43(unsigned long size);

extern "C" __declspec(naked)
void __fastcall FrontEndHistoryDeque_AllocateBlocks_0059aa43(
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

    allocate_slot:
        push 80h
        call FrontEndHistoryBlockAllocate_0059aa43
        mov dword ptr [esi], eax
        pop ecx
        add esi, 4

    check_slot:
        cmp esi, dword ptr [esp + 0ch]
        jb allocate_slot
        pop esi
        ret 8
    }
}
