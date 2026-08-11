#include <cstdio>
#include <cstring>

struct CAnimationEntry { unsigned long id; };
struct APair { unsigned long first; CAnimationEntry second; };

struct ConsAlloc {
    void construct(APair *ptr, double val);
};

struct OuterAlloc {
    char pad[0x40];
    ConsAlloc sub;
    void Cons_val(APair *ptr, double val);
};

void OuterAlloc::Cons_val(APair *ptr, double val)
{
    sub.construct(ptr, val);
}

// Capture what construct received.
static APair    *g_ptr;
static double    g_val;
static ConsAlloc *g_this;

void ConsAlloc::construct(APair *ptr, double val)
{
    g_this = this;
    g_ptr  = ptr;
    g_val  = val;
}

int main()
{
    OuterAlloc a;
    APair storage;
    storage.first = 0;
    storage.second.id = 0;

    // Build an 8-byte pair payload and view it as the double that is copied.
    APair v;
    v.first = 0x11223344UL;
    v.second.id = 0x55667788UL;
    double payload;
    memcpy(&payload, &v, sizeof(double));

    a.Cons_val(&storage, payload);

    bool ok = true;
    if (g_ptr != &storage) ok = false;
    if (memcmp(&g_val, &v, sizeof(double)) != 0) ok = false;
    if ((char*)g_this != (char*)&a + 0x40) ok = false;

    printf(ok ? "CONS_VAL_OK\n" : "CONS_VAL_FAIL\n");
    return ok ? 0 : 1;
}