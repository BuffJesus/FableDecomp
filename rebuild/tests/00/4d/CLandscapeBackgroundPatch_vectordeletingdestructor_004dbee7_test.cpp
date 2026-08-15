// Behaviour model of the vector deleting destructor:
//   run scalar dtor; if (flags & 1) operator delete(this); return this.
#include <stdio.h>

static int g_dtor_calls = 0;
static int g_free_calls = 0;

struct Obj
{
    int x;
    void scalar_dtor() { ++g_dtor_calls; }
};

static void my_free(void *p) { (void)p; ++g_free_calls; }

static void *vec_del_dtor(Obj *self, unsigned int flags)
{
    self->scalar_dtor();
    if (flags & 1)
        my_free(self);
    return self;
}

int main()
{
    Obj a; a.x = 7;

    // flags bit0 clear -> dtor runs, no free, returns self
    g_dtor_calls = g_free_calls = 0;
    void *r0 = vec_del_dtor(&a, 0);
    if (r0 != &a || g_dtor_calls != 1 || g_free_calls != 0) { printf("FAIL0\n"); return 1; }

    // flags bit0 set -> dtor runs, free runs, returns self
    g_dtor_calls = g_free_calls = 0;
    void *r1 = vec_del_dtor(&a, 1);
    if (r1 != &a || g_dtor_calls != 1 || g_free_calls != 1) { printf("FAIL1\n"); return 1; }

    // upper bits ignored except bit0
    g_dtor_calls = g_free_calls = 0;
    void *r2 = vec_del_dtor(&a, 2);
    if (r2 != &a || g_dtor_calls != 1 || g_free_calls != 0) { printf("FAIL2\n"); return 1; }

    printf("VDD_PARITY_OK\n");
    return 0;
}