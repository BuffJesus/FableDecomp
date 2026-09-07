#include "engine/CBankFile.h"  // retyped onto the PDB layout; byte parity re-verified
struct CBankFile_Methods : CBankFile {
    bool M(int x0, int x1);
};
bool CBankFile_Methods::M(int x0, int x1) { return true; }