#include <cstdio>

struct CThingSub { char pad[0x34]; float radius; };
struct CThing { char pad[0x60]; CThingSub* sub; };
struct FilterObj { float minAllowedRadius; };

static bool impl(FilterObj* selfp, CThing* thingp)
{
    return thingp->sub->radius > selfp->minAllowedRadius;
}

int main()
{
    CThingSub subObj;
    CThing thingObj;
    CThingSub* subPtr = &subObj;
    thingObj.sub = subPtr;
    FilterObj filt;
    FilterObj* filtPtr = &filt;
    CThing* thingPtr = &thingObj;

    int ok = 1;

    filtPtr->minAllowedRadius = 5.0f; subObj.radius = 6.0f;
    if (impl(filtPtr, thingPtr) != true) ok = 0;

    filtPtr->minAllowedRadius = 5.0f; subObj.radius = 5.0f;
    if (impl(filtPtr, thingPtr) != false) ok = 0;

    filtPtr->minAllowedRadius = 5.0f; subObj.radius = 4.0f;
    if (impl(filtPtr, thingPtr) != false) ok = 0;

    filtPtr->minAllowedRadius = -1.0f; subObj.radius = 0.0f;
    if (impl(filtPtr, thingPtr) != true) ok = 0;

    if (ok) printf("BEHAVIOR_OK\n");
    else    printf("BEHAVIOR_FAIL\n");
    return 0;
}