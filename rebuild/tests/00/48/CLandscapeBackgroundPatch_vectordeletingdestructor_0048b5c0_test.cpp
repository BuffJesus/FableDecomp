#include <cstdio>

struct CLandscapeBackgroundPatch
{
    int tag;
    void Destroy();
};

static int g_destroyed = 0;
static int g_freed = 0;
static void* g_freed_ptr = 0;

void CLandscapeBackgroundPatch::Destroy() { g_destroyed++; }
void FreeMem(void* p) { g_freed++; g_freed_ptr = p; }

void* __fastcall LandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int edx, unsigned int flags);

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.tag = 42;

    // flags & 1 == 0 : destroy only, no free
    g_destroyed = g_freed = 0; g_freed_ptr = 0;
    void* r0 = LandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    bool ok0 = (r0 == &obj) && g_destroyed == 1 && g_freed == 0;

    // flags & 1 == 1 : destroy + free
    g_destroyed = g_freed = 0; g_freed_ptr = 0;
    void* r1 = LandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    bool ok1 = (r1 == &obj) && g_destroyed == 1 && g_freed == 1 && g_freed_ptr == &obj;

    // flags with other bits but bit0 set
    g_destroyed = g_freed = 0;
    void* r2 = LandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 3);
    bool ok2 = (r2 == &obj) && g_freed == 1;

    if (ok0 && ok1 && ok2)
        printf("VDD_PASS_0048b5c0\n");
    else
        printf("FAIL d=%d f=%d\n", g_destroyed, g_freed);
    return (ok0 && ok1 && ok2) ? 0 : 1;
}