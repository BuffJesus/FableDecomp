struct CTCPhysicsNavigator { char pad[0x3c]; char enabled; };

void __fastcall CTCPhysicsNavigator_EnableNavigator(CTCPhysicsNavigator* self, int /*edx*/, char enable)
{
    self->enabled = enable;
}