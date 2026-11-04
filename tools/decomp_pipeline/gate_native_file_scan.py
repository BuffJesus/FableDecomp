"""Fresh VC71 parity gate: CAFile::StartFileScan / ScanNextFile wchar_t* forms (_wfindfirst/_wfindnext on the expanded pattern, drive+directory+name out)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ':0x997f80,
 '?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z':0x998c90,
 '??BCWideString@@QBEPBGXZ':0x99af00,
 '?GetAsCharArray@CWideString@@QBEPBGXZ':0x99af10,
 '??1CWideString@@QAE@XZ':0x99b510,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '__wsplitpath':0xbfed4c,  # derived from retail site; not in symbol_index
 '_wcscat':0xbfed5e,  # derived from retail site; not in symbol_index
 '_wcscpy':0xbfed76,  # derived from retail site; not in symbol_index
 '__wfindfirst':0xbfed7c,  # derived from retail site; not in symbol_index
 '__findclose':0xbfed82,  # derived from retail site; not in symbol_index
 '__wfindnext':0xbfed88,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_FileScan_00999760.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='start_scan',source=F,symbol='?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PAG@Z',address=0x999760,size=237,instructions=69,relocations=15,flags=GS),
 dict(name='next_scan',source=F,symbol='?ScanNextFile@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PAG@Z',address=0x999850,size=243,instructions=72,relocations=15,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-scan.json',[],
  'CAFile::StartFileScan / ScanNextFile wchar_t* forms (_wfindfirst/_wfindnext on the expanded pattern, drive+directory+name out)')
 print('NATIVE_FILE_SCAN_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
