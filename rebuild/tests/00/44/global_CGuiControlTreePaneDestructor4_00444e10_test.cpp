#include <stdlib.h>
#include <stdio.h>

struct State { void* memory; };
static int stage;

static void Destroy(State* self)
{
    if (self != 0) {
        stage = stage * 10 + 1;
        stage = stage * 10 + 2;
        if (self->memory != 0) { free(self->memory); self->memory = 0; }
        stage = stage * 10 + 3;
        stage = stage * 10 + 4;
    }
}

int main()
{
    Destroy(0);
    if (stage != 0) return 1;
    State state = { malloc(8) };
    Destroy(&state);
    if (stage != 1234 || state.memory != 0) return 2;
    puts("GUI_TREE_DTOR5_PASS");
    return 0;
}