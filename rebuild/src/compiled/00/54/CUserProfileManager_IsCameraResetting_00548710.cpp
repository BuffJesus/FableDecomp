// Retail 0x00548710. The active profile mode at +0x0C selects which of
// the two independently stored camera-reset states is authoritative.
struct CUserProfileManager {
    char m_leading[0x0C];
    bool m_bAlternateCameraMode;
    char m_between0DAnd50[0x43];
    bool m_bCameraResettingWhenModeDisabled;
    char m_between51And10C[0xBB];
    bool m_bCameraResettingWhenModeEnabled;
    bool IsCameraResetting() const;
};

bool CUserProfileManager::IsCameraResetting() const {
    if (m_bAlternateCameraMode) {
        return m_bCameraResettingWhenModeEnabled;
    }
    return m_bCameraResettingWhenModeDisabled;
}