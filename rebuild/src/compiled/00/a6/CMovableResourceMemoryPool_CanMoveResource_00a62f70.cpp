#include "fable_handle_memory_pool.h"
#include "fable_game_text_bank.h"
#include <string.h>
bool CMovableResourceMemoryPool::CanMoveResource(void* resource, long handle)
{
    return resource ? static_cast<CMovableResource*>(resource)->CanRelocateData(handle) : false;
}
