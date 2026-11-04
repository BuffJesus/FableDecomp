"""Fresh VC71 parity gate: Mesh-file chunk setters/getters (material, map, material ext, helper point, dummy, convex volume) and the mesh helpers constructor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '?erase@?$vector@VCPlane@@V?$allocator@VCPlane@@@_STL@@@_STL@@QAEPAVCPlane@@PAV3@0@Z':0xaab500,
 '??_C@_04GEKNONIJ@HLPR?$AA@':0x129dfcc,
 '??_7C3DMeshFileHelpersChunk@@6B@':0x129e538,
}
F0=R/'rebuild/src/compiled/00/ab/C3DMeshFileMaterialChunk_Methods_00abfbf0.cpp'
F1=R/'rebuild/src/compiled/00/ab/C3DMeshFileMapChunk_Methods_00abfe80.cpp'
F2=R/'rebuild/src/compiled/00/ab/C3DMeshFileMaterialExtChunk_Methods_00abfd70.cpp'
F3=R/'rebuild/src/compiled/00/aa/C3DMeshFileHelperPointChunk_Methods_00aa96b0.cpp'
F4=R/'rebuild/src/compiled/00/aa/C3DMeshFileDummyObjectChunk_Methods_00aa9910.cpp'
F5=R/'rebuild/src/compiled/00/aa/C3DMeshFileConvexVolumeChunk_Methods_00aa9ad0.cpp'
F6=R/'rebuild/src/compiled/00/aa/C3DFileContainerChunks_00aa9670.cpp'
ROOTS=[
 dict(name='material_set_index',source=F0,symbol='?SetMaterialIndex@C3DMeshFileMaterialChunk@@QAEXJ@Z',address=0xabfc40,size=10,instructions=3,relocations=0),
 dict(name='material_set_name',source=F0,symbol='?SetName@C3DMeshFileMaterialChunk@@QAEXABVCCharString@@@Z',address=0xabfc50,size=8,instructions=2,relocations=1),
 dict(name='material_set_two_sided',source=F0,symbol='?SetTwoSided@C3DMeshFileMaterialChunk@@QAEX_N@Z',address=0xabfc60,size=10,instructions=3,relocations=0),
 dict(name='material_set_ambient',source=F0,symbol='?SetAmbient@C3DMeshFileMaterialChunk@@QAEXVCRGBColour@@@Z',address=0xabfc70,size=10,instructions=3,relocations=0),
 dict(name='material_set_diffuse',source=F0,symbol='?SetDiffuse@C3DMeshFileMaterialChunk@@QAEXVCRGBColour@@@Z',address=0xabfc80,size=10,instructions=3,relocations=0),
 dict(name='material_set_specular',source=F0,symbol='?SetSpecular@C3DMeshFileMaterialChunk@@QAEXVCRGBColour@@@Z',address=0xabfc90,size=10,instructions=3,relocations=0),
 dict(name='material_set_shiny',source=F0,symbol='?SetShiny@C3DMeshFileMaterialChunk@@QAEXM@Z',address=0xabfca0,size=10,instructions=3,relocations=0),
 dict(name='material_set_shiny_strength',source=F0,symbol='?SetShinyStrength@C3DMeshFileMaterialChunk@@QAEXM@Z',address=0xabfcb0,size=10,instructions=3,relocations=0),
 dict(name='material_set_transparency',source=F0,symbol='?SetTransparency@C3DMeshFileMaterialChunk@@QAEXM@Z',address=0xabfcc0,size=10,instructions=3,relocations=0),
 dict(name='map_set_description',source=F1,symbol='?SetMapDescription@C3DMeshFileMapChunk@@QAEXABVCCharString@@@Z',address=0xabff20,size=8,instructions=2,relocations=1),
 dict(name='map_set_filename',source=F1,symbol='?SetFilename@C3DMeshFileMapChunk@@QAEXABVCCharString@@@Z',address=0xabff30,size=8,instructions=2,relocations=1),
 dict(name='map_get_crc',source=F1,symbol='?GetCRC@C3DMeshFileMapChunk@@QBE?BKXZ',address=0xabff40,size=4,instructions=2,relocations=0),
 dict(name='map_set_crc',source=F1,symbol='?SetCRC@C3DMeshFileMapChunk@@QAEXK@Z',address=0xabff50,size=10,instructions=3,relocations=0),
 dict(name='map_set_type',source=F1,symbol='?SetType@C3DMeshFileMapChunk@@QAEXW4EMaterialMapType@@@Z',address=0xabff60,size=10,instructions=3,relocations=0),
 dict(name='ext_get_self_illumination',source=F2,symbol='?GetSelfIllumination@C3DMeshFileMaterialExtChunk@@QBEKXZ',address=0xabfdc0,size=4,instructions=2,relocations=0),
 dict(name='ext_set_self_illumination',source=F2,symbol='?SetSelfIllumination@C3DMeshFileMaterialExtChunk@@QAEXK@Z',address=0xabfdd0,size=10,instructions=3,relocations=0),
 dict(name='ext_reset_params',source=F2,symbol='?ResetParams@C3DMeshFileMaterialExtChunk@@AAEXXZ',address=0xabfe10,size=1,instructions=1,relocations=0),
 dict(name='point_set_position',source=F3,symbol='?SetPosition@C3DMeshFileHelperPointChunk@@QAEXABVC3DVector@@@Z',address=0xaa9860,size=26,instructions=9,relocations=0),
 dict(name='point_set_submesh',source=F3,symbol='?SetSubMeshIndex@C3DMeshFileHelperPointChunk@@QAEXJ@Z',address=0xaa9880,size=10,instructions=3,relocations=0),
 dict(name='point_set_bone',source=F3,symbol='?SetBoneIndex@C3DMeshFileHelperPointChunk@@QAEXJ@Z',address=0xaa9890,size=10,instructions=3,relocations=0),
 dict(name='point_set_name',source=F3,symbol='?SetName@C3DMeshFileHelperPointChunk@@QAEXABVCCharString@@@Z',address=0xaa98a0,size=8,instructions=2,relocations=1),
 dict(name='dummy_set_direction',source=F4,symbol='?SetDirection@C3DMeshFileDummyObjectChunk@@QAEXABVC3DVector@@@Z',address=0xaa9a20,size=26,instructions=9,relocations=0),
 dict(name='dummy_set_transform',source=F4,symbol='?SetTransform@C3DMeshFileDummyObjectChunk@@QAEXABVCMatrix3x4@@@Z',address=0xaa9a40,size=21,instructions=9,relocations=0),
 dict(name='dummy_set_local_origin',source=F4,symbol='?SetUseLocalOrigin@C3DMeshFileDummyObjectChunk@@QAEX_N@Z',address=0xaa9a60,size=13,instructions=3,relocations=0),
 dict(name='convex_set_name',source=F5,symbol='?SetName@C3DMeshFileConvexVolumeChunk@@QAEXABVCCharString@@@Z',address=0xaa9b10,size=8,instructions=2,relocations=1),
 dict(name='convex_clear_planes',source=F5,symbol='?ClearPlanes@C3DMeshFileConvexVolumeChunk@@QAEXXZ',address=0xaaaa40,size=17,instructions=7,relocations=1),
 dict(name='mesh_helpers_ctor',source=F6,symbol='??0C3DMeshFileHelpersChunk@@QAE@XZ',address=0xaa9650,size=23,instructions=8,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-chunk-accessors.json',[],
  'Mesh-file chunk setters/getters (material, map, material ext, helper point, dummy, convex volume) and the mesh helpers constructor')
 print('NATIVE_CHUNK_ACCESSORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
