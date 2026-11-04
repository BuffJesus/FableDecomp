"""Fresh VC71 parity gate: CConsole binding commands: static ConsoleRunScript/AddKeyBinding/AddStringBinding/RunStringBinding and the private AddKeyBinding/AddStringBinding/RunStringBinding they expand (out-of-line copies 9ECB00-9ECB40)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CWideString@@QAE@XZ':0x99b510,
 '?ToWideString@CCharString@@QBE?AVCWideString@@XZ':0x99e480,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '?GetAsLowerCase@CCharString@@QBE?AV1@XZ':0x99ed10,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??YCCharString@@QAEABV0@PBD@Z':0x99f100,
 '?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V1@@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@V1@@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V1@@_STL@@@3@@_STL@@QAEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z':0x9ab5d0,  # derived from retail site; not in symbol_index
 '??A?$map@VCCharString@@V1@U?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@_STL@@@3@@_STL@@QAEAAVCCharString@@ABV2@@Z':0x9ac700,  # derived from retail site; not in symbol_index
 '?RunTextCommand@CConsole@@QAEXABVCCharString@@@Z':0x9ec710,
 '?RunScript@CConsole@@QAEXABVCWideString@@@Z':0x9ec890,
 '??A?$map@DVCCharString@@U?$less@D@_STL@@V?$allocator@U?$pair@$$CBDVCCharString@@@_STL@@@3@@_STL@@QAEAAVCCharString@@ABD@Z':0x9efcf0,  # derived from retail site; not in symbol_index
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
 '??_C@_04PHHEBHMK@?4ini?$AA@':0x129c5f4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CConsole_BindingCommands_009ecb70.cpp'
ROOTS=[
 dict(name='run_script',source=F,symbol='?ConsoleRunScript@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9ecb70,size=255,instructions=94,relocations=14),
 dict(name='add_key_binding',source=F,symbol='?ConsoleAddKeyBinding@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9ecc70,size=111,instructions=39,relocations=7),
 dict(name='add_string_binding',source=F,symbol='?ConsoleAddStringBinding@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9ecce0,size=88,instructions=31,relocations=6),
 dict(name='run_string_binding',source=F,symbol='?ConsoleRunStringBinding@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9ecd40,size=63,instructions=26,relocations=4),
 dict(name='add_key_binding_member',source=F,symbol='?AddKeyBinding@CConsole@@AAEXDABVCCharString@@@Z',address=0x9ecb00,size=28,instructions=9,relocations=2),
 dict(name='add_string_binding_member',source=F,symbol='?AddStringBinding@CConsole@@AAEXABVCCharString@@0@Z',address=0x9ecb20,size=28,instructions=9,relocations=2),
 dict(name='run_string_binding_member',source=F,symbol='?RunStringBinding@CConsole@@AAEXABVCCharString@@@Z',address=0x9ecb40,size=39,instructions=17,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-binding-commands.json',[],
  'CConsole binding commands: static ConsoleRunScript/AddKeyBinding/AddStringBinding/RunStringBinding and the private AddKeyBinding/AddStringBinding/RunStringBinding they expand (out-of-line copies 9ECB00-9ECB40)')
 print('NATIVE_CONSOLE_BINDING_COMMANDS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
