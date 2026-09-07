#include "engine/CThingSearchTools.h"
#include <cstdio>

struct TypeList { char data[20]; }; // sizeof 20

struct CThingManager {
    TypeList* base;
    TypeList* lists;
};


TypeList* __fastcall CThingSearchTools_PeekTypeList(CThingSearchTools* self, int /*edx*/, int type)
{
    return self->ThingManager->lists + (unsigned char)type;
}

int main()
{
    TypeList lists[10];
    CThingManager arr;
    arr.base = 0;
    arr.lists = lists;
    CThingSearchTools tools;
    tools.ThingManager = &arr;

    for (int t = 0; t < 5; ++t) {
        TypeList* got = CThingSearchTools_PeekTypeList(&tools, 0, t);
        TypeList* expect = lists + t;
        if (got != expect) {
            std::printf("FAIL: t=%d got=%p expect=%p\n", t, (void*)got, (void*)expect);
            return 1;
        }
    }
    std::printf("CThingSearchTools_0049c780_TEST PASS\n");
    return 0;
}