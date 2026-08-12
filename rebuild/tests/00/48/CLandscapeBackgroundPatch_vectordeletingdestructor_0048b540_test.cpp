#include <stdio.h>
#include <stdlib.h>

// Mirror of the source declarations.
struct CLandscapeBackgroundPatch
{
    int marker;
    void destroy();
    void* deleting(unsigned int flags);
};

static int g_destroyed = 0;
static int g_freed = 0;
static void* g_last_free = 0;

// External targets referenced by the source translation unit.
void CLandscapeBackgroundPatch::destroy()
{
    g_destroyed++;
    this->marker = -1;
}

void operator_delete_impl(void* p)
{
    g_freed++;
    g_last_free = p;
}

int main()
{
    int ok = 1;

    // Case A: flags bit0 clear -> destructor runs, no free, returns this.
    {
        CLandscapeBackgroundPatch a;
        a.marker = 7;
        g_destroyed = g_freed = 0; g_last_free = 0;
        void* r = a.deleting(0);
        if (r != &a) ok = 0;
        if (g_destroyed != 1) ok = 0;
        if (g_freed != 0) ok = 0;
        if (a.marker != -1) ok = 0;
    }

    // Case B: flags bit0 set -> destructor runs AND operator delete(this).
    {
        CLandscapeBackgroundPatch b;
        b.marker = 42;
        g_destroyed = g_freed = 0; g_last_free = 0;
        void* r = b.deleting(1);
        if (r != &b) ok = 0;
        if (g_destroyed != 1) ok = 0;
        if (g_freed != 1) ok = 0;
        if (g_last_free != &b) ok = 0;
    }

    // Case C: only bit0 matters (flags=2 -> no free).
    {
        CLandscapeBackgroundPatch c;
        c.marker = 5;
        g_destroyed = g_freed = 0; g_last_free = 0;
        void* r = c.deleting(2);
        if (r != &c) ok = 0;
        if (g_freed != 0) ok = 0;
    }

    if (ok)
        printf("VDD_PARITY_OK\n");
    else
        printf("VDD_PARITY_FAIL\n");
    return ok ? 0 : 1;
}