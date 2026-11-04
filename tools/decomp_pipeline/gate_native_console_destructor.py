"""Fresh VC71 parity gate: CConsole destructor (maps cleared, registered inputs Reset(0), history cleared, then owning member destructors and the polymorphic CTBaseSingleton<CConsole> base) and the singleton base's scalar deleting destructor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V1@@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@V1@@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V1@@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V1@@_STL@@@2@@Z':0x9aa830,  # derived from retail site; not in symbol_index
 '?_M_erase@?$_Rb_tree@DU?$pair@$$CBDVCCharString@@@_STL@@U?$_Select1st@U?$pair@$$CBDVCCharString@@@_STL@@@2@U?$less@D@2@V?$allocator@U?$pair@$$CBDVCCharString@@@_STL@@@2@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBDVCCharString@@@_STL@@@2@@Z':0x9eecc0,  # derived from retail site; not in symbol_index
 '?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@2@@Z':0x9ef6e0,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??_7?$CTBaseSingleton@VCConsole@@@@6B@':0x129c44c,  # derived from retail site; not in symbol_index
 '??_7CConsole@@6B@':0x129c600,
}
F=R/'rebuild/src/compiled/00/9e/CConsole_Destructor_009ecf30.cpp'
ROOTS=[
 dict(name='destructor',source=F,symbol='??1CConsole@@UAE@XZ',address=0x9ecf30,size=606,instructions=232,relocations=20),
 dict(name='singleton_scalar_deleting_dtor',source=F,symbol='??_G?$CTBaseSingleton@VCConsole@@@@UAEPAXI@Z',address=0x9edb70,size=31,instructions=11,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-destructor.json',[],
  "CConsole destructor (maps cleared, registered inputs Reset(0), history cleared, then owning member destructors and the polymorphic CTBaseSingleton<CConsole> base) and the singleton base's scalar deleting destructor")
 print('NATIVE_CONSOLE_DESTRUCTOR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
