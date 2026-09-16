// Unapplied LuaRetailResources public method proposal; register on its userdata.
// uint32 slots deliberately preserve the raw zero-extended native byte argument.
void PlayRockTrollAnimation(unsigned id, const std::string& animation) {
    auto& entry = Get(id, Kind::Resource);
    if (animation != "SPECIAL_BOAST" && animation != "SPECIAL_IDLE")
        throw std::runtime_error("Unreviewed Rock Troll animation");
    FableString name(animation.c_str());
    const unsigned nativeFlag = *ASLR<const unsigned char*>(0x01375748);
    using Play = void (__thiscall*)(CScriptGameResourceObjectScriptedThingBase*,
        const CCharString*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
    ASLR<Play>(0x007E73D0)(&entry.resource, name.get(), 0, 0, 0, 1, nativeFlag, 0, 0);
    // Helper handles empty Data; CString still exists and is destroyed afterward.
}
