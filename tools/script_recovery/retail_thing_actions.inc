    // Resolve opaque live Thing IDs at the call boundary; no raw pointer escapes
    // to Lua. Owned/borrowed actor userdata uses sol's pointer conversion only.
    CScriptThing* ResolveThingArgument(sol::object value) {
        CheckOpen();
        if (value.is<CScriptThing*>()) {
            auto* actor = value.as<CScriptThing*>();
            if (actor) return actor;
        }
        if (value.is<unsigned>()) {
            auto id = value.as<unsigned>();
            if (static_cast<double>(id) == value.as<double>()) return &Get(id, Kind::Thing).thing;
        }
        throw std::runtime_error("Retail Thing argument requires a live Thing ID or actor");
    }
    bool ThingsAreWithinDistance(sol::object first, sol::object second, float distance) {
        if (!IsDistanceBetweenThingsUnder_API) throw std::runtime_error("Retail Thing distance API is unavailable");
        return IsDistanceBetweenThingsUnder_API(ResolveThingArgument(first), ResolveThingArgument(second), distance);
    }
    void FaceThing(sol::object actor, sol::object target, bool snap) {
        if (!EntitySetFacingAngleTowardsThing_API) throw std::runtime_error("Retail facing API is unavailable");
        EntitySetFacingAngleTowardsThing_API(m_game, ResolveThingArgument(actor), ResolveThingArgument(target), snap);
    }
    void AddConversationPerson(int conversation, sol::object actor) {
        if (!AddPersonToConversation_API) throw std::runtime_error("Retail conversation person API is unavailable");
        AddPersonToConversation_API(m_game, conversation, ResolveThingArgument(actor));
    }
    void AddConversationLine(int conversation, const std::string& text,
                             sol::object speaker, sol::object listener, bool flag) {
        if (!AddLineToConversation_API) throw std::runtime_error("Retail conversation line API is unavailable");
        auto* from = ResolveThingArgument(speaker);
        auto* to = ResolveThingArgument(listener);
        FableString line(text.c_str());
        AddLineToConversation_API(m_game, conversation, line.get(), flag, from, to);
    }
