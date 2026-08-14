#include <stdio.h>

struct Obj { int a; int b; };

static bool subA(Obj* s) { return s->a != 0; }
static bool subB(Obj* s) { return s->b != 0; }

static bool IsRunnable(Obj* s)
{
    if (!subA(s)) return false;
    if (!subB(s)) return false;
    return true;
}

int main()
{
    Obj o00 = {0,0}, o10 = {1,0}, o01 = {0,1}, o11 = {1,1};
    bool ok = true;
    ok = ok && (IsRunnable(&o00) == false);
    ok = ok && (IsRunnable(&o10) == false);
    ok = ok && (IsRunnable(&o01) == false);
    ok = ok && (IsRunnable(&o11) == true);
    if (ok) printf("BEHAVIOUR_OK\n");
    else printf("BEHAVIOUR_FAIL\n");
    return 0;
}