struct CGameScriptInterfaceOwnerOverlay
{
    std::byte pad00[0x7C];
    CScriptConversationManager* conversationManager;
};
static_assert(offsetof(CGameScriptInterfaceOwnerOverlay, conversationManager) == 0x7C);

struct CGameScriptInterfaceOverlay
{
    void* vfptr;
    CGameScriptInterfaceOwnerOverlay* owner;
};
static_assert(offsetof(CGameScriptInterfaceOverlay, owner) == 0x04);

void CGameScriptInterface::AddLineToConversation(
    long conversationId,
    const CCharString& line,
    bool param_4,
    const CScriptThing& speakerThing,
    const CScriptThing& listenerThing)
{
    using ScriptThingBoolCall = char(__thiscall*)(const CScriptThing*);
    using ScriptThingGetThingCall = CThing*(__thiscall*)(const CScriptThing*);

    auto* const speakerVtable = *reinterpret_cast<void***>(const_cast<CScriptThing*>(&speakerThing));
    const auto speakerIsValid =
        reinterpret_cast<ScriptThingBoolCall>(speakerVtable[75])(&speakerThing);
    if (speakerIsValid != '\0')
    {
        CThing* listener = nullptr;

        auto* const listenerVtable = *reinterpret_cast<void***>(const_cast<CScriptThing*>(&listenerThing));
        const auto listenerIsValid =
            reinterpret_cast<ScriptThingBoolCall>(listenerVtable[75])(&listenerThing);
        if (listenerIsValid != '\0')
        {
            listener =
                reinterpret_cast<ScriptThingGetThingCall>(listenerVtable[11])(&listenerThing);
        }

        auto* const overlay = reinterpret_cast<CGameScriptInterfaceOverlay*>(this);
        CScriptConversationManager* const conversationManager =
            overlay->owner->conversationManager;

        CThing* const speaker =
            reinterpret_cast<ScriptThingGetThingCall>(speakerVtable[11])(&speakerThing);

        conversationManager->AddLineToConversation(
            conversationId,
            speaker,
            listener,
            &line,
            param_4);
    }
}