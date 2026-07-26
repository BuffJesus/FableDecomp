struct CHairCardDef {
    unsigned char _pad_0x0[0x28];
    int field28;
};

struct CHairCardBase {
    void Copy(CHairCardDef* other);
};

void __fastcall CHairCardDef_Copy(CHairCardDef* self, int edx_dummy, CHairCardDef* other)
{
    (void)edx_dummy;
    reinterpret_cast<CHairCardBase*>(self)->Copy(other);
    self->field28 = other->field28;
}