#include <stdio.h>
struct CUserProfileManager {
    char m_leading[0x0C]; bool m_bAlternateCameraMode;
    char m_between0DAnd50[0x43]; bool m_bCameraResettingWhenModeDisabled;
    char m_between51And10C[0xBB]; bool m_bCameraResettingWhenModeEnabled;
    bool IsCameraResetting() const;
};
bool CUserProfileManager::IsCameraResetting() const {
    if (m_bAlternateCameraMode) return m_bCameraResettingWhenModeEnabled;
    return m_bCameraResettingWhenModeDisabled;
}
int main() {
    CUserProfileManager profiles;
    profiles.m_bCameraResettingWhenModeDisabled = true;
    profiles.m_bCameraResettingWhenModeEnabled = false;
    profiles.m_bAlternateCameraMode = false;
    if (!profiles.IsCameraResetting()) return 1;
    profiles.m_bAlternateCameraMode = true;
    if (profiles.IsCameraResetting()) return 2;
    printf("IS_CAMERA_RESETTING PASS\n");
    return 0;
}