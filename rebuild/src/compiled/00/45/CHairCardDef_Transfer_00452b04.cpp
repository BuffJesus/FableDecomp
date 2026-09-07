#include "engine/CHairCardDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTransferObject {
    void Accept(void* field);
};


void __fastcall CHairCardDef_Transfer(CHairCardDef* self, int /*edx*/, CTransferObject* other) {
    other->Accept(&self->HairObject);
}