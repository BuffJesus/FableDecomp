#include <cstdio>

struct Item { char pad[0x110]; void* m_ptr; };

static int g_helperCalls = 0;
static int g_baseCalls = 0;
static void* g_lastHelperArg = 0;

void HelperModel(void* p) { g_helperCalls++; g_lastHelperArg = p; }
void BaseOnDieModel(Item* self) { g_baseCalls++; (void)self; }

void OnDieModel(Item* self)
{
    void* p = self->m_ptr;
    if (p != 0)
        HelperModel(p);
    BaseOnDieModel(self);
}

int main()
{
    int ok = 1;

    // Case 1: null pointer -> helper NOT called, base called
    Item a; a.m_ptr = 0;
    g_helperCalls = 0; g_baseCalls = 0;
    OnDieModel(&a);
    if (g_helperCalls != 0) ok = 0;
    if (g_baseCalls != 1) ok = 0;

    // Case 2: non-null pointer -> helper called with that ptr, base called
    int dummy = 5;
    Item b; b.m_ptr = &dummy;
    g_helperCalls = 0; g_baseCalls = 0; g_lastHelperArg = 0;
    OnDieModel(&b);
    if (g_helperCalls != 1) ok = 0;
    if (g_lastHelperArg != &dummy) ok = 0;
    if (g_baseCalls != 1) ok = 0;

    if (ok) printf("ONDIE_OK\n");
    else printf("ONDIE_FAIL\n");
    return 0;
}