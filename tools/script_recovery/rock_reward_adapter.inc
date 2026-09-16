// Isolated proposal. Declare/bind AddRockTrollReward(CScriptThing*,int) on quest.
// 415D70 converts a four-byte CDefString token into a constructed hidden-output CString.
void LuaQuestState::AddRockTrollReward(CScriptThing* target, int reward) {
    if (!m_pGameInterface || !target || !AddItemToContainer_API || !CCharString_Destroy)
        throw std::runtime_error("Rock reward APIs unavailable");
    if (reward != 1 && reward != 2) throw std::runtime_error("Invalid Rock reward index");
    auto* definition = *ASLR<const unsigned char**>(0x0143E90C);
    if (!definition) throw std::runtime_error("Rock reward definition unavailable");
    using Convert = CCharString* (__thiscall*)(const void*, CCharString*);
    struct Converted {
        CCharString value{};
        bool constructed = false;
        ~Converted() { if (constructed) CCharString_Destroy(&value); }
    } item;
    ASLR<Convert>(0x00415D70)(definition + (reward == 1 ? 0x730 : 0x734), &item.value);
    item.constructed = true;
    AddItemToContainer_API(m_pGameInterface, target, &item.value);
    // Destructor runs even for token -1 / an empty CString; no value filter.
}
