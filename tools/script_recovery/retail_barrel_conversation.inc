    void AddBarrelConversation(CScriptThing* actor,const std::string& text) {
        CheckOpen();
        if (!m_game || !AddNewConversation_API || !AddLineToConversation_API ||
            !CCharString_Construct_Literal || !CCharString_Destroy || !RetailThing_Destroy_API)
            throw std::runtime_error("Retail Barrel conversation APIs unavailable");
        int conversation=AddNewConversation_API(m_game,actor,false,false);
        CCharString key{};CScriptThing listener{};
        bool keyLive=false,listenerLive=false;
        try {
            CCharString_Construct_Literal(&key,text.c_str(),-1);keyLive=true;
            using ConstructThing=CScriptThing*(__thiscall*)(CScriptThing*);
            auto* target=ASLR<ConstructThing>(0x006E7B40)(&listener);listenerLive=true;
            AddLineToConversation_API(m_game,conversation,&key,false,actor,target);
            listenerLive=false;RetailThing_Destroy_API(&listener);
            keyLive=false;CCharString_Destroy(&key);
        } catch (...) {
            if(listenerLive)try {listenerLive=false;RetailThing_Destroy_API(&listener);}catch(...) {}
            if(keyLive)try {keyLive=false;CCharString_Destroy(&key);}catch(...) {}
            throw;
        }
    }
