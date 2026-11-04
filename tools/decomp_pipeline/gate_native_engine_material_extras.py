"""Fresh VC71 parity gate: C3DMeshFileEngineMaterialChunk map filenames, flags and writer (lib_3d_mesh_file_engine_material.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,
 '??1CMapFilenames@C3DMeshFileEngineMaterialChunk@@UAE@XZ':0xac6210,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '??_7CIVCountedPointeeBase@@6B@':0x12354d0,
 '??_7CMapFilenames@C3DMeshFileEngineMaterialChunk@@6B@':0x129efbc,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ac/C3DMeshFileEngineMaterialChunk_Methods_00ac51d0.cpp'
ROOTS=[
 dict(name='map_filenames_ctor',source=F,symbol='??0CMapFilenames@C3DMeshFileEngineMaterialChunk@@QAE@XZ',address=0xac5190,size=52,instructions=15,relocations=5),
 dict(name='calculate_map_flags',source=F,symbol='?CalculateMapFlags@C3DMeshFileEngineMaterialChunk@@AAEXXZ',address=0xac5260,size=135,instructions=50,relocations=0),
 dict(name='write_to_file',source=F,symbol='?WriteToFile@C3DMeshFileEngineMaterialChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xac52f0,size=256,instructions=96,relocations=20),
 dict(name='remove_map_filenames',source=F,symbol='?RemoveMapFilenames@C3DMeshFileEngineMaterialChunk@@QAEXXZ',address=0xac5740,size=36,instructions=13,relocations=0),
 dict(name='map_filenames_deleting_dtor',source=F,symbol='??_GCMapFilenames@C3DMeshFileEngineMaterialChunk@@UAEPAXI@Z',address=0xac61f0,size=30,instructions=11,relocations=2),
 dict(name='map_filenames_dtor',source=F,symbol='??1CMapFilenames@C3DMeshFileEngineMaterialChunk@@UAE@XZ',address=0xac6210,size=43,instructions=13,relocations=5),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-engine-material-extras.json',[],
  'C3DMeshFileEngineMaterialChunk map filenames, flags and writer (lib_3d_mesh_file_engine_material.cpp)')
 print('NATIVE_ENGINE_MATERIAL_EXTRAS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
