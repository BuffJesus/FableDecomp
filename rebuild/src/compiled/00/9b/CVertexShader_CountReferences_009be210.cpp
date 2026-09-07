#include "engine/CVertexShader.h"  // retyped onto the PDB layout; byte parity re-verified
struct CVertexShaderData { char pad[0x10]; unsigned long f; };
struct CVertexShader_Methods : CVertexShader {
    unsigned long M();
};
unsigned long CVertexShader_Methods::M(){ return this->Data->f; }