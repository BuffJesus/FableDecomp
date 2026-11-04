"""Fresh VC71 parity gate: NParticleEngine::Initialise (0x00AEBE20), Uninitialise (0x00AEBEA0), SetEnvironmentInteraction (0x00AEAB20), exact after relocation (existing sources and fixtures, first exact-root gate)"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CPSCManager@NParticleEngine@@QAE@XZ':0xaf2970,
 '??0CParticleEmitterDatabase@NParticleEngine@@QAE@XZ':0xae8bf0,
 '??0CParticleEmitterManager@NParticleEngine@@QAE@XZ':0xaeba90,
 '??2@YAPAXI@Z':0xbfea1a,
 '??3@YAXPAX@Z':0xbfe9bc,
 '?PEmitterDatabase@NParticleEngine@@3V?$CCountedPointer@VCParticleEmitterDatabase@NParticleEngine@@@@A':0x13d2e14,
 '?PEmitterManager@NParticleEngine@@3V?$CCountedPointer@VCParticleEmitterManager@NParticleEngine@@@@A':0x13d2e1c,
 '?PEnvironmentInteraction@NParticleEngine@@3V?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@A':0x13d2e2c,
 '?PPSCManager@NParticleEngine@@3V?$CCountedPointer@VCPSCManager@NParticleEngine@@@@A':0x13d2e24,
 '?Reset@?$CCountedPointer@VCPSCManager@NParticleEngine@@@@QAEXPAVCPSCManager@NParticleEngine@@@Z':0xaecc30,
 '?Reset@?$CCountedPointer@VCParticleEmitterDatabase@NParticleEngine@@@@QAEXPAVCParticleEmitterDatabase@NParticleEngine@@@Z':0xaeccc0,
 '?Reset@?$CCountedPointer@VCParticleEmitterManager@NParticleEngine@@@@QAEXPAVCParticleEmitterManager@NParticleEngine@@@Z':0xaecac0,
}
ROOTS=[
 dict(name='particle_engine_initialise',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Initialise_00aebe20.cpp',symbol='?Initialise@NParticleEngine@@YIXXZ',address=0xaebe20,size=122,instructions=40,relocations=14),
 dict(name='particle_engine_uninitialise',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Uninitialise_00aebea0.cpp',symbol='?Uninitialise@NParticleEngine@@YIXXZ',address=0xaebea0,size=163,instructions=49,relocations=18),
 dict(name='particle_engine_set_environment_interaction',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_SetEnvironmentInteraction_00aeab20.cpp',symbol='?SetEnvironmentInteraction@NParticleEngine@@YIXV?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@@Z',address=0xaeab20,size=99,instructions=34,relocations=7),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-engine-lifetime.json',[],
  'NParticleEngine::Initialise (0x00AEBE20), Uninitialise (0x00AEBEA0), SetEnvironmentInteraction (0x00AEAB20), exact after')
 print('NATIVE_PARTICLE_ENGINE_LIFETIME_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
