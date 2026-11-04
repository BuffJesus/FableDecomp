"""Fresh VC71 parity gate: C3DMeshFileXTextureIndexChunk write/create; C3DMeshFileXMappingInfoChunk write/create/read."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_04IBKIFCME@XMIF?$AA@':0x129df48,  # derived from retail site; not in symbol_index
 '??_C@_04JOIPCIFD@XTID?$AA@':0x129e1ec,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileXTextureIndexChunk@@6B@':0x129e1f8,  # derived from retail site; not in symbol_index
 '??_7C3DMeshFileXMappingInfoChunk@@6B@':0x129e238,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunks_00aa4b50.cpp'
ROOTS=[
 dict(name='xtid_write',source=F,symbol='?WriteToFile@C3DMeshFileXTextureIndexChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa4af0,size=16,instructions=5,relocations=1),
 dict(name='xtid_create',source=F,symbol='?CreateNewInstance@C3DMeshFileXTextureIndexChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa4b00,size=69,instructions=29,relocations=4),
 dict(name='xmif_write',source=F,symbol='?WriteToFile@C3DMeshFileXMappingInfoChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa4c00,size=33,instructions=14,relocations=2),
 dict(name='xmif_create',source=F,symbol='?CreateNewInstance@C3DMeshFileXMappingInfoChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xaa4c30,size=69,instructions=29,relocations=4),
 dict(name='xmif_read',source=F,symbol='?ReadFromFile@C3DMeshFileXMappingInfoChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xaa52b0,size=182,instructions=71,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-x-leaf-chunks.json',[],
  'C3DMeshFileXTextureIndexChunk write/create; C3DMeshFileXMappingInfoChunk write/create/read')
 print('NATIVE_MESH_X_LEAF_CHUNKS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
