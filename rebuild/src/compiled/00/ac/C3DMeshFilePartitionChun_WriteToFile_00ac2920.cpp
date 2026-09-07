#include "engine/C3DMeshFilePartitionChunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct C3DMeshPartitionElement { void N(void* a); };
struct C3DMeshFilePartitionChunk_Methods : C3DMeshFilePartitionChunk {
    void WriteToFile(void* a, int b);
};
void C3DMeshFilePartitionChunk_Methods::WriteToFile(void* a, int b){ this->PartitionTree_Data->N(a); }