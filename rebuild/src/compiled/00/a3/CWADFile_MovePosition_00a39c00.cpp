#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWADFile_Methods : CWADFile {
    void MovePosition(long a);
};
void CWADFile_Methods::MovePosition(long a){ this->FilePos += a; }