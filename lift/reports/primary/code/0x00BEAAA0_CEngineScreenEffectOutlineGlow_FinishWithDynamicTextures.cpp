void CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures()
{
    struct FinishWithDynamicTexturesOverlay
    {
        void** vfptr;
        std::byte pad_04[0x0C];
        int field_10;
        void* Blur2Texture;
    };

    static_assert(offsetof(FinishWithDynamicTexturesOverlay, field_10) == 0x10);
    static_assert(offsetof(FinishWithDynamicTexturesOverlay, Blur2Texture) == 0x14);

    auto* self = reinterpret_cast<FinishWithDynamicTexturesOverlay*>(this);

    self->vfptr = &PTR_FinishWithDynamicTextures_012A8134;

    if (self->Blur2Texture != nullptr)
    {
        operator delete(self->Blur2Texture);
        self->Blur2Texture = nullptr;
    }

    if (self->field_10 != 0)
    {
        LandscapeLayerList__InsertNode(self->field_10);
        self->field_10 = 0;
    }

    self->vfptr = &PTR_LAB_0129B860;
}