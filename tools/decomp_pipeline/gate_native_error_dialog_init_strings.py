"""Fresh VC71 parity gate: errordialog.cpp InitExternalStrings (Ego_r extern C fastcall; locals hKey/Buffer/dwDataSize/wLanguageId/dwType): strings.dll, the LangID language, crash-handler texts."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '@RetrieveString@16':0x99d470,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??_C@_08LLJELPPP@eula?4rtf?$AA@':0x122d05c,  # derived from retail site; not in symbol_index
 '??_C@_0CN@FFMDDMNN@Software?2Microsoft?2Microsoft?5Gam@':0x129a920,  # derived from retail site; not in symbol_index
 '??_C@_0BO@PNNGHGDB@Invalid?5?1?5missing?5strings?4dll?$AA@':0x129aa74,  # derived from retail site; not in symbol_index
 '??_C@_0BM@DOIDNLKA@Gathering?5Exception?5Data?4?4?4?$AA@':0x129aa94,  # derived from retail site; not in symbol_index
 '??_C@_0L@GOHGLDDA@Exception?$CB?$AA@':0x129aab0,  # derived from retail site; not in symbol_index
 '??_C@_06EJDNNJKL@LangID?$AA@':0x129aabc,  # derived from retail site; not in symbol_index
 '??_C@_06JPHCLLC@Error?$CB?$AA@':0x129aac4,  # derived from retail site; not in symbol_index
 '??_C@_0BJ@MFODLNDL@?2strings?4dll?5is?5missing?4?$AA@':0x129aacc,  # derived from retail site; not in symbol_index
 '??_C@_0M@NPIPHBOM@strings?4dll?$AA@':0x129aae8,  # derived from retail site; not in symbol_index
 '?gApplicationLanguage@@3GA':0x138e060,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
 '_gamestarttime':0x13bce38,  # derived from retail site; not in symbol_index
 '?Text_GatheringExceptionData@@3PADA':0x13bd0c8,  # derived from retail site; not in symbol_index
 '?Text_Exception@@3PADA':0x13bd1c8,  # derived from retail site; not in symbol_index
 '_Text_EULA':0x13bd2c8,  # derived from retail site; not in symbol_index
 '_Text_Switches':0x13bd2e8,  # derived from retail site; not in symbol_index
 '_hStrings':0x13bd6ec,  # derived from retail site; not in symbol_index
 '__imp__RegQueryValueExA@24':0x143fbec,  # derived from retail site; not in symbol_index
 '__imp__RegCloseKey@4':0x143fbfc,  # derived from retail site; not in symbol_index
 '__imp__RegOpenKeyExA@20':0x143fc08,  # derived from retail site; not in symbol_index
 '__imp__GetCurrentDirectoryA@8':0x143fd78,  # derived from retail site; not in symbol_index
 '__imp__ExitProcess@4':0x143fd7c,  # derived from retail site; not in symbol_index
 '__imp__GetTickCount@0':0x143fdfc,  # derived from retail site; not in symbol_index
 '__imp__LoadLibraryA@4':0x143fe40,  # derived from retail site; not in symbol_index
 '__imp__MessageBoxA@16':0x14402c4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/ErrorDialog_InitExternalStrings_0099e260.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='init_external_strings',source=F,symbol='@InitExternalStrings@0',address=0x99e260,size=502,instructions=126,relocations=49,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-error-dialog-init-strings.json',[],
  'errordialog.cpp InitExternalStrings (Ego_r extern C fastcall; locals hKey/Buffer/dwDataSize/wLanguageId/dwType): strings.dll, the LangID language, crash-handler texts')
 print('NATIVE_ERROR_DIALOG_INIT_STRINGS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
