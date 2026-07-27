namespace
{
struct CActionPointInterfaceOwnerOverlay
{
    std::byte pad00[0x24];
    std::uint32_t flags;
    std::byte pad28[0x44 - 0x28];
    int* interfacesBegin;
    int* interfacesEnd;
};

static_assert(offsetof(CActionPointInterfaceOwnerOverlay, flags) == 0x24);
static_assert(offsetof(CActionPointInterfaceOwnerOverlay, interfacesBegin) == 0x44);
static_assert(offsetof(CActionPointInterfaceOwnerOverlay, interfacesEnd) == 0x48);
}

bool CGameScriptInterface::GetSleepingPositionAndOrientationFromBed(
    const CScriptThing& thing0,
    const CScriptThing& thing1,
    C3DVector& outVector0,
    C3DVector& outVector1)
{
    const auto callIsValidScriptThing = [](const CScriptThing& thing) -> bool
    {
        auto** const vftable = *reinterpret_cast<void***>(const_cast<CScriptThing*>(&thing));
        using Fn = char(__thiscall*)(const CScriptThing*);
        return reinterpret_cast<Fn>(vftable[0x12C / 4])(&thing) != 0;
    };

    const auto callGetThing = [](const CScriptThing& thing) -> CThing*
    {
        auto** const vftable = *reinterpret_cast<void***>(const_cast<CScriptThing*>(&thing));
        using Fn = CThing*(__thiscall*)(const CScriptThing*);
        return reinterpret_cast<Fn>(vftable[0x2C / 4])(&thing);
    };

    const auto callBedCheck = [](CGameScriptInterface* self, const CScriptThing& thing) -> bool
    {
        auto** const vftable = *reinterpret_cast<void***>(self);
        using Fn = char(__thiscall*)(CGameScriptInterface*, const CScriptThing*);
        return reinterpret_cast<Fn>(vftable[0xAA8 / 4])(self, &thing) != 0;
    };

    if (!callIsValidScriptThing(thing0))
    {
        return false;
    }

    if (!callIsValidScriptThing(thing1))
    {
        return false;
    }

    if (!callBedCheck(this, thing1))
    {
        return false;
    }

    CThing* const thing1Object = callGetThing(thing1);
    auto* const owner = reinterpret_cast<CActionPointInterfaceOwnerOverlay*>(thing1Object);
    if ((owner->flags & 0x2000u) == 0)
    {
        return false;
    }

    ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x2D);
    auto* const interfaces =
        reinterpret_cast<CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>*>(
            &owner->interfacesBegin);

    int* lowerBound =
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>::LowerBound(
            interfaces,
            &interfaceType);

    if (lowerBound == owner->interfacesEnd || *lowerBound > static_cast<int>(interfaceType))
    {
        lowerBound = owner->interfacesEnd;
    }

    auto* const actionPoint = reinterpret_cast<CTCUserActionPointBase*>(lowerBound[1]);

    CRightHandedSet scratchSet;
    C3DVector tempVector;
    if (!actionPoint->GetFreeActionPoint(callGetThing(thing0), outVector0, &scratchSet, &tempVector))
    {
        return false;
    }

    outVector1 = tempVector;
    CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera(
        reinterpret_cast<CTCCameraPointScriptedSpline*>(actionPoint));
    return true;
}