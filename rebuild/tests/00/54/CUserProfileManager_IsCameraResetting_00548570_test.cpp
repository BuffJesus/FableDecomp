#include <stdio.h>

struct CUserProfileManager {
    char pad0[0xc];
    char m_flagC;    // +0xc
    char pad_d;      // +0xd
    char m_flagE;    // +0xe
    char pad1[0xce - 0xf];
    char m_flagCE;   // +0xce
};

char __fastcall IsCameraResetting(CUserProfileManager* this_ptr) {
    if (this_ptr->m_flagC)
        return this_ptr->m_flagCE;
    return this_ptr->m_flagE;
}

int main() {
    CUserProfileManager obj;
    obj.m_flagC = 1;
    obj.m_flagE = 0x22;
    obj.m_flagCE = 0x55;
    char r1 = IsCameraResetting(&obj);
    if (r1 != 0x55) { printf("FAIL case1 got %d\n", (int)r1); return 1; }

    obj.m_flagC = 0;
    obj.m_flagE = 0x33;
    obj.m_flagCE = 0x77;
    char r2 = IsCameraResetting(&obj);
    if (r2 != 0x33) { printf("FAIL case2 got %d\n", (int)r2); return 1; }

    printf("OK_0x00548570\n");
    return 0;
}