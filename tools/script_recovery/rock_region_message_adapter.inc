// Include rock_region_message.h and declare this method in LuaQuestState.
// RegisterRockRegionMessage(lua) before binding the quest method below.
void LuaQuestState::WithRegionLoadedMessage(sol::protected_function callback) {
    WithRockRegionMessage(m_pGameInterface, callback,
        ASLR<RockDefaultStringCtor>(0x0099E4B0), MsgOnRegionLoaded_API, CCharString_Destroy);
}
// Quest usertype: type["WithRegionLoadedMessage"] = &LuaQuestState::WithRegionLoadedMessage;
