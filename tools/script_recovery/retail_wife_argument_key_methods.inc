    bool WithArgumentKey(int number, sol::protected_function callback) {
        CheckOpen();
        WifeArgumentKeyAPIs api{GFIntToCharString_API,CCharString_Construct_Literal,
            ASLR<WifeConcatStrings>(0x0099F570),CCharString_Destroy,
            CCharString_AssignLiteral_API,TextEntryExists_API,AddLineToConversation_API};
        return WithRetailWifeArgumentKey(m_game,number,callback,api);
    }
    void AddArgumentKeyLine(RetailWifeArgumentKey& key, int conversation,
                            sol::object speaker, sol::object listener) {
        CheckOpen();
        auto* from=ResolveThingArgument(speaker);
        auto* to=ResolveThingArgument(listener);
        key.AddLine(conversation,from,to);
    }
