#include <cstdio>

struct CTCTavernGame {
    char pad[0xa8];
    int stateA;
    char pad2[0xc0 - 0xa8 - 4];
    int stateB;
};

int __fastcall CTCTavernGame_IsBetting(const CTCTavernGame* self) {
    return self->stateB == 0x2d && self->stateA == 0x5;
}

int main() {
    CTCTavernGame g;
    g.stateA = 5; g.stateB = 0x2d;
    if (!CTCTavernGame_IsBetting(&g)) { std::printf("FAIL both-match\n"); return 1; }
    g.stateB = 0;
    if (CTCTavernGame_IsBetting(&g)) { std::printf("FAIL b-mismatch\n"); return 1; }
    g.stateB = 0x2d; g.stateA = 0;
    if (CTCTavernGame_IsBetting(&g)) { std::printf("FAIL a-mismatch\n"); return 1; }
    std::printf("CTCTavernGame_005e9c30_TEST PASS\n");
    return 0;
}