CLandscapeLayerMesh::CLandscapeLayerMesh(const CEngineLandscapeMap* map)
{
    struct CLandscapeLayerMeshCtorOverlay
    {
        void* vftable;                              // +0x00
        const CEngineLandscapeMap* field_04;       // +0x04, observed ctor store
        std::uint32_t field_08;                    // +0x08
        std::uint32_t field_0C;                    // +0x0C
        std::uint32_t field_10;                    // +0x10
        std::uint32_t field_14;                    // +0x14
        std::uint32_t field_18;                    // +0x18
        std::uint32_t field_1C;                    // +0x1C
        std::uint32_t field_20;                    // +0x20
        std::uint32_t field_24;                    // +0x24
        std::uint32_t field_28;                    // +0x28
        std::uint32_t field_2C;                    // +0x2C
        std::uint16_t field_30;                    // +0x30
        std::uint16_t field_32;                    // +0x32
        std::byte pad34[0x0C];                     // +0x34..+0x3F, untouched here
        std::uint32_t field_40;                    // +0x40
        std::uint8_t field_44;                     // +0x44
        std::byte pad45[0x03];                     // +0x45..+0x47
        std::uint32_t field_48;                    // +0x48
        std::uint32_t field_4C;                    // +0x4C
        std::uint32_t field_50;                    // +0x50
        std::uint16_t field_54;                    // +0x54
        std::uint16_t field_56;                    // +0x56
        std::uint32_t field_58;                    // +0x58
        std::uint32_t field_5C;                    // +0x5C
    };

    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_04) == 0x04);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_08) == 0x08);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_0C) == 0x0C);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_10) == 0x10);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_14) == 0x14);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_18) == 0x18);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_1C) == 0x1C);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_20) == 0x20);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_24) == 0x24);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_28) == 0x28);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_2C) == 0x2C);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_30) == 0x30);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_32) == 0x32);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_40) == 0x40);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_44) == 0x44);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_48) == 0x48);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_4C) == 0x4C);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_50) == 0x50);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_54) == 0x54);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_56) == 0x56);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_58) == 0x58);
    static_assert(offsetof(CLandscapeLayerMeshCtorOverlay, field_5C) == 0x5C);
    static_assert(sizeof(CLandscapeLayerMeshCtorOverlay) == 0x60);

    auto& self = reinterpret_cast<CLandscapeLayerMeshCtorOverlay&>(*this);

    self.field_04 = map;
    self.vftable = reinterpret_cast<void*>(0x012A8334);
    self.field_08 = 0;
    self.field_0C = 0;
    self.field_30 = 0;
    self.field_32 = 0;
    self.field_40 = 0;
    self.field_44 = 1;
    self.field_48 = 0;
    self.field_4C = 0;
    self.field_50 = 0;
    self.field_54 = 0;
    self.field_56 = 0;
    self.field_58 = 0;
    self.field_5C = 0;
    self.field_10 = 0;
    self.field_20 = 0;
    self.field_14 = 0;
    self.field_24 = 0;
    self.field_18 = 0;
    self.field_28 = 0;
    self.field_1C = 0;
    self.field_2C = 0;
}