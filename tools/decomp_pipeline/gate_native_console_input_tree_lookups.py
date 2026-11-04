"""Fresh VC71 parity gate: RegisteredConsoleInputs tree lookups (STLport _M_lower_bound / _M_find, CCharString operator< expanded)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??M?$CBasicString@D@@QBE_NABV0@@Z':0x429950,
}
F=R/'rebuild/src/compiled/00/9e/STLConsoleInputTree_Lookups_009eec10.cpp'
ROOTS=[
 dict(name='lower_bound',source=F,symbol='?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@@_STL@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@2@ABVCCharString@@@Z',address=0x9eec10,size=65,instructions=31,relocations=1),
 dict(name='find',source=F,symbol='?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@@_STL@@QAEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z',address=0x9eed20,size=112,instructions=54,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-input-tree-lookups.json',[],
  'RegisteredConsoleInputs tree lookups (STLport _M_lower_bound / _M_find, CCharString operator< expanded)')
 print('NATIVE_CONSOLE_INPUT_TREE_LOOKUPS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
