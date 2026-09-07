#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
struct CBankFileAsync { int Method(int); };
struct CWADFile_Methods : CWADFile {
    int GetLength();
};
int CWADFile_Methods::GetLength(){ return this->Bank->Method(this->Index); }