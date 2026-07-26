struct CTCTavernGame {
    char pad[0xa8];
    int stateA;
    char pad2[0xc0 - 0xa8 - 4];
    int stateB;
};

int __fastcall CTCTavernGame_IsBetting(const CTCTavernGame* self) {
    return self->stateB == 0x2d && self->stateA == 0x5;
}