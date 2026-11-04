"""Fresh VC71 parity gate: CParticleProgram::OptimiseConstraints (0x00ADFDC0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CMemoryDataInputStream@@QAE@ABVCMemoryDataOutputStream@@@Z':0x995ee0,
 '??0CMemoryDataOutputStream@@QAE@XZ':0x996af0,
 '??1CMemoryDataInputStream@@UAE@XZ':0x993b10,
 '??1CMemoryDataOutputStream@@UAE@XZ':0x996b30,
 '?Clear@CMemoryDataOutputStream@@QAEXXZ':0x996b60,
 '?CopyTo@CDataInputStream@@QAEXAAVCDataOutputStream@@K@Z':0x9964e0,
 '?GetLength@CMemoryDataOutputStream@@QBEKXZ':0x994b40,
 '?PeekData@CMemoryDataOutputStream@@QBEPBXXZ':0x995e70,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?Write@CMemoryDataOutputStream@@UAEXPBXJ@Z':0x996cc0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
}
ROOTS=[
 dict(name='optimise_constraints',source=R/'rebuild/src/compiled/00/ad/CParticleProgram_OptimiseConstraints_00adfdc0.cpp',symbol='?OptimiseConstraints@CParticleProgram@@AAEXXZ',address=0xadfdc0,size=647,instructions=206,relocations=26),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-optimise-constraints.json',[],
  'CParticleProgram::OptimiseConstraints (0x00ADFDC0), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_OPTIMISE_CONSTRAINTS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
