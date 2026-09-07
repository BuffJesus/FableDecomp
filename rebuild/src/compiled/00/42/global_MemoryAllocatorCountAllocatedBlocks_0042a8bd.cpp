#pragma optimize("s",on)

void __cdecl free(void* memory);
void* __cdecl GetNoAllocatedAreas(void* current);

int __cdecl MemoryAllocator_CountAllocatedBlocks(void** begin, void** sentinel)
{
    void* current = *begin;
    void* end = *sentinel;
    int count = 0;
    while (current != end) {
        current = GetNoAllocatedAreas(current);
        ++count;
    }
    return count;
}

struct MemoryPoolState
{
    unsigned char* current;
    unsigned char* active_block;
    unsigned char* active_block_end;
    unsigned char** rotation_cursor;
};

void __fastcall MemoryPool_AdvancePointer(MemoryPoolState* pool)
{
    if (pool->current != pool->active_block_end - 4) {
        pool->current += 4;
        return;
    }
    if (pool->active_block != 0) free(pool->active_block);
    unsigned char** cursor = pool->rotation_cursor;
    pool->rotation_cursor = cursor + 1;
    unsigned char* block = cursor[1];
    pool->active_block = block;
    pool->active_block_end = block + 0x80;
    pool->current = block;
}