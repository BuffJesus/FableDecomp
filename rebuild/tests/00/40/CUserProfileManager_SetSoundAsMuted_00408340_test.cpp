#include <cstdio>
static int g_called = 0;
void SoundMutedChanged(void) { g_called++; }
struct CUserProfileManager {
    char pad[0x31];
    char muted;
    void SetSoundAsMuted(char v);
};
void CUserProfileManager::SetSoundAsMuted(char v)
{
    if (this->muted != v) {
        this->muted = v;
        SoundMutedChanged();
    }
}
int main() {
    CUserProfileManager o;
    o.muted = 0;
    o.SetSoundAsMuted(1);
    if (o.muted != 1 || g_called != 1) { std::printf("FAIL set\n"); return 1; }
    o.SetSoundAsMuted(1);
    if (o.muted != 1 || g_called != 1) { std::printf("FAIL noop\n"); return 1; }
    o.SetSoundAsMuted(0);
    if (o.muted != 0 || g_called != 2) { std::printf("FAIL clear\n"); return 1; }
    std::printf("CUserProfileManager_00408340_TEST PASS\n");
    return 0;
}