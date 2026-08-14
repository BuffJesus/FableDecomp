#include <cstdio>

struct Sub { int calls; void* lastMsg; };
struct Obj { char pad[0x4b]; unsigned char flag; char pad2[0x9c-0x4c]; Sub sub; };
struct Exec { char pad[0x14]; Obj* obj; };

static void* const g_PredMsg = (void*)0x12d29d4;

static void SubHelper(char* subptr, void* msg)
{
    Sub* s = (Sub*)subptr;
    s->calls++;
    s->lastMsg = msg;
}

static void OnPredicateFail(Exec* self)
{
    SubHelper((char*)self->obj + 0x9c, g_PredMsg);
    ((char*)self->obj)[0x4b] = 1;
}

int main()
{
    Obj o;
    o.flag = 0;
    o.sub.calls = 0;
    o.sub.lastMsg = 0;
    Exec e;
    e.obj = &o;

    OnPredicateFail(&e);

    if (o.flag == 1 && o.sub.calls == 1 && o.sub.lastMsg == g_PredMsg) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL flag=%d calls=%d\n", o.flag, o.sub.calls);
    }
    return 0;
}