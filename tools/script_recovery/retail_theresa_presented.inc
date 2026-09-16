    // Main owns one default-constructed CString across both presented-item polls.
    unsigned NewPresentedItemOutput(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !CCharString_Destroy)
            throw std::runtime_error("Theresa presented output requires actor/string APIs");
        auto& entry = Add(Kind::TheresaPresented);
        using DefaultCtor = CCharString* (__thiscall*)(CCharString*);
        ASLR<DefaultCtor>(0x0099E4B0)(&entry.text);
        entry.presentedActor = actor;
        entry.live = true;
        return entry.id;
    }
    bool PollPresentedItem(unsigned id) {
        auto& entry = Get(id, Kind::TheresaPresented);
        auto* table = reinterpret_cast<CScriptThingVTable*>(entry.presentedActor->pVTable);
        if (!table || !table->MsgIsPresentedWithItem)
            throw std::runtime_error("Theresa presented query slot unavailable");
        return table->MsgIsPresentedWithItem(entry.presentedActor, &entry.text);
    }
    bool PresentedItemMatches(unsigned id, const std::string& name) {
        auto& entry = Get(id, Kind::TheresaPresented);
        using NotEqual = bool (__thiscall*)(CCharString*, const char*);
        return !ASLR<NotEqual>(0x0099E960)(&entry.text, name.c_str());
    }
    void DestroyPresentedItemOutput(unsigned id) { Release(id, Kind::TheresaPresented); }
