#include "fable_handle_memory_pool.h"
#include "fable_game_text_bank.h"
#include <string.h>
void CMovableResourceMemoryPool::ValidateResource(void* resource)
{
    static_cast<CMovableResource*>(resource)->Verify();
}
