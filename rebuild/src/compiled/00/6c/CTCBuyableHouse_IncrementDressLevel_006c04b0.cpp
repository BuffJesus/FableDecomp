struct CTCBuyableHouse {
    char pad[0x1c];
    int field_1c;
    char pad2[0x14];
    int field_34;
    void SetDressLevel(int lvl);
};

extern int GetNewThing();

void __fastcall CTCBuyableHouse_IncrementDressLevel(CTCBuyableHouse* self)
{
    self->SetDressLevel(self->field_1c + 1);
    self->field_34 = GetNewThing();
}