struct CTCPhysicsNavigator {
    char pad[0x48];
    unsigned char enabled;
};

void __fastcall CTCPhysicsNavigator_EnableNavigator(CTCPhysicsNavigator* self, void* /*edx*/, unsigned char enable)
{
    self->enabled = enable;
}