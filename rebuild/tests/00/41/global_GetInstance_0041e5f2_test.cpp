// Standalone behaviour test for GetInstance @ 0x0041e5f2
// Links against local stubs for operator new and the ctor.
#include <stdio.h>

struct CFrontEndManager
{
    unsigned char _pad[0xd0];
};

// ---- controllable stubs ----
static unsigned char g_arena[0xd0];
static int g_new_calls = 0;
static int g_ctor_calls = 0;

void * operator new(unsigned int size)
{
    ++g_new_calls;
    if (size != 0xd0) return 0;   // engine allocates exactly 0xd0 bytes
    return g_arena;
}
void operator delete(void *) {}

extern "C" CFrontEndManager * __fastcall CFrontEndManager_ctor(CFrontEndManager *self)
{
    ++g_ctor_calls;
    return self; // ctor returns 'this'
}

static CFrontEndManager * g_instance = 0;

CFrontEndManager * __fastcall NUISystem_CFrontEndManager_GetInstance(void)
{
    CFrontEndManager *p = g_instance;
    if (p == 0)
    {
        void *mem = operator new(0xd0);
        if (mem != 0)
            p = CFrontEndManager_ctor((CFrontEndManager *)mem);
        else
            p = 0;
        g_instance = p;
    }
    return p;
}

int main(void)
{
    CFrontEndManager *a = NUISystem_CFrontEndManager_GetInstance(); // first: alloc+ctor once
    CFrontEndManager *b = NUISystem_CFrontEndManager_GetInstance(); // second: cached

    int ok = 1;
    if (a == 0)            ok = 0;   // constructed
    if (a != b)            ok = 0;   // cached (identical pointer)
    if (g_new_calls != 1)  ok = 0;   // allocated once
    if (g_ctor_calls != 1) ok = 0;   // constructed once
    if ((void*)a != (void*)g_arena) ok = 0; // returned the allocation

    if (ok)
        printf("GLOBAL_0041e5f2_TEST PASS\n");
    else
        printf("GLOBAL_0041e5f2_TEST FAIL (a=%p b=%p new=%d ctor=%d)\n",
               (void*)a, (void*)b, g_new_calls, g_ctor_calls);
    return 0;
}