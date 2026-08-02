struct CInputProcessControlCreatureRightStick {
    char pad[0x28];
    char flagA;
    char flagB;
    char flagC;
    void Init();
};

void __fastcall CInputProcessControlCreatureRightStick_Init(CInputProcessControlCreatureRightStick* self) {
    self->flagA = 0;
    self->flagB = 0;
    self->flagC = 0;
}