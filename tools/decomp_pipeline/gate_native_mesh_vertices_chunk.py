"""Fresh VC71 parity gate: C3DMeshFileVerticesChunk info/write/create/read/description (resize inlined for C3DVertex2)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?erase@?$vector@VC3DVertex2@@V?$allocator@VC3DVertex2@@@_STL@@@_STL@@QAEPAVC3DVertex2@@PAV3@0@Z':0xa96a90,  # derived from retail site; not in symbol_index
 '?_M_fill_insert@?$vector@VC3DVertex2@@V?$allocator@VC3DVertex2@@@_STL@@@_STL@@QAEXPAVC3DVertex2@@IABV3@@Z':0xa97040,  # derived from retail site; not in symbol_index
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '_sprintf':0xbfeb10,  # derived from retail site; not in symbol_index
 '??_C@_04JDKLPIAI@VERT?$AA@':0x129eafc,  # derived from retail site; not in symbol_index
 '??_C@_0BH@GIJELKMA@Number?5of?5vertices?3?5?$CFu?$AA@':0x129ebf4,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileVerticesChunk@@6B@':0x129ecf8,  # derived from retail site; not in symbol_index
 '??_C@_0M@KPPFGJOF@Vertex?5List?$AA@':0x129ed24,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileVerticesChunk_Methods_00abbd70.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileVerticesChunk@@UBE?BVCCharString@@XZ',address=0xabbd70,size=92,instructions=25,relocations=5,flags=GS),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileVerticesChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabbdf0,size=151,instructions=60,relocations=9,flags=GS),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileVerticesChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xabc800,size=83,instructions=31,relocations=4,flags=GS),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileVerticesChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xabd150,size=289,instructions=120,relocations=4,flags=GS),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileVerticesChunk@@UBE?BVCCharString@@XZ',address=0xabec70,size=25,instructions=9,relocations=2,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-vertices-chunk.json',[],
  'C3DMeshFileVerticesChunk info/write/create/read/description (resize inlined for C3DVertex2)')
 print('NATIVE_MESH_VERTICES_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
