"""Fresh VC71 parity gate: C3DAnimFileBoneMaskChunk info/write/create/read/description (count + per-bone bytes)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@_NV?$allocator@_N@_STL@@@_STL@@QAEXIAB_N@Z':0x467686,  # derived from retail site; not in symbol_index
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_04EIEHPPNM@AMSK?$AA@':0x129e8f8,  # derived from retail site; not in symbol_index
 '??_7C3DAnimFileBoneMaskChunk@@6B@':0x129e980,  # derived from retail site; not in symbol_index
 '??_C@_0P@JLGAIIPD@Animation?5Mask?$AA@':0x129e9ac,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DAnimFileBoneMaskChunk_Methods_00ab0110.cpp'
ROOTS=[
 dict(name='info',source=F,symbol='?GetChunkInfo@C3DAnimFileBoneMaskChunk@@UBE?BVCCharString@@XZ',address=0xab0110,size=25,instructions=9,relocations=2),
 dict(name='write',source=F,symbol='?WriteToFile@C3DAnimFileBoneMaskChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xab06b0,size=72,instructions=32,relocations=2),
 dict(name='create',source=F,symbol='?CreateNewInstance@C3DAnimFileBoneMaskChunk@@UBEPAVCChunkedFileChunk@@XZ',address=0xab0730,size=83,instructions=31,relocations=4),
 dict(name='read',source=F,symbol='?ReadFromFile@C3DAnimFileBoneMaskChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z',address=0xab0790,size=223,instructions=90,relocations=3),
 dict(name='description',source=F,symbol='?GetChunkDescription@C3DAnimFileBoneMaskChunk@@UBE?BVCCharString@@XZ',address=0xab0b30,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-bone-mask-chunk.json',[],
  'C3DAnimFileBoneMaskChunk info/write/create/read/description (count + per-bone bytes)')
 print('NATIVE_ANIM_BONE_MASK_CHUNK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
