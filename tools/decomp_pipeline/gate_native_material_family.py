"""Fresh VC71 parity gate: Mesh-file material, map and list constructors and map helpers (lib_3d_mesh_file_material.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '?CountSubChunksOfType@CChunkedFileSuperChunk@@QBEKPBD@Z':0xaa7ea0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '?AddSubChunk@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z':0xaa84f0,
 '?ResetParams@C3DMeshFileMaterialChunk@@AAEXXZ':0xac0250,
 '?DeleteData@?$CCountedPointer@VC3DMeshFileMapChunk@@@@CIXPAX@Z':0xac1460,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??2@YAPAXI@Z':0xbfea1a,
 '??_C@_04DFMHHGNM@MTLS?$AA@':0x129e074,
 '??_C@_04OOCFPGDI@MTRL?$AA@':0x129edc4,
 '??_C@_04CJFPMDAL@MTLE?$AA@':0x129edcc,
 '??_C@_04DLCDKOFJ@MMAP?$AA@':0x129edd4,
 '??_7C3DMeshFileMaterialListChunk@@6B@':0x129ede0,
 '??_7C3DMeshFileMapChunk@@6B@':0x129ee34,
 '??_7C3DMeshFileMaterialExtChunk@@6B@':0x129ee90,
 '??_7C3DMeshFileMaterialChunk@@6B@':0x129eee0,
}
F0=R/'rebuild/src/compiled/00/aa/C3DFileContainerChunks_00aa9670.cpp'
F1=R/'rebuild/src/compiled/00/ac/C3DMeshFileMaterialChunk_Maps_00ac0c00.cpp'
F2=R/'rebuild/src/compiled/00/ab/C3DMeshFileMapChunk_Methods_00abfe80.cpp'
F3=R/'rebuild/src/compiled/00/ab/C3DMeshFileMaterialExtChunk_Methods_00abfd70.cpp'
F4=R/'rebuild/src/compiled/00/ab/C3DMeshFileMaterialChunk_Methods_00abfbf0.cpp'
ROOTS=[
 dict(name='material_list_ctor',source=F0,symbol='??0C3DMeshFileMaterialListChunk@@QAE@XZ',address=0xabfb90,size=23,instructions=8,relocations=3),
 dict(name='material_has_extended_properties',source=F1,symbol='?HasExtendedProperties@C3DMeshFileMaterialChunk@@QBE_NXZ',address=0xabfcd0,size=19,instructions=7,relocations=2),
 dict(name='map_ctor_type',source=F2,symbol='??0C3DMeshFileMapChunk@@QAE@W4EMaterialMapType@@@Z',address=0xabfe20,size=48,instructions=14,relocations=5),
 dict(name='map_ctor',source=F2,symbol='??0C3DMeshFileMapChunk@@QAE@XZ',address=0xabfe50,size=46,instructions=13,relocations=5),
 dict(name='material_create_map',source=F2,symbol='?CreateMap@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMapChunk@@@@W4EMaterialMapType@@@Z',address=0xac0c00,size=222,instructions=81,relocations=11),
 dict(name='material_ext_ctor',source=F3,symbol='??0C3DMeshFileMaterialExtChunk@@QAE@XZ',address=0xac02b0,size=23,instructions=8,relocations=3),
 dict(name='material_ctor_index',source=F4,symbol='??0C3DMeshFileMaterialChunk@@QAE@J@Z',address=0xac0a80,size=47,instructions=14,relocations=5),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-material-family.json',[],
  'Mesh-file material, map and list constructors and map helpers (lib_3d_mesh_file_material.cpp)')
 print('NATIVE_MATERIAL_FAMILY_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
