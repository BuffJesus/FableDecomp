#include <cstdio>

struct CGameCameraManager {
    char pad[0xb1];
    unsigned char changeInControlDirection;
};

void __fastcall CGameCameraManager_SetChangeInControlDirection(CGameCameraManager* self)
{
    self->changeInControlDirection = 1;
}

int main()
{
    CGameCameraManager m;
    m.changeInControlDirection = 0;
    CGameCameraManager_SetChangeInControlDirection(&m);
    if (m.changeInControlDirection != 1) {
        std::printf("FAIL: byte at 0xb1 not set\n");
        return 1;
    }
    std::printf("CGameCameraManager_00697750_TEST PASS\n");
    return 0;
}