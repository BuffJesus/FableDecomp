    // Resolve the owned wrapper while its scope is live; return only a vector copy.
    sol::table ThingPosition(unsigned id, sol::this_state state) {
        return ReadRetailThingPosition(&Get(id, Kind::Thing).thing, state);
    }
