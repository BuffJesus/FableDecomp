#include <stdlib.h>
#include <stdio.h>

struct Entry { int destroyed; int flags; };
struct State { Entry* begin; Entry* end; };
static int owner_deletes;
static int destroyed_count;
static int flags_or;

static void Destroy(State* self)
{
    if (self != 0) {
        for (Entry* current = self->begin; current != self->end; ++current) {
            current->destroyed = 1;
            current->flags = 0;
            ++destroyed_count;
            flags_or |= current->flags;
        }
        if (self->begin != 0) free(self->begin);
        ++owner_deletes;
    }
}

int main()
{
    Destroy(0);
    if (owner_deletes != 0) return 1;
    State state;
    state.begin = (Entry*)malloc(3 * sizeof(Entry));
    state.end = state.begin + 3;
    for (Entry* item = state.begin; item != state.end; ++item) { item->destroyed = 0; item->flags = -1; }
    Destroy(&state);
    if (destroyed_count != 3 || flags_or != 0 || owner_deletes != 1) return 2;
    puts("CATCH_DTOR_FAMILY_PASS");
    return 0;
}