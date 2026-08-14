#include <cstdio>

struct Item {
    void* vtbl;
    char pad[0x68];
    void* field6c;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

void Freer(void* p) { g_freed++; g_freed_ptr = p; }
void BaseOnDie(Item* self) { g_base_called++; }

void OnDie(Item* self)
{
    if (self->field6c) {
        Freer(self->field6c);
    }
    self->vtbl = (void*)0x1230ba0;
    BaseOnDie(self);
}

int main()
{
    // case 1: field6c non-null -> Freer called
    Item a;
    a.vtbl = 0;
    int dummy = 5;
    a.field6c = &dummy;
    OnDie(&a);
    if (g_freed != 1) { printf("BAD1\n"); return 1; }
    if (g_freed_ptr != &dummy) { printf("BAD2\n"); return 1; }
    if (a.vtbl != (void*)0x1230ba0) { printf("BAD3\n"); return 1; }
    if (g_base_called != 1) { printf("BAD4\n"); return 1; }

    // case 2: field6c null -> Freer NOT called
    Item b;
    b.vtbl = 0;
    b.field6c = 0;
    OnDie(&b);
    if (g_freed != 1) { printf("BAD5\n"); return 1; }
    if (b.vtbl != (void*)0x1230ba0) { printf("BAD6\n"); return 1; }
    if (g_base_called != 2) { printf("BAD7\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}