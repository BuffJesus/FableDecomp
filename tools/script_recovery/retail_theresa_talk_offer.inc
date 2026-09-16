    // DB9EE0..DB9F94 keeps the talk key alive across the possession query.
    bool TheresaTalkOffersChocolates(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !actor->pVTable || !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Theresa talk offer APIs unavailable");
        auto* table = reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
        if (!table->MsgIsTalkedToBy) throw std::runtime_error("Theresa talk slot unavailable");
        CCharString key{};
        CCharString_Construct_Literal(&key,"SCRIPT_NAME_HERO",-1);
        bool offers;
        try { offers = table->MsgIsTalkedToBy(actor,&key) && DoesTheresaHeroHaveChocolates(); }
        catch (...) { try { CCharString_Destroy(&key); } catch (...) {} throw; }
        CCharString_Destroy(&key);
        return offers;
    }
