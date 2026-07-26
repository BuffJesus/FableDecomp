#include <cstdio>

struct RefObj { void** vtbl; int refCount; };
struct RefPtr { RefObj* obj; };
typedef void (__fastcall *DtorFn)(RefObj*);
extern void __fastcall sub_0040fd80(RefPtr*, void*, RefObj**);

static int g_destroyed = 0;
static void __fastcall myDtor(RefObj* o) { (void)o; g_destroyed = 1; }

int main()
{
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&myDtor;

    RefObj cur;
    cur.vtbl = vt;
    cur.refCount = 1;

    RefObj inc;
    inc.vtbl = vt;
    inc.refCount = 5;
    RefObj* incPtr = &inc;

    RefPtr p;
    p.obj = &cur;

    sub_0040fd80(&p, 0, &incPtr);

    if (g_destroyed == 1 && p.obj == &inc && inc.refCount == 6) {
        printf("OK_0040fd80\n");
    } else {
        printf("FAIL d=%d obj=%p rc=%d\n", g_destroyed, (void*)p.obj, inc.refCount);
    }
    return 0;
}