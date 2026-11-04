#include <stdlib.h>
#include "fable_ime.h"

void* FABLE_FASTCALL NUISystem::CIME::Callback_malloc(unsigned int bytes)
{
    return malloc(bytes);
}

void FABLE_FASTCALL NUISystem::CIME::Callback_free(void* pointer)
{
    free(pointer);
}
