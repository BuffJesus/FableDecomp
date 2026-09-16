// Separate capability proposal. Add an exclusively owned MazeSwordSlot member
// to the quest state; construct with the quest and never bind the slot itself.
void LuaQuestState::InitializeMazeSword() {
    m_mazeSwordSlot->LookupAndSetInLimbo("GoodSword",true,true);
}
void LuaQuestState::UnlimboMazeSword() {
    m_mazeSwordSlot->SetInLimboAndAlpha(false,true,0.0f,true);
}
