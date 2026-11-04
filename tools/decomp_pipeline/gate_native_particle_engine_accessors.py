"""Fresh VC71 parity gate: NParticleEngine accessors 0x00AEA9C0-0x00AEAB10 (IsRenderingEnabled, SetTickRate, GetTickRate, GetOneOverTickRate, GetLODDistance, GetFadeOut/InEnd/InStart distances, GetMaxDraw/Spawn distances, SetMeshGraphicBank, GetGraphicBank, GetMeshBank, Set/GetRandomSeed, singleton getters), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?OneOverTickRate@NParticleEngine@@3MA':0x1399508,
 '?PEmitterDatabase@NParticleEngine@@3V?$CCountedPointer@VCParticleEmitterDatabase@NParticleEngine@@@@A':0x13d2e14,
 '?PEmitterManager@NParticleEngine@@3V?$CCountedPointer@VCParticleEmitterManager@NParticleEngine@@@@A':0x13d2e1c,
 '?PEnvironmentInteraction@NParticleEngine@@3V?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@A':0x13d2e2c,
 '?PGraphicDataBank@NParticleEngine@@3PBVCGraphicDataBank@@B':0x13d2e08,
 '?PMeshDataBank@NParticleEngine@@3PBVCMeshDataBank@@B':0x13d2e0c,
 '?PMeshGraphicDataBank@NParticleEngine@@3PBVCGraphicDataBank@@B':0x13d2e04,
 '?PPSCManager@NParticleEngine@@3V?$CCountedPointer@VCPSCManager@NParticleEngine@@@@A':0x13d2e24,
 '?RandomSeed@NParticleEngine@@3KA':0x13d2e10,
 '?RenderingEnabled@NParticleEngine@@3_NA':0x139950c,
 '?TickRate@NParticleEngine@@3JA':0x1399504,
 '_FADE_IN_END_DISTANCE':0x129f198,
 '_FADE_OUT_START_DISTANCE':0x129f19c,
 '_LOD_DISTANCE':0x1399518,
 '_MAX_DRAW_DISTANCE':0x1399514,
 '_MAX_SPAWN_DISTANCE':0x1399510,
 '__real@3f800000':0x122ded8,
}
ROOTS=[
 dict(name='particle_engine_is_rendering_enabled',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?IsRenderingEnabled@NParticleEngine@@YI_NXZ',address=0xaea9c0,size=6,instructions=2,relocations=1),
 dict(name='particle_engine_set_tick_rate',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?SetTickRate@NParticleEngine@@YIXJ@Z',address=0xaea9d0,size=27,instructions=8,relocations=3),
 dict(name='particle_engine_get_tick_rate',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetTickRate@NParticleEngine@@YIJXZ',address=0xaea9f0,size=6,instructions=2,relocations=1),
 dict(name='particle_engine_get_one_over_tick_rate',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetOneOverTickRate@NParticleEngine@@YIMXZ',address=0xaeaa00,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_get_lod_distance',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetLODDistance@NParticleEngine@@YIMXZ',address=0xaeaa10,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_get_fade_out_start_distance',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetFadeOutStartDistance@NParticleEngine@@YIMXZ',address=0xaeaa20,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_get_fade_in_end_distance',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetFadeInEndDistance@NParticleEngine@@YIMXZ',address=0xaeaa30,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_get_fade_in_start_distance',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetFadeInStartDistance@NParticleEngine@@YIMXZ',address=0xaeaa40,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_get_max_draw_distance',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetMaxDrawDistance@NParticleEngine@@YIMXZ',address=0xaeaa50,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_get_max_spawn_distance',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetMaxSpawnDistance@NParticleEngine@@YIMXZ',address=0xaeaa60,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_set_mesh_graphic_bank',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?SetMeshGraphicBank@NParticleEngine@@YIXABVCGraphicDataBank@@@Z',address=0xaeaa70,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_get_graphic_bank',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetGraphicBank@NParticleEngine@@YIABVCGraphicDataBank@@XZ',address=0xaeaaa0,size=6,instructions=2,relocations=1),
 dict(name='particle_engine_get_mesh_bank',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetMeshBank@NParticleEngine@@YIABVCMeshDataBank@@XZ',address=0xaeaab0,size=6,instructions=2,relocations=1),
 dict(name='particle_engine_set_random_seed',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?SetRandomSeed@NParticleEngine@@YIXK@Z',address=0xaeaac0,size=7,instructions=2,relocations=1),
 dict(name='particle_engine_get_random_seed',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetRandomSeed@NParticleEngine@@YIAAKXZ',address=0xaeaad0,size=6,instructions=2,relocations=1),
 dict(name='particle_engine_get_particle_emitter_database',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetParticleEmitterDatabase@NParticleEngine@@YIAAVCParticleEmitterDatabase@1@XZ',address=0xaeaae0,size=6,instructions=2,relocations=1),
 dict(name='particle_engine_get_particle_emitter_manager',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ',address=0xaeaaf0,size=6,instructions=2,relocations=1),
 dict(name='particle_engine_get_psc_manager',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetPSCManager@NParticleEngine@@YIAAVCPSCManager@1@XZ',address=0xaeab00,size=6,instructions=2,relocations=1),
 dict(name='particle_engine_get_environment_interaction',source=R/'rebuild/src/compiled/00/ae/NParticleEngine_Accessors_00aea9c0.cpp',symbol='?GetEnvironmentInteraction@NParticleEngine@@YIAAVCAEnvironmentInteraction@1@XZ',address=0xaeab10,size=6,instructions=2,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-engine-accessors.json',[],
  'NParticleEngine accessors 0x00AEA9C0-0x00AEAB10 (IsRenderingEnabled, SetTickRate, GetTickRate, GetOneOverTickRate, GetLO')
 print('NATIVE_PARTICLE_ENGINE_ACCESSORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
