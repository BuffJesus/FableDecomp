"""Fresh VC71 parity gate: CPhysicsFacesSegmentBranch/Leaf HasChildren, Sizeof, GetNoFaces and destructors (face partition tree), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??_7CIPhysicsFacesSegmentBase@@6B@':0x129de70,
 '??_7CPhysicsFacesSegmentBranch@@6B@':0x129de88,
 '??_7CPhysicsFacesSegmentLeaf@@6B@':0x129dea4,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='cphysicsfacessegmentbranch_haschildren',source=R/'rebuild/src/compiled/00/a8/CPhysicsFacesSegment_Members_00a85fb0.cpp',symbol='?HasChildren@CPhysicsFacesSegmentBranch@@MBE_NXZ',address=0xa86f10,size=3,instructions=2,relocations=0),
 dict(name='cphysicsfacessegmentbranch_sizeof',source=R/'rebuild/src/compiled/00/a8/CPhysicsFacesSegment_Members_00a85fb0.cpp',symbol='?Sizeof@CPhysicsFacesSegmentBranch@@MBEJXZ',address=0xa85fb0,size=29,instructions=14,relocations=0),
 dict(name='cphysicsfacessegmentbranch_getnofaces',source=R/'rebuild/src/compiled/00/a8/CPhysicsFacesSegment_Members_00a85fb0.cpp',symbol='?GetNoFaces@CPhysicsFacesSegmentBranch@@MBEJXZ',address=0xa85fd0,size=27,instructions=14,relocations=0),
 dict(name='cphysicsfacessegmentbranch_destructor',source=R/'rebuild/src/compiled/00/a8/CPhysicsFacesSegment_Members_00a85fb0.cpp',symbol='??1CPhysicsFacesSegmentBranch@@MAE@XZ',address=0xa85ff0,size=59,instructions=20,relocations=2),
 dict(name='cphysicsfacessegmentleaf_haschildren',source=R/'rebuild/src/compiled/00/a8/CPhysicsFacesSegment_Members_00a85fb0.cpp',symbol='?HasChildren@CPhysicsFacesSegmentLeaf@@MBE_NXZ',address=0xa87110,size=3,instructions=2,relocations=0),
 dict(name='cphysicsfacessegmentleaf_sizeof',source=R/'rebuild/src/compiled/00/a8/CPhysicsFacesSegment_Members_00a85fb0.cpp',symbol='?Sizeof@CPhysicsFacesSegmentLeaf@@MBEJXZ',address=0xa86100,size=13,instructions=5,relocations=0),
 dict(name='cphysicsfacessegmentleaf_getnofaces',source=R/'rebuild/src/compiled/00/a8/CPhysicsFacesSegment_Members_00a85fb0.cpp',symbol='?GetNoFaces@CPhysicsFacesSegmentLeaf@@MBEJXZ',address=0xa860f0,size=9,instructions=4,relocations=0),
 dict(name='cphysicsfacessegmentleaf_destructor',source=R/'rebuild/src/compiled/00/a8/CPhysicsFacesSegment_Members_00a85fb0.cpp',symbol='??1CPhysicsFacesSegmentLeaf@@MAE@XZ',address=0xa87140,size=33,instructions=12,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-physics-faces-segment.json',[],
  'CPhysicsFacesSegmentBranch/Leaf HasChildren, Sizeof, GetNoFaces and destructors (face partition tree), exact after reloc')
 print('NATIVE_PHYSICS_FACES_SEGMENT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
