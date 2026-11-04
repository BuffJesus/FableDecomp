// NParticleEngine::CParticleEmitterDatabase::OpenBank @ 0x00AE8980
// (FableWin ?OpenBank@CParticleEmitterDatabase@NParticleEngine@@QAEXABVCCharString@@@Z).
//
// Replaces the database's particle bank with a fresh CParticleDataBank,
// opens it read-only (flags 0x48) and builds the database's symbol map from
// the bank's entries.

#include "fable_particle_engine.h"

void NParticleEngine::CParticleEmitterDatabase::OpenBank(const CCharString& bankName)
{
    EmitterBank.Reset(new CParticleDataBank);
    EmitterBank->OpenReadOnly(bankName, 0x48);
    EmitterBank->CreateSymbolMap(GetSymbolMap(), false);
}