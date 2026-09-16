    // Native B60E..BD87/CCDD: one output object survives both polling sites.
    unsigned NewPresentedItemOutput(CScriptThing* actor) {
        if (!actor || !CCharString_Destroy) throw std::runtime_error("Presented output requires actor/string APIs");
        auto& e = Add(Kind::PresentedOutput);
        using DefaultCtor = CCharString* (__thiscall*)(CCharString*);
        ASLR<DefaultCtor>(0x0099E4B0)(&e.presented);
        e.presentedActor = actor;
        e.live = true;
        return static_cast<unsigned>(m_entries.size());
    }
    bool PollPresentedItem(unsigned id) {
        auto& e = Get(id, Kind::PresentedOutput);
        auto* table = reinterpret_cast<CScriptThingVTable*>(e.presentedActor->pVTable);
        if (!table || !table->MsgIsPresentedWithItem) throw std::runtime_error("Presented query slot unavailable");
        return table->MsgIsPresentedWithItem(e.presentedActor, &e.presented);
    }
    bool PresentedItemMatches(unsigned id, const std::string& name) {
        auto& e = Get(id, Kind::PresentedOutput);
        using NotEqual = bool (__thiscall*)(CCharString*,const char*);
        return !ASLR<NotEqual>(0x0099E960)(&e.presented,name.c_str());
    }
    void DestroyPresentedItemOutput(unsigned id) { Destroy(Get(id,Kind::PresentedOutput)); }
