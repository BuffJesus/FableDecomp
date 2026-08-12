#include <cstdio>
#include <new>

static int g_dtor_calls = 0;
static void* g_last_deleted = 0;

struct CLandscapeBackgroundPatch {
    int a, b, c;
    void* deleting_destructor(unsigned int flags);
};

extern void __fastcall CLandscapeBackgroundPatch_dtor(void* self);
void __fastcall CLandscapeBackgroundPatch_dtor(void* self) { (void)self; ++g_dtor_calls; }

void* CLandscapeBackgroundPatch::deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(this);
    if (flags & 1)
        ::operator delete(this);
    return this;
}

int main()
{
    // flags & 1 clear: destructor runs, no delete, returns this
    CLandscapeBackgroundPatch obj;
    g_dtor_calls = 0;
    void* r = obj.deleting_destructor(0);
    bool ok1 = (r == &obj) && (g_dtor_calls == 1);

    // flags & 1 set: destructor runs then operator delete on heap object
    CLandscapeBackgroundPatch* p = new CLandscapeBackgroundPatch();
    g_dtor_calls = 0;
    void* r2 = p->deleting_destructor(1);
    bool ok2 = (r2 == p) && (g_dtor_calls == 1);

    if (ok1 && ok2) {
        printf("VDD_PARITY_OK\n");
        return 0;
    }
    printf("VDD_FAIL %d %d\n", (int)ok1, (int)ok2);
    return 1;
}