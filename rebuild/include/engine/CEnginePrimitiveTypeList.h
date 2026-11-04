#pragma once
#include "fable_carray.h"
#include "fable_engine_primitive.h"
// Ego_r.pdb: derives CArray<long>. Donor debug vector is16B; retail STLport
// vector is12B, confirmed by finish/end offsets4/8 in BB1600 and BB1640.
class CEnginePrimitiveTypeList : public CArray<long> {
public:
    void Add(EEnginePrimitiveType type){push_back((long)type);}
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveTypeList)==12);
