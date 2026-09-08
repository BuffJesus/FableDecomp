#include "engine/CEngineInternalPrimitiveParticleGroup.h"

struct CEnginePrimitive;

struct CParticleVector4 {
    int values[4];
};

struct CEnginePrimitiveUpdateView {
    unsigned char padding0[0xc];
    unsigned long type;
    unsigned char enabled;
    unsigned char visible;
    unsigned char padding12[2];
    unsigned long mode;
    unsigned char padding18;
    unsigned char option0;
    unsigned char option1;
    unsigned char option2;
    CParticleVector4 transform;
};

struct CEngineInternalPrimitiveParticleGroup_Methods
    : CEngineInternalPrimitiveParticleGroup {
    void Update(const CEnginePrimitive& primitive, unsigned long key);
};

void CEngineInternalPrimitiveParticleGroup_Methods::Update(
    const CEnginePrimitive& primitive,
    unsigned long key)
{
    const CEnginePrimitiveUpdateView& source =
        (const CEnginePrimitiveUpdateView&)primitive;
    unsigned char* bytes = (unsigned char*)this;
    *(unsigned long*)(bytes + 0xc) = key;
    bytes[0xa0] = source.enabled;
    bytes[0xa1] = source.visible;
    bytes[0x27] = 0xff;
    *(unsigned long*)(bytes + 0x6c) = source.type;
    *(CParticleVector4*)(bytes + 0x5c) = source.transform;
    *(unsigned long*)(bytes + 0xa4) = source.mode;
    bytes[0xa8] = source.option0;
    bytes[0xa9] = source.option1;
    bytes[0xaa] = source.option2;
    switch (*(unsigned long*)(bytes + 0xa4)) {
    case 0:
        *(unsigned long*)(bytes + 0x30) = 0x8000;
        break;
    case 1:
        *(unsigned long*)(bytes + 0x30) = 0x100000;
        break;
    case 2:
        *(unsigned long*)(bytes + 0x30) = 0x80000;
        break;
    }
}
