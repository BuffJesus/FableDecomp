"""Fresh VC71 parity gate: C3DMeshFileGroupChunk info/write/create/description + scalar deleting dtor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteUWORD@CDataOutputStream@@QAEXG@Z':0x993e80,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '??1CChunkedFileSuperChunk@@UAE@XZ':0xaa8490,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '_sprintf':0xbfeb10,  # derived from retail site; not in symbol_index
 '??_C@_04FNMOPFIH@VGRP?$AA@':0x129eaf4,  # derived from retail site; not in symbol_index
 '??_C@_0CD@CILDHKAH@Bone?5Index?3?5?$CFli?0?5Vertex?5Count?3?5?$CF@':0x129ebd0,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileGroupChunk@@6B@':0x129ecb8,  # derived from retail site; not in symbol_index
 '??_C@_0N@JJAKEOCI@Vertex?5Group?$AA@':0x129ece4,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileGroupChunk_Methods_00abae50.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DMeshFileGroupChunk@@UBE?BVCCharString@@XZ',address=0xabae50,size=98,instructions=28,relocations=5,flags=GS),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileGroupChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xabaee0,size=111,instructions=46,relocations=5,flags=GS),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileGroupChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xabc6e0,size=90,instructions=32,relocations=4,flags=GS),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DMeshFileGroupChunk@@UBE?BVCCharString@@XZ',address=0xabec50,size=25,instructions=9,relocations=2,flags=GS),
 dict(name='deleting_dtor',source=F,symbol='??_GC3DMeshFileGroupChunk@@UAEPAXI@Z',address=0xabedd0,size=54,instructions=19,relocations=4,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-group-chunk.json',[],
  'C3DMeshFileGroupChunk info/write/create/description + scalar deleting dtor')
 print('NATIVE_MESH_GROUP_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
