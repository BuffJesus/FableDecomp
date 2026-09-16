// Separate opt-in capability; existing CreateThread callers retain guards.
void LuaQuestState::CreateMazeUnlimboThread() {
    if(!m_pParentHost) throw std::runtime_error("Maze helper parent unavailable");
    m_pParentHost->CreateNativeEntryThread("UnLimboSword","",{});
}
