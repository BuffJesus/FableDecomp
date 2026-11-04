"""Fresh VC71 parity gate: C3DMeshFileXBoundingBox/XBoundingSphere/XAnimatedPrimitive chunks write/create/read."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z':0xaa78c0,  # derived from retail site; not in symbol_index
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,  # derived from retail site; not in symbol_index
 '?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z':0xaa8830,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04OGKGDHNF@XBDS?$AA@':0x129cef0,  # derived from retail site; not in symbol_index
 '??_C@_04LFHPBEMF@XBDB?$AA@':0x129cef8,  # derived from retail site; not in symbol_index
 '??_C@_04PGCHKCNG@XAPR?$AA@':0x129df50,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileXAnimatedPrimitiveChunk@@6B@':0x129e278,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileXBoundingSphereChunk@@6B@':0x129e2bc,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileXBoundingBoxChunk@@6B@':0x129e2fc,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunks_00aa4b50.cpp'
ROOTS=[
 dict(name='xbds_write',source=F,symbol='?WriteToFile@C3DMeshFileXBoundingSphereChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa4b50,size=57,instructions=23,relocations=4),
 dict(name='xapr_write',source=F,symbol='?WriteToFile@C3DMeshFileXAnimatedPrimitiveChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa4ca0,size=48,instructions=20,relocations=3),
 dict(name='xapr_create',source=F,symbol='?CreateNewInstance@C3DMeshFileXAnimatedPrimitiveChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa4cd0,size=76,instructions=30,relocations=4),
 dict(name='xbds_read',source=F,symbol='?ReadFromFile@C3DMeshFileXBoundingSphereChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa4e40,size=364,instructions=138,relocations=4),
 dict(name='xbds_create',source=F,symbol='?CreateNewInstance@C3DMeshFileXBoundingSphereChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa4fb0,size=79,instructions=36,relocations=4),
 dict(name='xapr_read',source=F,symbol='?ReadFromFile@C3DMeshFileXAnimatedPrimitiveChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa5370,size=225,instructions=89,relocations=3),
 dict(name='xbdb_write',source=F,symbol='?WriteToFile@C3DMeshFileXBoundingBoxChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa4b90,size=79,instructions=31,relocations=6),
 dict(name='xbdb_read',source=F,symbol='?ReadFromFile@C3DMeshFileXBoundingBoxChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa5030,size=534,instructions=200,relocations=6),
 dict(name='xbdb_create',source=F,symbol='?CreateNewInstance@C3DMeshFileXBoundingBoxChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa5250,size=85,instructions=38,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-x-chunks.json',[],
  'C3DMeshFileXBoundingBox/XBoundingSphere/XAnimatedPrimitive chunks write/create/read')
 print('NATIVE_MESH_X_CHUNKS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
