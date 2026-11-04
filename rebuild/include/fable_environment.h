#pragma once

// CEnvironment's polymorphic interface as retail dispatches through it.
//
// engine/CEnvironment.h is the generated PDB layout (a flat struct with no
// virtuals); a TU that needs CEnvironment's virtual slots includes this
// header instead. The two must not meet in one TU.
//
// Slot order from Ego_r.pdb's ??_7CEnvironment@@6B@ (read from ego_r.exe and
// symbolised through the PDB): 0 ??_E, 1 PeekInterpolatedParams (private),
// 2 PeekWindDirection, 3 PeekWindSpeed, 4 PeekSunVector, 5 PeekMoonVector.
// Retail CParticleEnvironmentInteraction::GetWindVelocity 0x0049BE10 calls
// slots 2 and 3 ([vptr+8], [vptr+0xC]). Bases and data are not modelled.

#include "fable_2d_vector.h"

struct CIDrawEnvironmentParams;

// FableWin CEnvironmentSpellEffectInterpolator: only ClearScreenFilters
// (retail 0x006B8410; caller CWorld::ActivateWorld 0x0049F180).
class CEnvironmentSpellEffectInterpolator
{
public:
    void ClearScreenFilters();
};

// Ego_r CEnvironmentSpellEffectInterface (CEnvironment::SpellEffectInterface): only FrameUpdate (retail 0x00885010,
// Ego_r 0x008C4A70), which CEnvironment::UpdateInterpolation forwards to.
class CEnvironmentSpellEffectInterface
{
public:
    void FrameUpdate();
};

class CEnvironment
{
public:
    virtual ~CEnvironment();

private:
    // ?PeekInterpolatedParams@CEnvironment@@EBEABUCIDrawEnvironmentParams@@XZ
    virtual const CIDrawEnvironmentParams& PeekInterpolatedParams() const;

public:
    // ?PeekWindDirection@CEnvironment@@UBEABVC3DVector@@XZ
    virtual const C3DVector& PeekWindDirection() const;
    // ?PeekWindSpeed@CEnvironment@@UBEMXZ
    virtual float PeekWindSpeed() const;
    // ?PeekSunVector@CEnvironment@@UBEABVC3DVector@@XZ
    virtual const C3DVector& PeekSunVector() const;
    // ?PeekMoonVector@CEnvironment@@UBEABVC3DVector@@XZ
    virtual const C3DVector& PeekMoonVector() const;

    // FableWin ?GetSpellEffectInterpolator@CEnvironment@@QAEAAVCEnvironmentSpellEffectInterpolator@@XZ;
    // inline in retail (ActivateWorld 0x0049F180 loads +0x1964).
    // Ego_r ?UpdateInterpolation@CEnvironment@@QAEXXZ (retail 0x006B32A0; CWorld::UpdateFrame calls it): advance the
    // spell-effect interface for this frame.
    void UpdateInterpolation();

    CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator()
    {
        return *SpellEffectInterpolator;
    }

private:
    // Layout added for GetSpellEffectInterpolator / UpdateInterpolation; retail offsets (Ego_r has the two
    // scoped_ptrs at +0x1A8C / +0x1A90). The retail size is not modelled.
    fable_u8 unknown04_[0x1960 - 0x04];
    CEnvironmentSpellEffectInterface* SpellEffectInterface;         // +0x1960 scoped_ptr (Ego_r +0x1A8C)
    CEnvironmentSpellEffectInterpolator* SpellEffectInterpolator;   // +0x1964 scoped_ptr
};
