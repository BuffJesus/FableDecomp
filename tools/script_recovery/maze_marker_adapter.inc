// Pending host declaration/binding; preserves EA7CC0..EA7D13 temporary order.
void LuaQuestState::AddMiniMapMarkerByScriptName(const std::string& name, const std::string& markerName) {
    if(!m_pGameInterface || !GetThingWithScriptName_ByName_API || !MiniMapAddMarker_API ||
       !RetailThing_Destroy_API || !g_pCScriptThingVTable)
        throw std::runtime_error("Named marker APIs unavailable");
    FableString marker(markerName.c_str());
    FableString key(name.c_str());
    struct ReturnedThing {
        CScriptThing value{};
        ReturnedThing() { value.pVTable=g_pCScriptThingVTable; }
        ~ReturnedThing() { RetailThing_Destroy_API(&value); }
    } target;
    GetThingWithScriptName_ByName_API(m_pGameInterface,&target.value,key);
    MiniMapAddMarker_API(m_pGameInterface,&target.value,marker);
    // Destroy target, key, marker, including an empty native output.
}
