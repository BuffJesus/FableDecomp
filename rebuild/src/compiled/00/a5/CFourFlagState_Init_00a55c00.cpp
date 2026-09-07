// Retail 0x00A55C00. Four adjacent byte flags: the first is set, the other
// three cleared. Field names are by offset - their meaning is not established.
struct CFourFlagState {
    bool m_flag0;
    bool m_flag1;
    bool m_flag2;
    bool m_flag3;
    void Init();
};

void CFourFlagState::Init() {
    m_flag0 = true;
    m_flag1 = false;
    m_flag2 = false;
    m_flag3 = false;
}