#include <stdlib.h>
#include <stdio.h>

struct State { void* vftable; void* heap_block; };
static int base_calls;
static int delete_calls;
static void* expected_vftable = (void*)0x1230ba0;

static State* Destroy(State* self, unsigned char flags)
{
    if (self->heap_block != 0) {
        free(self->heap_block);
        self->heap_block = 0;
    }
    self->vftable = expected_vftable;
    ++base_calls;
    if ((flags & 1) != 0) ++delete_calls;
    return self;
}

int main()
{
    State state;
    state.heap_block = malloc(8);
    if (Destroy(&state, 0) != &state || state.vftable != expected_vftable) return 1;
    if (base_calls != 1 || delete_calls != 0) return 2;
    state.heap_block = 0;
    if (Destroy(&state, 1) != &state || base_calls != 2 || delete_calls != 1) return 3;
    puts("MEMORY_BLOCK_DESTRUCTOR_PASS");
    return 0;
}