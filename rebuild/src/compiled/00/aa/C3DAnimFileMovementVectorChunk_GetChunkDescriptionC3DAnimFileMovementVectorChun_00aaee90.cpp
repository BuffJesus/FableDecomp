#include "engine/C3DAnimFileMovementVectorChunk.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public: CCharString(const char* text, int length); void* Data; };
struct C3DAnimFileMovementVectorChunk_Methods : C3DAnimFileMovementVectorChunk {
    CCharString GetChunkDescription() const;
};
CCharString C3DAnimFileMovementVectorChunk_Methods::GetChunkDescription() const
{
    return CCharString("MovementVector", -1);
}
