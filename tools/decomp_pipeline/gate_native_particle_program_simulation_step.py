"""Fresh VC71 parity gate: CParticleProgram SetupSimulation (0x00AE0050), Update (0x00AE34D0), ClearGroups (0x00AE3510), both SimulationStep overloads (0x00AE2CB0, 0x00AE2D20), ~CParticleSimulation (0x00AE0630) and ~CParticleProgram (0x00AE3670), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??$sort@PAU?$pair@VCCharString@@K@_STL@@V?$CKeyPairCompareLess@VCCharString@@K@@@_STL@@YIXPAU?$pair@VCCharString@@K@0@0V?$CKeyPairCompareLess@VCCharString@@K@@@Z':0x9b8ef0,
 '??1?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAE@XZ':0xae7ef0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??1CMemoryDataOutputStream@@UAE@XZ':0x996b30,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??_7CMovableResource@@6B@':0x129b860,
 '??_7CParticleProgram@@6B@':0x129f0d8,
 '??_7CParticleSimulation@@6B@':0x129f0c4,
 '?ApplyConstraints@CParticleProgram@@ABEXAAVCParticleSimulation@@JM@Z':0xae2510,
 '?ApplyDragging@CParticleSimulation@@AAEXABVCParticleContext@@@Z':0xadf300,
 '?ApplyForces@CParticleSimulation@@AAEXABVCParticleContext@@@Z':0xadf760,
 '?CalculateAveragePatchSize@CParticleProgram@@AAEXABVCParticleSimulation@@@Z':0xae0710,
 '?Clear@CParticleProgram@@QAEXXZ':0xae3360,
 '?Clear@CParticleSimulation@@QAEXXZ':0xadf1e0,
 '?ClingToMesh@CParticleProgram@@ABEXAAVCParticleSimulation@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@@Z':0xae1320,
 '?CopyFrom@CParticleSimulation@@QAE_NABV1@AAVCMovableResourceMemoryPool@@@Z':0xadf610,
 '?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KPBMKK@Z':0xa5bef0,
 '?erase@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEPAVC3DGroup2@@PAV3@0@Z':0xae7f30,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='setup_simulation',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_Setup_00ae0050.cpp',symbol='?SetupSimulation@CParticleProgram@@QBE_NAAVCParticleSimulation@@AAVCMovableResourceMemoryPool@@@Z',address=0xae0050,size=26,instructions=9,relocations=1),
 dict(name='update',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_UpdateAndClearGroups_00ae34d0.cpp',symbol='?Update@CParticleProgram@@QAEXXZ',address=0xae34d0,size=59,instructions=18,relocations=2),
 dict(name='clear_groups',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_UpdateAndClearGroups_00ae34d0.cpp',symbol='?ClearGroups@CParticleProgram@@QAEXXZ',address=0xae3510,size=55,instructions=16,relocations=2),
 dict(name='simulation_step',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_SimulationStep_00ae2cb0.cpp',symbol='?SimulationStep@CParticleProgram@@QBEXAAVCParticleSimulation@@ABVCParticleContext@@@Z',address=0xae2cb0,size=102,instructions=46,relocations=4),
 dict(name='simulation_step_cling',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_SimulationStep_00ae2cb0.cpp',symbol='?SimulationStep@CParticleProgram@@QBEXAAVCParticleSimulation@@ABVCParticleContext@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@@Z',address=0xae2d20,size=97,instructions=42,relocations=4),
 dict(name='simulation_destructor',source=R/'rebuild/src/compiled/00/ae/CParticleSimulation_Destructor_00ae0630.cpp',symbol='??1CParticleSimulation@@UAE@XZ',address=0xae0630,size=71,instructions=22,relocations=4),
 dict(name='program_destructor',source=R/'rebuild/src/compiled/00/ae/CParticleSimulation_Destructor_00ae0630.cpp',symbol='??1CParticleProgram@@UAE@XZ',address=0xae3670,size=338,instructions=101,relocations=20),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-simulation-step.json',[],
  'CParticleProgram SetupSimulation (0x00AE0050), Update (0x00AE34D0), ClearGroups (0x00AE3510), both SimulationStep overlo')
 print('NATIVE_PARTICLE_PROGRAM_SIMULATION_STEP_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
