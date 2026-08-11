#include <cstdio>

struct CThingCreatureBase {
    int h1ret;
    int h2calls;
    bool Helper1();
    void Helper2();
    bool EditWorldUpdate();
};

bool CThingCreatureBase::Helper1() { return h1ret != 0; }
void CThingCreatureBase::Helper2() { h2calls++; }

bool CThingCreatureBase::EditWorldUpdate()
{
    if (Helper1())
        return true;
    Helper2();
    return false;
}

int main()
{
    CThingCreatureBase a; a.h1ret = 1; a.h2calls = 0;
    bool r1 = a.EditWorldUpdate();
    // helper1 true -> returns true, helper2 not called
    bool ok1 = (r1 == true) && (a.h2calls == 0);

    CThingCreatureBase b; b.h1ret = 0; b.h2calls = 0;
    bool r2 = b.EditWorldUpdate();
    // helper1 false -> returns false, helper2 called once
    bool ok2 = (r2 == false) && (b.h2calls == 1);

    if (ok1 && ok2) { printf("EDITWORLDUPDATE_OK\n"); return 0; }
    printf("FAIL\n");
    return 1;
}