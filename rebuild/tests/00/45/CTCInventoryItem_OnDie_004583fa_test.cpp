#include <cstdio>

struct CTCInventoryItem {
    char pad[0x44];
    void* f44;
};

static int g_freed = 0;
static void* g_last_free = 0;
static int g_base_called = 0;

extern "C" void my_free(void* p) { g_freed++; g_last_free = p; }
void __fastcall base_die_forward(CTCInventoryItem* self) { g_base_called++; }

void __fastcall OnDie(CTCInventoryItem* self);

int main()
{
    // case 1: f44 non-null -> free called, base called
    CTCInventoryItem a; a.f44 = (void*)0x1234;
    OnDie(&a);
    bool ok1 = (g_freed == 1) && (g_last_free == (void*)0x1234) && (g_base_called == 1);

    // case 2: f44 null -> no free, base still called
    g_freed = 0; g_base_called = 0; g_last_free = 0;
    CTCInventoryItem b; b.f44 = 0;
    OnDie(&b);
    bool ok2 = (g_freed == 0) && (g_base_called == 1);

    if (ok1 && ok2)
        printf("004583fa_TEST PASS\n");
    else
        printf("FAIL %d %d\n", (int)ok1, (int)ok2);
    return 0;
}