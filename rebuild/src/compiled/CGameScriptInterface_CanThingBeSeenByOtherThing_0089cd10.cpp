struct CThing;
struct CGameScriptInterface {
    bool CanThingBe_Seen_ByOtherThing(CThing* a, CThing* b);
    bool CanSeeImpl(CThing* a, CThing* b, int mode);
};

bool CGameScriptInterface::CanThingBe_Seen_ByOtherThing(CThing* a, CThing* b)
{
    return CanSeeImpl(a, b, 1);
}