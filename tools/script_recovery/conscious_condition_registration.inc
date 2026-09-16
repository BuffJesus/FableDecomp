    questState_type["RegisterBoundConsciousCondition"] = [pEntityHost](LuaQuestState&) {
        if (!pEntityHost) throw std::runtime_error("Conscious condition requires an entity VM");
        RegisterRetailConsciousCondition(pEntityHost, pEntityHost->GetParentScript_Stub(), &pEntityHost->m_Me);
    };
