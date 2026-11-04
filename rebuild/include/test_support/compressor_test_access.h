#pragma once
#include "engine/CCompressorZlib.h"
// Test-only access preserves the private PDB signatures of these methods.
class CCompressorZlibTestAccess
{
public:
    static void Initialise(CCompressorZlib& c) { c.Initialise(); }
    static void* Alloc(void* context, unsigned int count, unsigned int size) { return CCompressorZlib::Alloc(context,count,size); }
    static void Free(void* context, void* memory) { CCompressorZlib::Free(context,memory); }
};
