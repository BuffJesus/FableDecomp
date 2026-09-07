#include "engine/CBufferedFileDataOutputStream.h"  // retyped onto the PDB layout; byte parity re-verified
struct CBufferedFileDataOutputStream_Methods : CBufferedFileDataOutputStream {
    void M(unsigned long v);
};
void CBufferedFileDataOutputStream_Methods::M(unsigned long v){ this->FilePos = v; }