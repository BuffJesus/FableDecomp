#include <cstdio>

void operator_delete_stub(void *p);

struct CLandscapeBackgroundPatch
{
    void dtor();
    void * vector_deleting_destructor(unsigned int flags);
};

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void *g_deleted_ptr = 0;

void CLandscapeBackgroundPatch::dtor()
{
    g_dtor_calls++;
}

void operator_delete_stub(void *p)
{
    g_delete_calls++;
    g_deleted_ptr = p;
}

int main()
{
    CLandscapeBackgroundPatch obj;

    // flags bit0 clear: dtor runs, no delete, returns this
    g_dtor_calls = g_delete_calls = 0; g_deleted_ptr = 0;
    void *r0 = obj.vector_deleting_destructor(0);
    bool ok0 = (r0 == (void*)&obj) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    // flags bit0 set: dtor runs, delete runs on this, returns this
    g_dtor_calls = g_delete_calls = 0; g_deleted_ptr = 0;
    void *r1 = obj.vector_deleting_destructor(1);
    bool ok1 = (r1 == (void*)&obj) && (g_dtor_calls == 1) && (g_delete_calls == 1) && (g_deleted_ptr == (void*)&obj);

    // higher bits set but bit0 clear -> no delete
    g_dtor_calls = g_delete_calls = 0; g_deleted_ptr = 0;
    void *r2 = obj.vector_deleting_destructor(2);
    bool ok2 = (r2 == (void*)&obj) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    if (ok0 && ok1 && ok2)
        printf("VECDELDTOR_OK\n");
    else
        printf("VECDELDTOR_FAIL %d %d %d\n", ok0, ok1, ok2);
    return (ok0 && ok1 && ok2) ? 0 : 1;
}