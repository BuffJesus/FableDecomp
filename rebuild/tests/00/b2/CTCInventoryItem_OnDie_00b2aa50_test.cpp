#include <cstdio>

struct CTCInventoryItem {
    char pad[0x208];
    void* p208;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;
static CTCInventoryItem* g_base_self = 0;

void EngineFree(void* p) { g_freed++; g_freed_ptr = p; }
void BaseOnDie(CTCInventoryItem* self) { g_base_called++; g_base_self = self; }

void OnDie(CTCInventoryItem* self)
{
    void* p = self->p208;
    if (p) {
        EngineFree(p);
    }
    BaseOnDie(self);
}

int main()
{
    CTCInventoryItem a;
    // case 1: p208 non-null -> free called, base called
    int dummy = 5;
    a.p208 = &dummy;
    g_freed = 0; g_base_called = 0;
    OnDie(&a);
    if (g_freed != 1) { printf("FAIL freed\n"); return 1; }
    if (g_freed_ptr != &dummy) { printf("FAIL freed_ptr\n"); return 1; }
    if (g_base_called != 1) { printf("FAIL base\n"); return 1; }
    if (g_base_self != &a) { printf("FAIL base_self\n"); return 1; }

    // case 2: p208 null -> no free, base still called
    a.p208 = 0;
    g_freed = 0; g_base_called = 0;
    OnDie(&a);
    if (g_freed != 0) { printf("FAIL free_on_null\n"); return 1; }
    if (g_base_called != 1) { printf("FAIL base2\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}