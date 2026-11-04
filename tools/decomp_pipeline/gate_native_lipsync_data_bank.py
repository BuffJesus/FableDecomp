"""Fresh VC71 parity gate: CLipSyncDataBank CBankFile overrides (GetDataFormatVersion, Update, IsDiskFile, GetEnumerationType, PrepareDataForInclusion; 0xA42790..0xA42960). Hand-owned fable_lipsync_data_bank.h; journal 2026-10-29."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??3@YAXPAX@Z':0xbfe9bc,
 '?GetEnumerationType@CBankFile@@MBE?AVCCharString@@K@Z':0x9cbd10,
 '?Update@CBankFile@@UAEXXZ':0x9cec60,
}
ROOTS=[
 dict(name='clipsyncdatabank_getdataformatversion',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Overrides_00a42790.cpp',symbol='?GetDataFormatVersion@CLipSyncDataBank@@UAEKXZ',address=0xa42790,size=6,instructions=2,relocations=0),
 dict(name='clipsyncdatabank_update',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Overrides_00a42790.cpp',symbol='?Update@CLipSyncDataBank@@UAEXXZ',address=0xa427a0,size=16,instructions=5,relocations=1),
 dict(name='clipsyncdatabank_isdiskfile',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Overrides_00a42790.cpp',symbol='?IsDiskFile@CLipSyncDataBank@@MAE_NKK@Z',address=0xa427c0,size=5,instructions=2,relocations=0),
 dict(name='clipsyncdatabank_getenumerationtype',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Overrides_00a42790.cpp',symbol='?GetEnumerationType@CLipSyncDataBank@@MBE?AVCCharString@@K@Z',address=0xa427e0,size=22,instructions=9,relocations=1),
 dict(name='clipsyncdatabank_preparedataforinclusion',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Overrides_00a42790.cpp',symbol='?PrepareDataForInclusion@CLipSyncDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z',address=0xa42940,size=32,instructions=13,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-lipsync-data-bank.json',[],
  'CLipSyncDataBank CBankFile overrides (GetDataFormatVersion, Update, IsDiskFile, GetEnumerationType, PrepareDataForInclus')
 print('NATIVE_LIPSYNC_DATA_BANK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
