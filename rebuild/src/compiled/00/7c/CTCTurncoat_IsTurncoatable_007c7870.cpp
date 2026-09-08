#include "engine/CTCTurncoat.h"

// Retail CTurncoatDef is 0x54 bytes rather than the donor's 0x58, but the
// PDB-backed Turncoatable member agrees at +0x25.
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
