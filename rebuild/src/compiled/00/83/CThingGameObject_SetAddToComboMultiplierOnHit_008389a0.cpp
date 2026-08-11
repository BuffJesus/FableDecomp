struct CThingGameObject {
    char pad[0xc8];
    unsigned char flags;
    void SetAddToComboMultiplierOnHit(bool v);
};

void CThingGameObject::SetAddToComboMultiplierOnHit(bool v)
{
    flags = (unsigned char)((flags & 0xfe) | (v & 1) | 2);
}