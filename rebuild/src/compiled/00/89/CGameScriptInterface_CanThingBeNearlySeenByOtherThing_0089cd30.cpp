struct CThingChecker {
    bool CanThingBeSeen(void* a, void* b, int mode);
};

struct CGameScriptInterface {
    bool CanThingBe_NearlySeen_ByOtherThing(void* thingA, void* thingB);
};

bool CGameScriptInterface::CanThingBe_NearlySeen_ByOtherThing(void* thingA, void* thingB)
{
    return ((CThingChecker*)this)->CanThingBeSeen(thingA, thingB, 2);
}