"""Fresh VC71 parity gate: CVertexCompressionParams Write/ReadFromFile + C3DMeshFileXVertexDataChunk write/create/read."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@DV?$allocator@D@_STL@@@_STL@@QAEXIABD@Z':0x4122d0,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteToFile@@YIXABVCVertexCompressionParams@@AAVCDataOutputStream@@@Z':0xaa4d20,  # derived from retail site; not in symbol_index
 '?ReadFromFile@@YIXAAVCVertexCompressionParams@@AAVCDataInputStream@@@Z':0xaa5460,  # derived from retail site; not in symbol_index
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04DINDAPDJ@XVBD?$AA@':0x129df40,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileXVertexDataChunk@@6B@':0x129e33c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunks_00aa4b50.cpp'
ROOTS=[
 dict(name='params_write',source=F,symbol='?WriteToFile@@YIXABVCVertexCompressionParams@@AAVCDataOutputStream@@@Z',address=0xaa4d20,size=96,instructions=39,relocations=8),
 dict(name='params_read',source=F,symbol='?ReadFromFile@@YIXAAVCVertexCompressionParams@@AAVCDataInputStream@@@Z',address=0xaa5460,size=700,instructions=265,relocations=8),
 dict(name='write',source=F,symbol='?WriteToFile@C3DMeshFileXVertexDataChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa58c0,size=109,instructions=45,relocations=6),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DMeshFileXVertexDataChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa59c0,size=105,instructions=44,relocations=4),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DMeshFileXVertexDataChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa66b0,size=592,instructions=229,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-xvertex-data-chunk.json',[],
  'CVertexCompressionParams Write/ReadFromFile + C3DMeshFileXVertexDataChunk write/create/read')
 print('NATIVE_MESH_XVERTEX_DATA_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
