#include <stdio.h>

static int g_dtor = 0;
static int g_del = 0;
static void* g_deleted = 0;

struct CTCMapwho { int x; };

static void dtor(CTCMapwho* self) { if (self) g_dtor++; }
static void opdel(void* p) { g_del++; g_deleted = p; }

static void OnDie_model(CTCMapwho* self)
{
    if (self)
    {
        dtor(self);
        opdel(self);
    }
}

int main()
{
    CTCMapwho obj;
    obj.x = 42;

    /* null path: nothing happens */
    OnDie_model(0);
    if (g_dtor != 0 || g_del != 0) { printf("FAIL null\n"); return 1; }

    /* non-null path: dtor then delete(this) */
    OnDie_model(&obj);
    if (g_dtor != 1) { printf("FAIL dtor\n"); return 1; }
    if (g_del != 1) { printf("FAIL del\n"); return 1; }
    if (g_deleted != (void*)&obj) { printf("FAIL delptr\n"); return 1; }

    printf("OK_0051af00\n");
    return 0;
}