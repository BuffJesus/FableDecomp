namespace
{
    extern void* PTR__vector_deleting_destructor__0125d2c4;

    struct CTCPhysicsBaseLayout
    {
        void** Vftable;                 // 0x00
        std::byte pad04[0x08];         // 0x04
        C3DVector Position;            // 0x0C
        C3DVector OldPosition;         // 0x18
        long OldPositionLastFrameSet;  // 0x24
        C3DVector Velocity;            // 0x28
        float Radius;                  // 0x34
        std::uint32_t PhysicsDef;      // 0x38
        std::uint8_t MovingFlags;      // 0x3C
        std::byte pad3D[0x03];         // 0x3D
    };

    static_assert(offsetof(CTCPhysicsBaseLayout, Vftable) == 0x00);
    static_assert(offsetof(CTCPhysicsBaseLayout, Position) == 0x0C);
    static_assert(offsetof(CTCPhysicsBaseLayout, OldPosition) == 0x18);
    static_assert(offsetof(CTCPhysicsBaseLayout, OldPositionLastFrameSet) == 0x24);
    static_assert(offsetof(CTCPhysicsBaseLayout, Velocity) == 0x28);
    static_assert(offsetof(CTCPhysicsBaseLayout, Radius) == 0x34);
    static_assert(offsetof(CTCPhysicsBaseLayout, PhysicsDef) == 0x38);
    static_assert(offsetof(CTCPhysicsBaseLayout, MovingFlags) == 0x3C);
    static_assert(sizeof(CTCPhysicsBaseLayout) == 0x40);
}

CTCPhysicsBase::CTCPhysicsBase(CThing& thing)
    : CEngineSubPrimitive2DClockSprite(
          *reinterpret_cast<CEngineInternalPrimitive2DClockSprite*>(&thing))
{
    auto& self = *reinterpret_cast<CTCPhysicsBaseLayout*>(this);

    self.Vftable = reinterpret_cast<void**>(&PTR__vector_deleting_destructor__0125d2c4);
    self.Position.x = 0.0f;
    self.Position.y = 0.0f;
    self.Position.z = 0.0f;
    self.OldPosition.x = 0.0f;
    self.OldPosition.y = 0.0f;
    self.OldPosition.z = 0.0f;
    self.OldPositionLastFrameSet = 0;
    self.Velocity.x = 0.0f;
    self.Velocity.y = 0.0f;
    self.Velocity.z = 0.0f;
    self.Radius = -1.0f;
    self.PhysicsDef = 0;
    self.MovingFlags &= 0xFC;
}