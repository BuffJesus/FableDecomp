"""Fresh VC71 parity gate: vector<unsigned char> push_back, back_insert_iterator operator= and __copy instantiated by CMemoryFile::Copy (lib_memory_file)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_insert_overflow@?$vector@EV?$allocator@E@_STL@@@_STL@@QAEXPAEABEABU__true_type@2@I_N@Z':0x485b6c,
}
F=R/'rebuild/src/compiled/00/9e/CMemoryFile_Copy_009e9760.cpp'
ROOTS=[
 dict(name='push_back',source=F,symbol='?push_back@?$vector@EV?$allocator@E@_STL@@@_STL@@QAEXABE@Z',address=0x9e9880,size=49,instructions=19,relocations=1),
 dict(name='back_insert_assign',source=F,symbol='??4?$back_insert_iterator@V?$CArrayVector@E@@@_STL@@QAEAAV01@ABE@Z',address=0x9e98c0,size=60,instructions=26,relocations=1),
 dict(name='copy_counted',source=F,symbol='??$__copy@PBEV?$back_insert_iterator@V?$CArrayVector@E@@@_STL@@H@_STL@@YI?AV?$back_insert_iterator@V?$CArrayVector@E@@@0@PBE0V10@ABUrandom_access_iterator_tag@0@PAH@Z',address=0x9e9900,size=93,instructions=45,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-memory-file-byte-vector.json',[],
  'vector<unsigned char> push_back, back_insert_iterator operator= and __copy instantiated by CMemoryFile::Copy (lib_memory_file)')
 print('NATIVE_MEMORY_FILE_BYTE_VECTOR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
