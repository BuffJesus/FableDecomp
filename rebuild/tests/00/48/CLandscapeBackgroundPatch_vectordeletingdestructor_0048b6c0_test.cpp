#include <cstdio>

extern "C" void __cdecl eng_operator_delete(void* p);

struct CLandscapeBackgroundPatch
{
    int dummy;
    void lbp_scalar_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted_ptr = 0;

void CLandscapeBackgroundPatch::lbp_scalar_dtor()
{
    g_dtor_called++;
}

extern "C" void __cdecl eng_operator_delete(void* p)
{
    g_delete_called++;
    g_deleted_ptr = p;
}

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->lbp_scalar_dtor();
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.dummy = 7;

    // flags without bit0: dtor runs, no delete, returns this
    g_dtor_called = g_delete_called = 0; g_deleted_ptr = 0;
    void* r1 = obj.vector_deleting_destructor(0);
    bool ok1 = (g_dtor_called == 1) && (g_delete_called == 0) && (r1 == &obj);

    // flags with bit0: dtor runs AND delete called with this
    g_dtor_called = g_delete_called = 0; g_deleted_ptr = 0;
    void* r2 = obj.vector_deleting_destructor(1);
    bool ok2 = (g_dtor_called == 1) && (g_delete_called == 1) &&
               (g_deleted_ptr == &obj) && (r2 == &obj);

    // even flag value with bit0 set (3) => delete
    g_dtor_called = g_delete_called = 0;
    obj.vector_deleting_destructor(3);
    bool ok3 = (g_delete_called == 1);

    // flag value 2 (bit0 clear) => no delete
    g_dtor_called = g_delete_called = 0;
    obj.vector_deleting_destructor(2);
    bool ok4 = (g_delete_called == 0) && (g_dtor_called == 1);

    if (ok1 && ok2 && ok3 && ok4)
        printf("VDD_PARITY_OK\n");
    else
        printf("VDD_FAIL %d %d %d %d\n", ok1, ok2, ok3, ok4);
    return (ok1 && ok2 && ok3 && ok4) ? 0 : 1;
}