    questState_type["RegisterBoundAliveCondition"] = [pEntityHost](LuaQuestState&) {
        if (!pEntityHost) throw std::runtime_error("Alive condition requires an entity VM");
        RegisterRetailAliveCondition(pEntityHost, pEntityHost->GetParentScript_Stub(), &pEntityHost->m_Me);
    };

