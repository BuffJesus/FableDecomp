#pragma once
// Hand-owned (Ego_r.pdb): CParticleContext, the environment a particle simulation step runs in (gravity, wind, the
// owner's movement since the last step, and the constraint solver's limits). Abstract interface : CBaseClassNonCopyable,
// 4 bytes (vtable only). Slot order from Ego_r: 0 scalar deleting destructor, 1 GetGravityVector, 2 GetWindVector,
// 3 GetDeltaTransform, 4 GetMaximumIterations, 5 GetMaximumDistanceError (retail CParticleProgram::SimulationStep
// 0x00AE2CB0 calls slots 5 then 4 through [vtable+0x14] / [vtable+0x10]).
#include "rebuild_abi.h"
#include "fable_base_class.h"

class C3DVector;
class CMatrix3x4;

class CParticleContext : public CBaseClassNonCopyable
{
public:
    virtual C3DVector GetGravityVector() const = 0;
    virtual C3DVector GetWindVector() const = 0;
    virtual void GetDeltaTransform(CMatrix3x4& transform) const = 0;
    virtual long GetMaximumIterations() const = 0;
    virtual float GetMaximumDistanceError() const = 0;
};
FABLE_STATIC_ASSERT(sizeof(CParticleContext) == 4);
