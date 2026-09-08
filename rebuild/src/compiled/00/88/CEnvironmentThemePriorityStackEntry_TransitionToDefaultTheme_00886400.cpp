extern double GetGameTimeSeconds();

// Retail omits four bytes present before these transition fields in Ego_r;
// the field identities are shared, while the offsets below are retail's.
struct CEnvironmentThemePriorityStackEntryRetail {
    float BaseWeight;
    unsigned char BaseWeightLocked;
    unsigned char ThemesAndPadding[0x0f];
    long ThemeTransitionTargetID;
    float ThemeTransitionInitialWeight;
    bool ThemeTransitionActive;
    unsigned char Alignment[3];
    double ThemeTransitionStartTime;
    float ThemeTransitionDuration;

    void TransitionToDefaultTheme(float duration);
};

void CEnvironmentThemePriorityStackEntryRetail::TransitionToDefaultTheme(float duration)
{
    ThemeTransitionTargetID = -1;
    ThemeTransitionActive = true;
    ThemeTransitionStartTime = GetGameTimeSeconds();
    ThemeTransitionDuration = duration;
    ThemeTransitionInitialWeight = BaseWeight;
}