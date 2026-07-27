struct CScriptThing_MsgIsPresentedWithItem_Overlay
{
    std::byte pad_0[4];
    void* field_4;
};
static_assert(offsetof(CScriptThing_MsgIsPresentedWithItem_Overlay, field_4) == 0x4);

bool CScriptThing::MsgIsPresentedWithItem(CCharString& string) const
{
    const auto* self = reinterpret_cast<const CScriptThing_MsgIsPresentedWithItem_Overlay*>(this);
    if (self->field_4 == nullptr)
    {
        return false;
    }

    using IndirectFn = bool(__thiscall*)(void*, CCharString&);
    auto** vftable = *reinterpret_cast<void***>(self->field_4);
    return reinterpret_cast<IndirectFn>(vftable[0x8C / sizeof(void*)])(self->field_4, string);
}