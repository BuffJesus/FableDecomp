"""Fresh VC71 parity gate: CMemoryDataOutputStream::CopyDataToString (SetAsSubString over the const GetAsCArray and size)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?SetAsSubString@CCharString@@QAEXPBDJ@Z':0x99ef40,
}
F=R/'rebuild/src/compiled/00/99/CMemoryDataOutputStream_CopyDataToString_00995e80.cpp'
ROOTS=[
 dict(name='copy_to_string',source=F,symbol='?CopyDataToString@CMemoryDataOutputStream@@QBEXAAVCCharString@@@Z',address=0x995e80,size=33,instructions=13,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-memory-output-copy-to-string.json',[],
  'CMemoryDataOutputStream::CopyDataToString (SetAsSubString over the const GetAsCArray and size)')
 print('NATIVE_MEMORY_OUTPUT_COPY_TO_STRING_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
