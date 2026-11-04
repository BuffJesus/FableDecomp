"""Fresh VC71 parity gate: CChunkedFileChunk / CChunkedFileSuperChunk base virtuals, destructors and the sub-chunk writer."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?RemoveAllSubChunks@CChunkedFileSuperChunk@@QAEXXZ':0xaa8400,  # derived from retail site; not in symbol_index
 '??1CChunkedFileSuperChunk@@UAE@XZ':0xaa8490,  # derived from retail site; not in symbol_index
 '??$__destroy_aux@PAV?$CCountedPointer@VCChunkedFileChunk@@@@@_STL@@YIXPAV?$CCountedPointer@VCChunkedFileChunk@@@@0ABU__false_type@0@@Z':0xaa9140,  # derived from retail site; not in symbol_index
 '??4?$vector@V?$CCountedPointer@VCChunkedFileChunk@@@@V?$allocator@V?$CCountedPointer@VCChunkedFileChunk@@@@@_STL@@@_STL@@QAEAAV01@ABV01@@Z':0xaa9330,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_7CChunkedFileChunk@@6B@':0x129e474,  # derived from retail site; not in symbol_index
 '??_C@_0BD@LNBNNPCI@Unknown?5chunk?5type?$AA@':0x129e4a0,  # derived from retail site; not in symbol_index
 '??_7CChunkedFileSuperChunk@@6B@':0x129e4f0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/aa/CChunkedFileChunk_Base_00aa75d0.cpp'
ROOTS=[
 dict(name='description',source=F,symbol='?GetChunkDescription@CChunkedFileChunk@@UBE?BVCCharString@@XZ',address=0xaa75d0,size=25,instructions=9,relocations=2),
 dict(name='info',source=F,symbol='?GetChunkInfo@CChunkedFileChunk@@UBE?BVCCharString@@XZ',address=0xaa75f0,size=25,instructions=9,relocations=2),
 dict(name='unknown',source=F,symbol='?IsUnknownType@CChunkedFileChunk@@UBEHXZ',address=0xaa7610,size=3,instructions=2,relocations=0),
 dict(name='is_super',source=F,symbol='?IsSuperChunk@CChunkedFileChunk@@UBEHXZ',address=0xaa7620,size=3,instructions=2,relocations=0),
 dict(name='as_super',source=F,symbol='?GetAsSuperChunk@CChunkedFileChunk@@UAEPAVCChunkedFileSuperChunk@@XZ',address=0xaa7630,size=3,instructions=2,relocations=0),
 dict(name='as_super_const',source=F,symbol='?GetAsSuperChunk@CChunkedFileChunk@@UBEPBVCChunkedFileSuperChunk@@XZ',address=0xaa7640,size=3,instructions=2,relocations=0),
 dict(name='init_new',source=F,symbol='?InitNewInstance@CChunkedFileChunk@@MBEPAV1@PAV1@@Z',address=0xaa7690,size=7,instructions=2,relocations=0),
 dict(name='super_is_super',source=F,symbol='?IsSuperChunk@CChunkedFileSuperChunk@@UBEHXZ',address=0xaa76a0,size=6,instructions=2,relocations=0),
 dict(name='super_as_super',source=F,symbol='?GetAsSuperChunk@CChunkedFileSuperChunk@@UAEPAV1@XZ',address=0xaa76b0,size=3,instructions=2,relocations=0),
 dict(name='super_as_super_const',source=F,symbol='?GetAsSuperChunk@CChunkedFileSuperChunk@@UBEPBV1@XZ',address=0xaa76c0,size=3,instructions=2,relocations=0),
 dict(name='super_init_new',source=F,symbol='?InitNewInstance@CChunkedFileSuperChunk@@MBEPAVCChunkedFileChunk@@PAV2@@Z',address=0xaa8460,size=36,instructions=16,relocations=1),
 dict(name='super_write',source=F,symbol='?WriteToFile@CChunkedFileSuperChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z',address=0xaa78c0,size=131,instructions=60,relocations=2),
 dict(name='chunk_dtor',source=F,symbol='??1CChunkedFileChunk@@UAE@XZ',address=0xaa75c0,size=11,instructions=2,relocations=2),
 dict(name='chunk_deleting_dtor',source=F,symbol='??_GCChunkedFileChunk@@UAEPAXI@Z',address=0xaa8d50,size=36,instructions=12,relocations=3),
 dict(name='super_dtor',source=F,symbol='??1CChunkedFileSuperChunk@@UAE@XZ',address=0xaa8490,size=95,instructions=33,relocations=8),
 dict(name='super_deleting_dtor',source=F,symbol='??_GCChunkedFileSuperChunk@@UAEPAXI@Z',address=0xaa9600,size=30,instructions=11,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-chunked-file-base.json',[],
  'CChunkedFileChunk / CChunkedFileSuperChunk base virtuals, destructors and the sub-chunk writer')
 print('NATIVE_CHUNKED_FILE_BASE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
