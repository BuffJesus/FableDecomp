"""Fresh VC71 parity gate: CAFile::CompareFiles (Ego_r locals; CArray<unsigned char> 4 KB buffers, _STL::min by reference, inline memcmp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '_malloc':0xbfea0e,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_CompareFiles_00998f60.cpp'
ROOTS=[
 dict(name='compare_files',source=F,symbol='?CompareFiles@CAFile@@SI_NAAV1@0@Z',address=0x998f60,size=323,instructions=125,relocations=6),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-compare.json',[],
  'CAFile::CompareFiles (Ego_r locals; CArray<unsigned char> 4 KB buffers, _STL::min by reference, inline memcmp)')
 print('NATIVE_FILE_COMPARE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
