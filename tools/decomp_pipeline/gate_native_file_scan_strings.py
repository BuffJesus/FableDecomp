"""Fresh VC71 parity gate: CAFile::StartFileScan / ScanNextFile CWideString forms (tfilename[261] through the wchar_t* forms)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PAG@Z':0x999760,  # derived from retail site; not in symbol_index
 '?ScanNextFile@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PAG@Z':0x999850,  # derived from retail site; not in symbol_index
 '??4CWideString@@QAEABV0@PBG@Z':0x99b800,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_FileScanStrings_00999c50.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='start_scan_string',source=F,symbol='?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@AAV2@@Z',address=0x999c50,size=93,instructions=22,relocations=5,flags=GS),
 dict(name='next_scan_string',source=F,symbol='?ScanNextFile@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@AAV2@@Z',address=0x999cb0,size=93,instructions=22,relocations=5,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-scan-strings.json',[],
  'CAFile::StartFileScan / ScanNextFile CWideString forms (tfilename[261] through the wchar_t* forms)')
 print('NATIVE_FILE_SCAN_STRINGS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
