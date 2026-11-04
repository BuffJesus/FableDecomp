"""Fresh VC71 parity gate: CParticleProgram::WriteToFile (0x00AE2230), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetLength@CMemoryDataOutputStream@@QBEKXZ':0x994b40,
 '?PeekData@CMemoryDataOutputStream@@QBEPBXXZ':0x995e70,
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteGroups@CParticleProgram@@CIXAAVCDataOutputStream@@ABV?$CArray@VC3DGroup2@@@@@Z':0xae0680,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '?WriteToFile@CParticleSimulation@@QBEXAAVCDataOutputStream@@@Z':0xae2050,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z':0xae18c0,
}
ROOTS=[
 dict(name='program_write',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_WriteToFile_00ae2230.cpp',symbol='?WriteToFile@CParticleProgram@@QBEXAAVCDataOutputStream@@@Z',address=0xae2230,size=734,instructions=249,relocations=21),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-write.json',[],
  'CParticleProgram::WriteToFile (0x00AE2230), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_WRITE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
