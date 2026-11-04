"""Fresh VC71 parity gate: errordialog.cpp EnumResName / EnumResType debug callbacks and the ErrorArray lookups nErrorStringFromErrorCode / nErrorUrlStringFromErrorCode (Ego_r names and locals)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?EnumResName@@YGHPAUHINSTANCE__@@PBDPADJ@Z':0x99d2c0,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??_C@_04PHJALBM@?7?$CFs?6?$AA@':0x129a954,  # derived from retail site; not in symbol_index
 '??_C@_04BGBALOOJ@?7?$CFd?6?$AA@':0x129a95c,  # derived from retail site; not in symbol_index
 '??_C@_03OFAPEBGM@?$CFs?6?$AA@':0x129a964,  # derived from retail site; not in symbol_index
 '??_C@_03PMGGPEJJ@?$CFd?6?$AA@':0x129a968,  # derived from retail site; not in symbol_index
 '?ErrorArray@@3PAUERRORARRAYSTRUCT@@A':0x138de50,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
 '__imp__EnumResourceNamesA@16':0x143fd9c,  # derived from retail site; not in symbol_index
 '__imp__OutputDebugStringA@4':0x143fda0,  # derived from retail site; not in symbol_index
 '__imp__wsprintfA':0x14402d0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/ErrorDialog_Lookups_0099d2c0.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='enum_res_name',source=F,symbol='?EnumResName@@YGHPAUHINSTANCE__@@PBDPADJ@Z',address=0x99d2c0,size=95,instructions=22,relocations=6,flags=GS),
 dict(name='enum_res_type',source=F,symbol='?EnumResType@@YGHPAUHINSTANCE__@@PADJ@Z',address=0x99d320,size=120,instructions=30,relocations=8,flags=GS),
 dict(name='error_string',source=F,symbol='?nErrorStringFromErrorCode@@YIHH@Z',address=0x99db80,size=46,instructions=17,relocations=3,flags=GS),
 dict(name='error_url',source=F,symbol='?nErrorUrlStringFromErrorCode@@YIHH@Z',address=0x99dbb0,size=46,instructions=17,relocations=3,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-error-dialog-lookups.json',[],
  'errordialog.cpp EnumResName / EnumResType debug callbacks and the ErrorArray lookups nErrorStringFromErrorCode / nErrorUrlStringFromErrorCode (Ego_r names and locals)')
 print('NATIVE_ERROR_DIALOG_LOOKUPS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
