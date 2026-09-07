#include <stdio.h>

struct Item { unsigned long value; };
struct VectorState { unsigned char unused[4]; Item* finish; Item* end; };

static int insert_calls;
static Item* insert_position;
static Item* insert_value;

static void Push(VectorState* self, Item* value)
{
    Item* finish = self->finish;
    if (finish != self->end) {
        if (finish != 0) *finish = *value;
        self->finish = (Item*)((unsigned char*)self->finish + 4);
        return;
    }
    ++insert_calls;
    insert_position = finish;
    insert_value = value;
}

int main()
{
    Item storage[2];
    Item value = { 0x10203040UL };
    VectorState state;
    state.finish = storage;
    state.end = storage + 2;
    Push(&state, &value);
    if (storage[0].value != value.value || state.finish != storage + 1 || insert_calls != 0) return 1;
    state.finish = state.end;
    Push(&state, &value);
    if (insert_calls != 1 || insert_position != state.end || insert_value != &value) return 2;
    puts("STD_VECTOR_PUSH_BACK_PASS");
    return 0;
}