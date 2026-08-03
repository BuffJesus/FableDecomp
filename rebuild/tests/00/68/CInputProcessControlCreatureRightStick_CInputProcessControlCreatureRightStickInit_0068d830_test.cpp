#include <cstring>
#include <cstdio>

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

int main() {
    CInputProcessControlCreatureRightStick obj;
    memset(&obj, 0xFF, sizeof(obj));
    CInputProcessControlCreatureRightStick_Init(&obj);
    bool ok = (obj.flagA == 0) && (obj.flagB == 0) && (obj.flagC == 0);
    if (ok) {
        printf("CAND_INIT_ZERO_OK\n");
    } else {
        printf("CAND_INIT_ZERO_FAIL\n");
    }
    return 0;
}