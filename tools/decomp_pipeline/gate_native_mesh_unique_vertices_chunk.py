"""Fresh VC71 parity gate: C3DMeshFileUniqueVerticesChunk write/create/read/description + scalar deleting dtor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z':0x464ace,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?reserve@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXI@Z':0x9b4030,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '??1CChunkedFileChunk@@UAE@XZ':0xaa75c0,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '_memmove':0xbfeae6,  # derived from retail site; not in symbol_index
 '??_C@_04CBEPCLMK@UNIV?$AA@':0x129eb0c,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileUniqueVerticesChunk@@6B@':0x129ec6c,  # derived from retail site; not in symbol_index
 '??_C@_0BK@FIGDGANF@Unique?5Vertex?5Information?$AA@':0x129ec98,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileUniqueVerticesChunk_Methods_00abc040.cpp'
ROOTS=[
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileUniqueVerticesChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabc040,size=59,instructions=28,relocations=2),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileUniqueVerticesChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xabc5f0,size=83,instructions=31,relocations=4),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileUniqueVerticesChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xabcaf0,size=289,instructions=114,relocations=5),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileUniqueVerticesChunk@@UBE?BVCCharString@@XZ',address=0xabeab0,size=25,instructions=9,relocations=2),
 dict(name='deleting_dtor',source=F,symbol='??_GC3DMeshFileUniqueVerticesChunk@@UAEPAXI@Z',address=0xabecf0,size=54,instructions=19,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-unique-vertices-chunk.json',[],
  'C3DMeshFileUniqueVerticesChunk write/create/read/description + scalar deleting dtor')
 print('NATIVE_MESH_UNIQUE_VERTICES_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
