struct CTransferObject {
    void Accept(void* field);
};

struct CHairCardDef {
    char pad[0x28];
    int field28;
};

void __fastcall CHairCardDef_Transfer(CHairCardDef* self, int /*edx*/, CTransferObject* other) {
    other->Accept(&self->field28);
}