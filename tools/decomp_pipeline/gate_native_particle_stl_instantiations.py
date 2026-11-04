"""Fresh VC71 parity gate: STLport instantiations of the CParticleProgram unit: vector<CLoopBegin>::_M_insert_overflow (0x00AE64C0), erase for export pairs (0x00AE6A20), C3DGroup2 (0x00AE7F30) and C3DQuad2 (0x00AE6430), vector<C3DGroup2>::reserve (0x00AE80F0) and ~vector (0x00AE7EF0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??$__copy@PAU?$pair@VCCharString@@K@_STL@@PAU12@H@_STL@@YIPAU?$pair@VCCharString@@K@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z':0xae5910,
 '??$__copy@PAVC3DGroup2@@PAV1@H@_STL@@YIPAVC3DGroup2@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z':0xae7a20,
 '??$__uninitialized_copy@PBVC3DGroup2@@PAV1@@_STL@@YIPAVC3DGroup2@@PBV1@0PAV1@ABU__false_type@0@@Z':0xae7480,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '?_M_clear@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEXXZ':0xae7fa0,
 '_free':0xbfea14,
 '_malloc':0xbfea0e,
}
ROOTS=[
 dict(name='stlvectorloopbegin_insert_overflow',source=R/'rebuild/src/compiled/00/ae/STLVectorLoopBegin_insert_overflow_00ae64c0.cpp',symbol='?_M_insert_overflow@?$vector@UCLoopBegin@CParticleProgram@@V?$allocator@UCLoopBegin@CParticleProgram@@@_STL@@@_STL@@QAEXPAUCLoopBegin@CParticleProgram@@ABU34@ABU__false_type@2@I_N@Z',address=0xae64c0,size=259,instructions=107,relocations=2),
 dict(name='stlvectorexportparticle_erase',source=R/'rebuild/src/compiled/00/ae/STLVectorExportParticle_erase_00ae6a20.cpp',symbol='?erase@?$vector@U?$pair@VCCharString@@K@_STL@@V?$allocator@U?$pair@VCCharString@@K@_STL@@@2@@_STL@@QAEPAU?$pair@VCCharString@@K@2@PAU32@0@Z',address=0xae6a20,size=76,instructions=31,relocations=2),
 dict(name='stlvectorgroup2_erase',source=R/'rebuild/src/compiled/00/ae/STLVectorGroup2_erase_00ae7f30.cpp',symbol='?erase@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEPAVC3DGroup2@@PAV3@0@Z',address=0xae7f30,size=99,instructions=43,relocations=2),
 dict(name='stlvectorquad2_erase',source=R/'rebuild/src/compiled/00/ae/STLVectorQuad2_erase_00ae6430.cpp',symbol='?erase@?$vector@VC3DQuad2@@V?$allocator@VC3DQuad2@@@_STL@@@_STL@@QAEPAVC3DQuad2@@PAV3@0@Z',address=0xae6430,size=59,instructions=26,relocations=0),
 dict(name='stlvectorgroup2_reserve',source=R/'rebuild/src/compiled/00/ae/STLVectorGroup2_reserve_00ae80f0.cpp',symbol='?reserve@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEXI@Z',address=0xae80f0,size=163,instructions=64,relocations=4),
 dict(name='stlvectorgroup2_destructor',source=R/'rebuild/src/compiled/00/ae/STLVectorGroup2_destructor_00ae7ef0.cpp',symbol='??1?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAE@XZ',address=0xae7ef0,size=58,instructions=28,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-stl-instantiations.json',[],
  'STLport instantiations of the CParticleProgram unit: vector<CLoopBegin>::_M_insert_overflow (0x00AE64C0), erase for expo')
 print('NATIVE_PARTICLE_STL_INSTANTIATIONS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
