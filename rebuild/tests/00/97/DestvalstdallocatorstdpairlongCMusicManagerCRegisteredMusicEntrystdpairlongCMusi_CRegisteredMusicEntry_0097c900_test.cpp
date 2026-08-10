#include <cstdio>

struct Sub { int inited; };
struct Obj { char pad[0x118]; Sub sub; int based; };

static void Sub_Init(Sub* s) { s->inited = 1; }
static void Base_Do(Obj* o) { o->based = 1; }

static void DestVal_118_A(Obj* self)
{
    Sub_Init(&self->sub);
    Base_Do(self);
}

int main()
{
    Obj o;
    o.sub.inited = 0;
    o.based = 0;
    DestVal_118_A(&o);
    if (o.sub.inited == 1 && o.based == 1) {
        printf("OK_0097c900\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}