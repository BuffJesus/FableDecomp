"""Fresh VC71 parity gate: C3DMeshFileClothChunk write/create/read."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@DV?$allocator@D@_STL@@@_STL@@QAEXIABD@Z':0x4122d0,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteNullTerminatedString@CDataOutputStream@@QAEXABVCCharString@@@Z':0x995ae0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04KEFJIOJP@CLTH?$AA@':0x129df30,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileClothChunk@@6B@':0x129f008,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ac/C3DMeshFileClothChunk_Methods_00ac6600.cpp'
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileClothChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xac6600,size=146,instructions=61,relocations=7),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileClothChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xac66e0,size=99,instructions=35,relocations=6),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileClothChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xac6750,size=583,instructions=223,relocations=11),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-cloth-chunk.json',[],
  'C3DMeshFileClothChunk write/create/read')
 print('NATIVE_MESH_CLOTH_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
