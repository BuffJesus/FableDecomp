// Standalone behaviour test for GetControlAngleXY @ 0x00697700
// Compiles with cl /O2 ; prints the PassPattern on success.
#include <stdio.h>

struct CGameCameraManager
{
    char _pad0[0xB8];
    float m_controlAngleXY;
};

float __fastcall GetControlAngleXY_CGameCameraManager(CGameCameraManager *thisptr);

int main(void)
{
    CGameCameraManager cam;
    // Sanity: the accessed member must be at byte offset 0xB8.
    if ((char*)&cam.m_controlAngleXY - (char*)&cam != 0xB8)
    {
        printf("OFFSET FAIL\n");
        return 1;
    }

    cam.m_controlAngleXY = 1.5f;
    float a = GetControlAngleXY_CGameCameraManager(&cam);

    cam.m_controlAngleXY = -0.25f;
    float b = GetControlAngleXY_CGameCameraManager(&cam);

    if (a == 1.5f && b == -0.25f)
        printf("CGAMECAMERAMANAGER_00697700_TEST PASS\n");
    else
        printf("TEST FAIL a=%f b=%f\n", a, b);
    return 0;
}
