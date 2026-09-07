#include <stdio.h>

struct Item { int value; };
struct VectorState { unsigned char unused[4]; Item* end; };

static Item* seen_first;
static Item* seen_last;
static Item* seen_destination;

static Item* Copy(Item* first, Item* last, Item* destination)
{
    seen_first = first;
    seen_last = last;
    seen_destination = destination;
    while (first != last) *destination++ = *first++;
    return destination;
}

static Item* PushCopy(VectorState* self, Item* destination, Item* first)
{
    Item* new_end = Copy(first, self->end, destination);
    self->end = new_end;
    return destination;
}

int main()
{
    Item source[3] = { { 4 }, { 7 }, { 9 } };
    Item destination[4] = { { 0 }, { 0 }, { 0 }, { 0 } };
    VectorState state;
    state.end = source + 3;
    if (PushCopy(&state, destination, source) != destination) return 1;
    if (seen_first != source || seen_last != source + 3 || seen_destination != destination) return 2;
    if (destination[0].value != 4 || destination[2].value != 9 || state.end != destination + 3) return 3;
    puts("VECTOR_PUSH_COPY_PASS");
    return 0;
}