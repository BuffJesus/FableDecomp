// Native inlined Thing position query: implementation slot 0x18 when present,
// otherwise the current relocated fallback vector. Never synthesize zeroes.
inline sol::table ReadRetailThingPosition(CScriptThing* actor, sol::this_state state) {
    // Forge represents a lookup's empty native wrapper as nil.
    auto* implementation = actor ? reinterpret_cast<CGameScriptThing*>(actor->pImp.Data) : nullptr;
    const volatile C3DVector* position = nullptr;
    if (implementation) {
        auto* table = reinterpret_cast<CGameScriptThingVTable*>(implementation->pVTable);
        if (!table || !table->GetPos) throw std::runtime_error("Retail position vtable is unavailable");
        position = table->GetPos(implementation);
    } else {
        position = ASLR<const volatile C3DVector*>(0x0143E8E0);
    }
    if (!position) throw std::runtime_error("Retail position returned a null vector");
    sol::state_view lua(state);
    return lua.create_table_with("x", static_cast<float>(position->x),
                                 "y", static_cast<float>(position->y),
                                 "z", static_cast<float>(position->z));
}
