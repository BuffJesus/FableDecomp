#include "engine/CDataInputStream.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDataInputStream_Methods : CDataInputStream {
    unsigned long M();
};
unsigned long CDataInputStream_Methods::M(){ return this->StreamPos; }