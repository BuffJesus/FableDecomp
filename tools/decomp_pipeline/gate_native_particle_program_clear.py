"""Fresh VC71 parity gate: CParticleProgram::Clear (0x00AE3360), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??$__copy@PAVC3DTriangle2@@PAV1@H@_STL@@YIPAVC3DTriangle2@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z':0xa744b0,
 '?Clear@CMemoryDataOutputStream@@QAEXXZ':0x996b60,
 '?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z':0xa62ed0,
 '?erase@?$vector@U?$pair@VCCharString@@K@_STL@@V?$allocator@U?$pair@VCCharString@@K@_STL@@@2@@_STL@@QAEPAU?$pair@VCCharString@@K@2@PAU32@0@Z':0xae6a20,
 '?erase@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEPAVC3DGroup2@@PAV3@0@Z':0xae7f30,
 '_memmove':0xbfeae6,
}
ROOTS=[
 dict(name='program_clear',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_Clear_00ae3360.cpp',symbol='?Clear@CParticleProgram@@QAEXXZ',address=0xae3360,size=360,instructions=112,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-clear.json',[],
  'CParticleProgram::Clear (0x00AE3360), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_CLEAR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
