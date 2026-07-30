#include <cstdio>

// CTCWeapon::SetAiming @ 0x005da4d0
// if (this->m_aiming == -1) this->m_aiming = 0;
struct CTCWeapon {
    char pad[0x14];
    int m_aiming; // +0x14
    void __fastcall SetAiming();
};

void __fastcall CTCWeapon::SetAiming() {
    if (m_aiming == -1)
        m_aiming = 0;
}

int main() {
    CTCWeapon w;
    // Case 1: field == -1 -> becomes 0
    w.m_aiming = -1;
    w.SetAiming();
    if (w.m_aiming != 0) { printf("FAIL case1 got %d\n", w.m_aiming); return 1; }

    // Case 2: field != -1 -> unchanged
    w.m_aiming = 5;
    w.SetAiming();
    if (w.m_aiming != 5) { printf("FAIL case2 got %d\n", w.m_aiming); return 1; }

    // Case 3: field == 0 -> unchanged
    w.m_aiming = 0;
    w.SetAiming();
    if (w.m_aiming != 0) { printf("FAIL case3 got %d\n", w.m_aiming); return 1; }

    printf("OK_0x005da4d0\n");
    return 0;
}