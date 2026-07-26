CTCCoopSpirit* __fastcall CTCCoopSpirit::Construct(CThing* pThing)
{
    static_assert(sizeof(CTCCoopSpirit) == 0x34);

    auto* spirit = static_cast<CTCCoopSpirit*>(operator new(0x34));
    if (spirit != nullptr) {
        return ::new (spirit) CTCCoopSpirit(pThing);
    }

    return nullptr;
}