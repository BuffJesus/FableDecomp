#include <cstdio>

struct Item {
    void** vtbl;
    char pad[0x28 - 4];
    void* field28;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

void FreeSlot(void* p) { g_freed++; g_freed_ptr = p; }
void BaseOnDie(Item* self) { g_base_called++; }

void OnDie_model(Item* self)
{
    if (self->field28) {
        FreeSlot(self->field28);
    }
    self->vtbl = (void**)0x1230ba0;
    BaseOnDie(self);
}

int main()
{
    // Case 1: field28 non-null -> FreeSlot called
    Item a; a.vtbl = 0; a.field28 = (void*)0x1234;
    OnDie_model(&a);
    if (g_freed != 1 || g_freed_ptr != (void*)0x1234) { printf("FAIL free\n"); return 1; }
    if (a.vtbl != (void**)0x1230ba0) { printf("FAIL vtbl\n"); return 1; }
    if (g_base_called != 1) { printf("FAIL base\n"); return 1; }

    // Case 2: field28 null -> FreeSlot NOT called
    Item b; b.vtbl = 0; b.field28 = 0;
    OnDie_model(&b);
    if (g_freed != 1) { printf("FAIL free2\n"); return 1; }
    if (b.vtbl != (void**)0x1230ba0) { printf("FAIL vtbl2\n"); return 1; }
    if (g_base_called != 2) { printf("FAIL base2\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}