struct CGameCameraManager {
    char pad[0xb1];
    unsigned char changeInControlDirection;
};

void __fastcall CGameCameraManager_SetChangeInControlDirection(CGameCameraManager* self)
{
    self->changeInControlDirection = 1;
}