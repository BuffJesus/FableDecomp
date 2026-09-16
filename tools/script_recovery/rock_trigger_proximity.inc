// Retail F308C0..F308D2 names SCRIPT_DEF+72C RockTrollTriggerProximity.
float LuaQuestState::GetRockTrollTriggerProximity() {
    const auto* definition=*ASLR<const unsigned char**>(0x0143E90C);
    if(!definition) throw std::runtime_error("Rock Troll script definition unavailable");
    float value;
    std::memcpy(&value,definition+0x72c,sizeof(value));
    return value; // reload pointer and value on every call; never cache installed5.0
}
