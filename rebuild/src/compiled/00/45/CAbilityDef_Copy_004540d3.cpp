struct CAbilityDef {
    unsigned char _tail[0x2c];
    void BaseCopy(const CAbilityDef* other);
};

void __fastcall CAbilityDef_Copy(CAbilityDef* self, void* dummy, const CAbilityDef* other)
{
    self->BaseCopy(other);
    *(unsigned long*)(self->_tail + 0x28) = *(const unsigned long*)(other->_tail + 0x28);
}