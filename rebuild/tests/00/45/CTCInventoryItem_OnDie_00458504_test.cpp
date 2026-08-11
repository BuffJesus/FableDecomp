#include <cstdio>

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;
static void* g_base_self = 0;

void __cdecl my_free(void* p) { g_freed++; g_freed_ptr = p; }
void __fastcall base_OnDie(void* self) { g_base_called++; g_base_self = self; }

struct CTCInventoryItem {
    char pad[0x3c];
    void* f3c;
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    if (this->f3c)
        my_free(this->f3c);
    base_OnDie(this);
}

int main()
{
    // Case 1: f3c non-null -> free called with it, then base called
    CTCInventoryItem a;
    int dummy = 0;
    a.f3c = &dummy;
    a.OnDie();
    bool ok1 = (g_freed == 1) && (g_freed_ptr == &dummy) &&
               (g_base_called == 1) && (g_base_self == &a);

    // Case 2: f3c null -> no free, base still called
    g_freed = 0; g_base_called = 0;
    CTCInventoryItem b;
    b.f3c = 0;
    b.OnDie();
    bool ok2 = (g_freed == 0) && (g_base_called == 1) && (g_base_self == &b);

    if (ok1 && ok2)
        printf("00458504_TEST PASS\n");
    else
        printf("FAIL f1=%d f2=%d\n", (int)ok1, (int)ok2);
    return 0;
}