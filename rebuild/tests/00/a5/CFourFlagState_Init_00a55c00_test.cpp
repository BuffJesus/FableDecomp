#include <stdio.h>
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
int main() {
    CFourFlagState state;
    state.m_flag0 = false;
    state.m_flag1 = true;
    state.m_flag2 = true;
    state.m_flag3 = true;
    state.Init();
    if (!state.m_flag0) return 1;
    if (state.m_flag1 || state.m_flag2 || state.m_flag3) return 2;
    printf("FOUR_FLAG_INIT PASS\n");
    return 0;
}