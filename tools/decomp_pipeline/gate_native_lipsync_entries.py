"""Fresh VC71 parity gate: CLipSyncDataBank entry table (CloseData/dtor/Close/NotifyEntryAdded/NotifyBankSize) and the out-of-line STLport vector<CLipSyncDataBankEntry> family 0x00A43500..0x00A43FF0, exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??$__copy@PAVCLipSyncDataBankEntry@@PAV1@H@_STL@@YIPAVCLipSyncDataBankEntry@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z':0xa438e0,
 '??$__destroy_aux@PAVCLipSyncDataBankEntry@@@_STL@@YIXPAVCLipSyncDataBankEntry@@0ABU__false_type@0@@Z':0xa43a00,
 '??$__uninitialized_copy@PAVCLipSyncDataBankEntry@@PAV1@@_STL@@YIPAVCLipSyncDataBankEntry@@PAV1@00ABU__false_type@0@@Z':0xa43630,
 '??$__uninitialized_fill_n@PAVCLipSyncDataBankEntry@@IV1@@_STL@@YIPAVCLipSyncDataBankEntry@@PAV1@IABV1@ABU__false_type@0@@Z':0xa43500,
 '??1CBankFileAsync@@UAE@XZ':0x9d6490,
 '??1CLipSyncDataBank@@UAE@XZ':0xa42ce0,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??_7CLipSyncDataBank@@6B@':0x129d3d4,
 '?Close@CBankFile@@UAEXXZ':0x9ce360,
 '?NotifyEntryAdded@CBankFile@@MAEXKKABV?$CArray@D@@@Z':0x9cbc20,
 '?_M_fill_insert@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@_STL@@@_STL@@QAEXPAVCLipSyncDataBankEntry@@IABV3@@Z':0xa43cd0,
 '?resize@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@_STL@@@_STL@@QAEXIABVCLipSyncDataBankEntry@@@Z':0xa43f20,
 '_free':0xbfea14,
 '_malloc':0xbfea0e,
}
ROOTS=[
 dict(name='clipsyncdatabank_closedata',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?CloseData@CLipSyncDataBank@@AAEXXZ',address=0xa42c00,size=62,instructions=23,relocations=2),
 dict(name='clipsyncdatabank_destructor',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??1CLipSyncDataBank@@UAE@XZ',address=0xa42ce0,size=180,instructions=52,relocations=7),
 dict(name='clipsyncdatabank_close',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?Close@CLipSyncDataBank@@UAEXXZ',address=0xa42da0,size=69,instructions=25,relocations=3),
 dict(name='clipsyncdatabank_notifyentryadded',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?NotifyEntryAdded@CLipSyncDataBank@@MAEXKKABV?$CArray@D@@@Z',address=0xa42df0,size=119,instructions=42,relocations=2),
 dict(name='clipsyncdatabank_notifybanksize',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?NotifyBankSize@CLipSyncDataBank@@MAEXKABV?$CArray@VCBankFileDataTypeCount@@@@@Z',address=0xa42e70,size=45,instructions=11,relocations=1),
 dict(name='lipsyncentries_uninitialized_fill_n_tag',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??$__uninitialized_fill_n@PAVCLipSyncDataBankEntry@@IV1@@_STL@@YIPAVCLipSyncDataBankEntry@@PAV1@IABV1@ABU__false_type@0@@Z',address=0xa43500,size=47,instructions=21,relocations=0),
 dict(name='lipsyncentries_uninitialized_copy',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??$__uninitialized_copy@PAVCLipSyncDataBankEntry@@PAV1@@_STL@@YIPAVCLipSyncDataBankEntry@@PAV1@00ABU__false_type@0@@Z',address=0xa43630,size=51,instructions=20,relocations=0),
 dict(name='lipsyncentries_copy_backward',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??$__copy_backward@PAVCLipSyncDataBankEntry@@PAV1@H@_STL@@YIPAVCLipSyncDataBankEntry@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z',address=0xa43720,size=94,instructions=41,relocations=0),
 dict(name='lipsyncentries_fill',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??$fill@PAVCLipSyncDataBankEntry@@V1@@_STL@@YIXPAVCLipSyncDataBankEntry@@0ABV1@@Z',address=0xa43890,size=72,instructions=33,relocations=0),
 dict(name='lipsyncentries_copy',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??$__copy@PAVCLipSyncDataBankEntry@@PAV1@H@_STL@@YIPAVCLipSyncDataBankEntry@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z',address=0xa438e0,size=88,instructions=39,relocations=0),
 dict(name='lipsyncentries_destroy_aux',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??$__destroy_aux@PAVCLipSyncDataBankEntry@@@_STL@@YIXPAVCLipSyncDataBankEntry@@0ABU__false_type@0@@Z',address=0xa43a00,size=50,instructions=21,relocations=0),
 dict(name='lipsyncentries_erase',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?erase@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@_STL@@@_STL@@QAEPAVCLipSyncDataBankEntry@@PAV3@0@Z',address=0xa43ae0,size=57,instructions=24,relocations=2),
 dict(name='lipsyncentries_insert_overflow',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?_M_insert_overflow@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@_STL@@@_STL@@QAEXPAVCLipSyncDataBankEntry@@ABV3@ABU__false_type@2@I_N@Z',address=0xa43b20,size=223,instructions=87,relocations=6),
 dict(name='lipsyncentries_clear',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?clear@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@_STL@@@_STL@@QAEXXZ',address=0xa43c90,size=49,instructions=23,relocations=2),
 dict(name='lipsyncentries_resize',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?resize@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@_STL@@@_STL@@QAEXIABVCLipSyncDataBankEntry@@@Z',address=0xa43f20,size=88,instructions=38,relocations=3),
 dict(name='lipsyncentries_resize_default',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='?resize@?$vector@VCLipSyncDataBankEntry@@V?$allocator@VCLipSyncDataBankEntry@@@_STL@@@_STL@@QAEXI@Z',address=0xa43ff0,size=39,instructions=10,relocations=1),
 dict(name='clipsyncdatabank_deleting_destructor',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??_GCLipSyncDataBank@@UAEPAXI@Z',address=0xa43fd0,size=30,instructions=11,relocations=2),
 dict(name='lipsyncentry_assign',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??4CLipSyncDataBankEntry@@QAEAAV0@ABV0@@Z',address=0xa436e0,size=60,instructions=26,relocations=0),
 dict(name='lipsyncentry_pointer_assign',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??4?$CIVCountedPointer@VCData@CLipSyncDataBankEntry@@@@QAEAAV0@ABV0@@Z',address=0xa43530,size=54,instructions=24,relocations=0),
 dict(name='lipsyncentry_construct',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??0CLipSyncDataBankEntry@@QAE@XZ',address=0xa43570,size=16,instructions=4,relocations=0),
 dict(name='lipsyncentry_destruct',source=R/'rebuild/src/compiled/00/a4/CLipSyncDataBank_Entries_00a42c00.cpp',symbol='??1CLipSyncDataBankEntry@@QAE@XZ',address=0xa43940,size=27,instructions=12,relocations=0),
 dict(name='lipsyncentries_uninitialized_fill_n',source=R/'rebuild/src/compiled/00/a4/LipSyncEntries_UninitializedFillN_00a43670.cpp',symbol='??$uninitialized_fill_n@VCLipSyncDataBankEntry@@@_STL@@YIPAVCLipSyncDataBankEntry@@PAV1@IABV1@@Z',address=0xa43670,size=47,instructions=21,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-lipsync-entries.json',[],
  'CLipSyncDataBank entry table (CloseData/dtor/Close/NotifyEntryAdded/NotifyBankSize) and the out-of-line STLport vector<C')
 print('NATIVE_LIPSYNC_ENTRIES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
