#include "engine/CTextureManager.h"

// Retail keeps the PDB-named Initialised and TextureBuffer members but its
// preallocation storage is 0x40 bytes smaller than Ego_r's donor layout.
struct CTextureManagerRetail {
    void* __vftable;
    bool Initialised;
    unsigned char _retailPreallocationState[0x5cb];
    void* TextureBuffer;
    void Uninitialise();
};

void CTextureManagerRetail::Uninitialise() {
    Initialised = false;
    if (TextureBuffer != 0) {
        TextureBuffer = 0;
    }
}