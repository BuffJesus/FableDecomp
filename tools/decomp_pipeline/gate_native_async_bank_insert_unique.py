"""Fresh VC71 parity gate: Async-bank pointer-tree unique insertion."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_decrement@_Rb_tree_base_iterator@_STL@@QAEXXZ':0x9d74e0,
 '?_M_insert@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@2@PAU_Rb_tree_node_base@2@0ABQAVCBankFileAsyncData@@0@Z':0x9d7990,
}
F=R/'rebuild/src/compiled/00/9d/STLAsyncPointerTree_InsertUnique_009d7cf0.cpp'
ROOTS=[
 dict(name='insert_unique',source=F,symbol='?insert_unique@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@_N@2@ABQAVCBankFileAsyncData@@@Z',address=0x9d7cf0,size=180,instructions=78,relocations=3),
]
def main():
 from gate_native_loading_restart_tree import compare_insert_unique
 behavior=compare_insert_unique()
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-async-bank-insert-unique.json',[behavior],
  'Async-bank pointer-tree unique insertion')
 print('NATIVE_ASYNC_BANK_INSERT_UNIQUE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
