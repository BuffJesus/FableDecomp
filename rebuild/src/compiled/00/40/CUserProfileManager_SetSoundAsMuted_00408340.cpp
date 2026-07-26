extern void SoundMutedChanged(void);
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