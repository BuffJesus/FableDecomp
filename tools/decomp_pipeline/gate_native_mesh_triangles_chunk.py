"""Fresh VC71 parity gate: C3DMeshFileTrianglesChunk info/write/create/read/description."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteUWORD@CDataOutputStream@@QAEXG@Z':0x993e80,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?resize@?$vector@VC3DTriangle2@@V?$allocator@VC3DTriangle2@@@_STL@@@_STL@@QAEXIABVC3DTriangle2@@@Z':0xa97a50,  # derived from retail site; not in symbol_index
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '_sprintf':0xbfeb10,  # derived from retail site; not in symbol_index
 '??_C@_04HLJHBOBI@TRIS?$AA@':0x129df28,  # derived from retail site; not in symbol_index
 '??_C@_0BI@LOGBGMEC@Number?5of?5triangles?3?5?$CFu?$AA@':0x129ec0c,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileTrianglesChunk@@6B@':0x129ed34,  # derived from retail site; not in symbol_index
 '??_C@_0O@IGGJHIAE@Triangle?5List?$AA@':0x129ed60,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileTrianglesChunk_Methods_00abbe90.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileTrianglesChunk@@UBE?BVCCharString@@XZ',address=0xabbe90,size=103,instructions=29,relocations=5,flags=GS),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileTrianglesChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabbf30,size=131,instructions=53,relocations=5,flags=GS),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileTrianglesChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xabc8e0,size=83,instructions=31,relocations=4,flags=GS),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileTrianglesChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xabd280,size=308,instructions=124,relocations=4,flags=GS),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileTrianglesChunk@@UBE?BVCCharString@@XZ',address=0xabec90,size=25,instructions=9,relocations=2,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-triangles-chunk.json',[],
  'C3DMeshFileTrianglesChunk info/write/create/read/description')
 print('NATIVE_MESH_TRIANGLES_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
