#include "engine/CDriveManager.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSystemManager; struct CDriveManager_Methods : CDriveManager {
    int Initialise(CSystemManager* a);
};
int CDriveManager_Methods::Initialise(CSystemManager* a){ this->SystemManager = a; return 1; }