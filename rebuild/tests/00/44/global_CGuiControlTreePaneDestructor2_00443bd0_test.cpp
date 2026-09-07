#include <stdlib.h>
#include <stdio.h>

struct State { void* memory; };
static int sort_calls;
static int base_calls;
static int delete_calls;

static State* Destroy(State* self, unsigned char flags)
{
    ++sort_calls;
    if (self->memory != 0) {
        free(self->memory);
        self->memory = 0;
    }
    ++base_calls;
    if ((flags & 1) != 0) ++delete_calls;
    return self;
}

int main()
{
    State state;
    state.memory = malloc(16);
    if (Destroy(&state, 0) != &state || sort_calls != 1 || base_calls != 1 || delete_calls != 0) return 1;
    if (Destroy(&state, 1) != &state || sort_calls != 2 || base_calls != 2 || delete_calls != 1) return 2;
    puts("GUI_TREE_DTOR2_PASS");
    return 0;
}