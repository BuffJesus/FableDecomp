"""Fresh VC71 parity gate: CBankFile::GetTotalDataSize (EntryExists and GetEntryDataSize expanded; FABLE_BANK_FILE_ENTRY_DATA_SIZE_INLINE)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??ACPackedUIntArray@@QBEKK@Z':0xa629f0,
}
F=R/'rebuild/src/compiled/00/9c/CBankFile_EntryExists_009cc190.cpp'
ROOTS=[
 dict(name='total_data_size',source=F,symbol='?GetTotalDataSize@CBankFile@@QBEKXZ',address=0x9cc450,size=112,instructions=51,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-total-data-size.json',[],
  'CBankFile::GetTotalDataSize (EntryExists and GetEntryDataSize expanded; FABLE_BANK_FILE_ENTRY_DATA_SIZE_INLINE)')
 print('NATIVE_BANK_TOTAL_DATA_SIZE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
