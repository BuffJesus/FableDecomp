#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWADFile_Methods : CWADFile {
    void M(unsigned long v);
};
void CWADFile_Methods::M(unsigned long v){ this->FilePos = v; }