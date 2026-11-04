#include "fable_handle_memory_pool.h"
#include "fable_game_text_bank.h"
#include <string.h>
void CMovableResourceMemoryPool::MoveResource(void* resource, long handle, void* from, void* to, unsigned long size)
{
    memmove(to,from,size);
    if(resource) static_cast<CMovableResource*>(resource)->RelocateData(handle,from,to,size);
}
