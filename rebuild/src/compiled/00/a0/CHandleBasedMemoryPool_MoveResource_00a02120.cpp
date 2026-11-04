#include "fable_handle_memory_pool.h"
#include "fable_game_text_bank.h"
#include <string.h>
void CHandleBasedMemoryPool::MoveResource(void*, long, void* from, void* to, unsigned long size)
{
    memmove(to,from,size);
}
