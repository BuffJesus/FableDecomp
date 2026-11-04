"""Fresh VC71 parity gate: CConsoleVariable::ProcessCommandLine (Ego_r locals parsed_item/parser/var_val_as_string/help_message; single exit; explicit CCharString("") parser filename temporary)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1?$list@VCStringParserCommentBoundaries@@@std@@QAE@XZ':0x4046b0,
 '??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z':0x404720,
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??YCCharString@@QAEABV0@PBD@Z':0x99f100,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '??H@YI?AVCCharString@@PBDABV0@@Z':0x99f690,
 '?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z':0x9ba080,
 '?NextItemExists@CStringParser@@QAE_NXZ':0x9ba4f0,
 '?SendMessageToUser@CConsole@@QAEXABVCCharString@@@Z':0x9eb260,
 '?PrintToString@CGenericVarRef@@QAEXAAVCCharString@@@Z':0xa63c00,
 '?ReadFromParser@CGenericVarRef@@QAE_NAAVCStringParser@@@Z':0xa63fc0,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_02LMMGGCAJ@?3?5?$AA@':0x12780a8,
 '??_C@_03MHLONNLM@?5?$DN?5?$AA@':0x129b6a8,  # derived from retail site; not in symbol_index
 '??_C@_06ICGJLFIM@string?$AA@':0x129c41c,
 '??_C@_05JEPJEHGA@slong?$AA@':0x129c424,
 '??_C@_05KNFBNCOH@float?$AA@':0x129c42c,
 '??_C@_04DDKJGDCM@bool?$AA@':0x129c434,
 '??_C@_08MIKNOGJA@type?5of?5?$AA@':0x129c5b8,  # derived from retail site; not in symbol_index
 '??_C@_0CL@MGHCCKDH@error?5?9?5could?5not?5read?5target?5fo@':0x129c5c4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CConsoleVariable_ProcessCommandLine_009ec430.cpp'
ROOTS=[
 dict(name='process_command_line',source=F,symbol='?ProcessCommandLine@CConsoleVariable@@EAE_NABVCCharString@@AAVCConsole@@@Z',address=0x9ec430,size=432,instructions=126,relocations=38),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-variable-process.json',[],
  'CConsoleVariable::ProcessCommandLine (Ego_r locals parsed_item/parser/var_val_as_string/help_message; single exit; explicit CCharString("") parser filename temporary)')
 print('NATIVE_CONSOLE_VARIABLE_PROCESS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
