"""Fresh VC71 parity gate: C3DAnimFileSequenceChunk SetBoneName/SetBoneIndex/SetParentIndex/SetSamplesPerSecond (0x00AAF570..0x00AAF5A0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
}
ROOTS=[
 dict(name='set_bone_name',source=R/'rebuild/src/compiled/00/aa/C3DAnimFileSequenceChunk_Setters_00aaf570.cpp',symbol='?SetBoneName@C3DAnimFileSequenceChunk@@QAEXABVCCharString@@@Z',address=0xaaf570,size=8,instructions=2,relocations=1),
 dict(name='set_bone_index',source=R/'rebuild/src/compiled/00/aa/C3DAnimFileSequenceChunk_Setters_00aaf570.cpp',symbol='?SetBoneIndex@C3DAnimFileSequenceChunk@@QAEXJ@Z',address=0xaaf580,size=10,instructions=3,relocations=0),
 dict(name='set_parent_index',source=R/'rebuild/src/compiled/00/aa/C3DAnimFileSequenceChunk_Setters_00aaf570.cpp',symbol='?SetParentIndex@C3DAnimFileSequenceChunk@@QAEXJ@Z',address=0xaaf590,size=10,instructions=3,relocations=0),
 dict(name='set_samples_per_second',source=R/'rebuild/src/compiled/00/aa/C3DAnimFileSequenceChunk_Setters_00aaf570.cpp',symbol='?SetSamplesPerSecond@C3DAnimFileSequenceChunk@@QAEXM@Z',address=0xaaf5a0,size=10,instructions=3,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-sequence-setters.json',[],
  'C3DAnimFileSequenceChunk SetBoneName/SetBoneIndex/SetParentIndex/SetSamplesPerSecond (0x00AAF570..0x00AAF5A0), exact aft')
 print('NATIVE_ANIM_SEQUENCE_SETTERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
