"""Fresh VC71 parity gate: CAFile::GetAbsoluteFilePath narrow form (Ego_r locals; parser over \\-separated items, '..' drops a component)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z':0x404720,
 '??8?$CBasicString@D@@QBE_NPBD@Z':0x4115a0,
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '?GetSubString@CCharString@@QBE?AV1@KK@Z':0x99ec70,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??YCCharString@@QAEABV0@ABV0@@Z':0x99f0a0,
 '?Truncate@CCharString@@QAEXK@Z':0x99f2a0,
 '??H@YI?AVCCharString@@PBDABV0@@Z':0x99f690,
 '?GetString@CParsedItem@@QBEABVCCharString@@XZ':0x9b9880,
 '?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z':0x9b9ba0,
 '?ReadAsStringUntilString@CStringParser@@QAE_NABVCCharString@@AAVCParsedItem@@@Z':0x9ba110,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_01KICIPPFI@?2?$AA@':0x124c530,  # derived from retail site; not in symbol_index
 '??_C@_02DJGKEECL@?4?4?$AA@':0x129a7b8,  # derived from retail site; not in symbol_index
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_GetAbsoluteFilePath_00998250.cpp'
ROOTS=[
 dict(name='absolute_narrow',source=F,symbol='?GetAbsoluteFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z',address=0x998250,size=786,instructions=249,relocations=54),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-absolute-path.json',[],
  "CAFile::GetAbsoluteFilePath narrow form (Ego_r locals; parser over \\-separated items, '..' drops a component)")
 print('NATIVE_FILE_ABSOLUTE_PATH_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
