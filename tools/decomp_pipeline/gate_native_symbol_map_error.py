"""Fresh VC71 parity gate: CSymbolMap::Error (Ego_r protected cdecl varargs member; locals string/error_out/line_string; the assembled message is dropped)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '__snprintf':0xbfeb96,  # derived from retail site; not in symbol_index
 '_vsprintf':0xbfedd0,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_0BC@OFLKCPPJ@?5?3?5error?5C2065?5?3?5?$AA@':0x129c138,  # derived from retail site; not in symbol_index
 '??_C@_04FEBIMPDM@?$CI?$CFd?$CJ?$AA@':0x129c14c,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CSymbolMap_Error_009e4f80.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='error',source=F,symbol='?Error@CSymbolMap@@IAAXABVCWideString@@JPBDZZ',address=0x9e4f80,size=270,instructions=69,relocations=22,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-symbol-map-error.json',[],
  'CSymbolMap::Error (Ego_r protected cdecl varargs member; locals string/error_out/line_string; the assembled message is dropped)')
 print('NATIVE_SYMBOL_MAP_ERROR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
