CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize(
    void* param_1,
    unsigned long param_2,
    unsigned long param_3,
    unsigned long param_4,
    unsigned long param_5,
    unsigned long param_6)
    : CMemoryAllocatorBase(param_4)
{
    (void)param_1;

    struct FixedSizeOverlay {
        std::byte pad_0000[0x0C14];
        std::uint32_t field_0C14;
        std::uint32_t field_0C18;
        std::uint32_t field_0C1C;
        std::uint32_t field_0C20;
        std::uint32_t field_0C24;
        std::uint32_t field_0C28;
        std::uint32_t field_0C2C;
        std::uint32_t field_0C30;
        std::uint32_t field_0C34;
        std::uint32_t field_0C38;
        unsigned long field_0C3C;
        unsigned long field_0C40;
        std::uint32_t field_0C44;
        std::uint32_t field_0C48;
    };

    static_assert(offsetof(FixedSizeOverlay, field_0C14) == 0x0C14);
    static_assert(offsetof(FixedSizeOverlay, field_0C18) == 0x0C18);
    static_assert(offsetof(FixedSizeOverlay, field_0C1C) == 0x0C1C);
    static_assert(offsetof(FixedSizeOverlay, field_0C20) == 0x0C20);
    static_assert(offsetof(FixedSizeOverlay, field_0C24) == 0x0C24);
    static_assert(offsetof(FixedSizeOverlay, field_0C28) == 0x0C28);
    static_assert(offsetof(FixedSizeOverlay, field_0C2C) == 0x0C2C);
    static_assert(offsetof(FixedSizeOverlay, field_0C30) == 0x0C30);
    static_assert(offsetof(FixedSizeOverlay, field_0C34) == 0x0C34);
    static_assert(offsetof(FixedSizeOverlay, field_0C38) == 0x0C38);
    static_assert(offsetof(FixedSizeOverlay, field_0C3C) == 0x0C3C);
    static_assert(offsetof(FixedSizeOverlay, field_0C40) == 0x0C40);
    static_assert(offsetof(FixedSizeOverlay, field_0C44) == 0x0C44);
    static_assert(offsetof(FixedSizeOverlay, field_0C48) == 0x0C48);

    auto* const self = reinterpret_cast<FixedSizeOverlay*>(this);

    self->field_0C24 = 0;
    self->field_0C44 = 0;
    self->field_0C48 = 0;
    self->field_0C1C = 0;
    self->field_0C20 = 0;

    const std::uint32_t shift = self->field_0C18 & 0x1Fu;
    const std::int32_t iVar1 =
        static_cast<std::int32_t>(
            ((self->field_0C14 + 0x1Bu) >> shift) << shift
        ) - 4;
    self->field_0C30 = static_cast<std::uint32_t>(iVar1);

    *reinterpret_cast<void***>(this) = &PTR__Allocate_CMemoryAllocatorFixedSize__UAEPAXK_Z_0129db64;

    const std::uint32_t uVar2 =
        ((static_cast<std::uint32_t>(param_3) + 3u + static_cast<std::uint32_t>(param_4)) /
         static_cast<std::uint32_t>(param_4)) *
        static_cast<std::uint32_t>(param_4);
    self->field_0C28 = uVar2 - 4;
    const std::uint32_t uVar3 =
        (static_cast<std::uint32_t>(param_2) - static_cast<std::uint32_t>(iVar1)) / uVar2;
    self->field_0C2C = uVar2;
    self->field_0C34 = uVar3;
    self->field_0C38 = uVar3 * uVar2 + static_cast<std::uint32_t>(iVar1);
    self->field_0C3C = param_5;
    self->field_0C40 = param_6;
}