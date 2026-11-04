"""Fresh VC71 parity gate: CParticleProgram::ExecuteProgram (0x00AE1A90, embedded jump table) and ApplyConstraints (0x00AE2510), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ExecuteProgram@CParticleProgram@@ABEXAAVCParticleSimulation@@@Z':0xae1a90,
 '?GetLength@CMemoryDataOutputStream@@QBEKXZ':0x994b40,
 '?PeekData@CMemoryDataOutputStream@@QBEPBXXZ':0x995e70,
 '?SatisfyDistanceConstraints@CParticleSimulation@@AAEXPBVCParticleDistanceConstraint@@K@Z':0xadf960,
 '?_M_insert_overflow@?$vector@UCLoopBegin@CParticleProgram@@V?$allocator@UCLoopBegin@CParticleProgram@@@_STL@@@_STL@@QAEXPAUCLoopBegin@CParticleProgram@@ABU34@ABU__false_type@2@I_N@Z':0xae64c0,
}
ROOTS=[
 dict(name='execute_program',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_ExecuteProgram_00ae1a90.cpp',symbol='?ExecuteProgram@CParticleProgram@@ABEXAAVCParticleSimulation@@@Z',address=0xae1a90,size=292,instructions=96,relocations=11,code_size=0x10a),
 dict(name='apply_constraints',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_ExecuteProgram_00ae1a90.cpp',symbol='?ApplyConstraints@CParticleProgram@@ABEXAAVCParticleSimulation@@JM@Z',address=0xae2510,size=61,instructions=27,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-execute.json',[],
  'CParticleProgram::ExecuteProgram (0x00AE1A90, embedded jump table) and ApplyConstraints (0x00AE2510), exact after reloca')
 print('NATIVE_PARTICLE_PROGRAM_EXECUTE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
