// Forwarder to a member of the Bank-object pointer at this+0x4. VC7.1 tail-calls
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CBankFileAsync { void Run(); };
struct CWADFile_Methods : CWADFile {
    void Run();
};
#pragma pack(pop)
void CWADFile_Methods::Run() { this->Bank->Run(); }