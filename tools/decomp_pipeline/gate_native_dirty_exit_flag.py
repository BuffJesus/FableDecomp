"""Fresh VC71 parity gate: errordialog.cpp exit flag (Ego_r extern C fastcall SetDirtyExitFlag / ClearDirtyExitFlag over the Fable TLC HKCU key, value ExitFlag)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??_C@_0CN@CKFIADFJ@Software?2Microsoft?2Microsoft?5Gam@':0x129a8e4,  # derived from retail site; not in symbol_index
 '??_C@_08LJDBNIPI@ExitFlag?$AA@':0x129a9c8,  # derived from retail site; not in symbol_index
 '??_C@_05CCAMFJOL@clean?$AA@':0x129a9d4,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
 '__imp__RegQueryValueExA@24':0x143fbec,  # derived from retail site; not in symbol_index
 '__imp__RegCreateKeyExA@36':0x143fbf0,  # derived from retail site; not in symbol_index
 '__imp__RegSetValueExA@24':0x143fbf4,  # derived from retail site; not in symbol_index
 '__imp__RegCloseKey@4':0x143fbfc,  # derived from retail site; not in symbol_index
 '__imp__RegOpenKeyExA@20':0x143fc08,  # derived from retail site; not in symbol_index
 '__imp__lstrlenA@4':0x143fd88,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/Global_ClearDirtyExitFlag_0099dc50.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='set',source=F,symbol='@SetDirtyExitFlag@0',address=0x99dbe0,size=106,instructions=30,relocations=7,flags=GS),
 dict(name='clear',source=F,symbol='@ClearDirtyExitFlag@0',address=0x99dc50,size=86,instructions=28,relocations=8,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-dirty-exit-flag.json',[],
  'errordialog.cpp exit flag (Ego_r extern C fastcall SetDirtyExitFlag / ClearDirtyExitFlag over the Fable TLC HKCU key, value ExitFlag)')
 print('NATIVE_DIRTY_EXIT_FLAG_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
