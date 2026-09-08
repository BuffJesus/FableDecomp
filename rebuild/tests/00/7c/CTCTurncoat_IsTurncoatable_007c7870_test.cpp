#include <cstdio>
#include "engine/CTCTurncoat.h"

struct CRetailTurncoatDef {
    unsigned char _base[0x25];
    bool Turncoatable;
};

struct CTCTurncoat_Methods : CTCTurncoat {
    int IsTurncoatable();
};

int CTCTurncoat_Methods::IsTurncoatable() {
    if (TurncoatOverride) {
        const CRetailTurncoatDef* definition =
            reinterpret_cast<const CRetailTurncoatDef*>(PDef_Object);
        if (definition->Turncoatable) return 1;
    }
    return 0;
}

int main() {
    CRetailTurncoatDef definition;
    CTCTurncoat_Methods turncoat;
    turncoat.PDef_Object = reinterpret_cast<CDefPointeeBase*>(&definition);
    turncoat.TurncoatOverride = true;
    definition.Turncoatable = true;
    if (turncoat.IsTurncoatable() != 1) return 1;
    turncoat.TurncoatOverride = false;
    if (turncoat.IsTurncoatable() != 0) return 2;
    turncoat.TurncoatOverride = true;
    definition.Turncoatable = false;
    if (turncoat.IsTurncoatable() != 0) return 3;
    std::printf("TURNCOAT_OK\n");
    return 0;
}
