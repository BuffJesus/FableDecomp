"""Fresh VC71 parity gate: the rest of the streaming font data bank family — CStreamingFontDataBank
construction/destruction, Close/CloseFonts, the GlyphData async entry lookup (InitStreamingEntry), the bank-file
overrides (data format version, update-entry adjustment, enumeration type, entry removal, inclusion), the
font entry's AcquireFont (constructs the CStreamingFontBank) and CanUnload, and CStreamingFontBankStateBlock
GetDefault/Set/CreateCRC. Exact roots; the bank reads are in gate_native_streaming_font_bank_read.py and the
entry lifetime in gate_native_streaming_font_entry.py."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '_memmove':0xbfeae6,  # derived from retail site; not in symbol_index
 '??0CStreamingFontBank@@QAE@PAVCStreamingFontDataBank@@K@Z':0x9a3c20,
 # CCountedPointer<CFontBank>::Reset: retail calls the folded template body at 0x0041947A.
 '?Reset@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@@Z':0x41947a,  # derived from retail site; not in symbol_index
 '??_C@_09GAGDNKMC@GlyphData?$AA@':0x129d5a4,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,  # derived from retail site; not in symbol_index
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??4CCharString@@QAEABV0@PBD@Z':0x99efe0,
 '?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0,
 '?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z':0x9d58d0,
 '??0CBankFileAsync@@QAE@XZ':0x9d5f80,
 '??1CBankFileAsync@@UAE@XZ':0x9d6490,
 '?Close@CBankFile@@UAEXXZ':0x9ce360,
 '?EntryExists@CBankFile@@QBE_NK@Z':0x9cc190,
 '?EntrySetUpdate@CBankFile@@QAE_NK_N@Z':0x9cc300,
 '?GetEnumerationType@CBankFile@@MBE?AVCCharString@@K@Z':0x9cbd10,
 '?Calc@CCRC@@QBEIPBXI@Z':0xa00950,
 '?CloseFonts@CStreamingFontDataBank@@AAEXXZ':0xa5f2f0,
 '??_7CStreamingFontDataBank@@6B@':0x129d5cc,  # derived from retail site; not in symbol_index
 '??_7CStreamingFontBankStateBlock@@6B@':0x129d594,  # derived from retail site; not in symbol_index
 '??_7CBankStateBlock@@6B@':0x129c82c,  # derived from retail site; not in symbol_index
 '??1CStreamingFontDataBank@@UAE@XZ':0xa5f3a0,
 '??0CDateAndTime@@QAE@JKKKKKK@Z':0x9e9a70,
}
D=R/'rebuild/src/compiled/00/a5'
ROOTS=[
 dict(name='acquire_font',source=D/'CStreamingFontDataBankEntry_AcquireFont_00a5f1c0.cpp',symbol='?AcquireFont@CStreamingFontDataBankEntry@@QBE?AV?$CCountedPointer@VCFontBank@@@@XZ',address=0xa5f1c0,size=83,instructions=35,relocations=3),
 dict(name='init_streaming_entry',source=D/'CStreamingFontDataBank_InitStreamingEntry_00a5ef50.cpp',symbol='?InitStreamingEntry@CStreamingFontDataBank@@QAEXAAVCSymbolMap@@@Z',address=0xa5ef50,size=178,instructions=61,relocations=7),
 dict(name='close_fonts',source=D/'CStreamingFontDataBank_CloseFonts_00a5f2f0.cpp',symbol='?CloseFonts@CStreamingFontDataBank@@AAEXXZ',address=0xa5f2f0,size=161,instructions=58,relocations=3),
 dict(name='ctor',source=D/'CStreamingFontDataBank_CStreamingFontDataBank_00a5f290.cpp',symbol='??0CStreamingFontDataBank@@QAE@XZ',address=0xa5f290,size=89,instructions=24,relocations=6),
 dict(name='dtor',source=D/'CStreamingFontDataBank_1CStreamingFontDataBank_00a5f3a0.cpp',symbol='??1CStreamingFontDataBank@@UAE@XZ',address=0xa5f3a0,size=126,instructions=31,relocations=7),
 dict(name='close',source=D/'CStreamingFontDataBank_Close_00a5f420.cpp',symbol='?Close@CStreamingFontDataBank@@UAEXXZ',address=0xa5f420,size=16,instructions=6,relocations=2),
 dict(name='data_format_version',source=D/'CStreamingFontDataBank_GetDataFormatVersion_00a5ee90.cpp',symbol='?GetDataFormatVersion@CStreamingFontDataBank@@MAEKXZ',address=0xa5ee90,size=6,instructions=2,relocations=0),
 dict(name='adjust_update_entries',source=D/'CStreamingFontDataBank_AdjustUpdateEntries_00a5eea0.cpp',symbol='?AdjustUpdateEntries@CStreamingFontDataBank@@MAEKXZ',address=0xa5eea0,size=57,instructions=30,relocations=2),
 dict(name='enumeration_type',source=D/'CStreamingFontDataBank_GetEnumerationType_00a5ef10.cpp',symbol='?GetEnumerationType@CStreamingFontDataBank@@MBE?AVCCharString@@K@Z',address=0xa5ef10,size=22,instructions=9,relocations=1),
 dict(name='entry_removed',source=D/'CStreamingFontDataBank_NotifyEntryRemoved_00a5f0b0.cpp',symbol='?NotifyEntryRemoved@CStreamingFontDataBank@@MAEXK@Z',address=0xa5f0b0,size=63,instructions=21,relocations=0),
 dict(name='prepare_for_inclusion',source=D/'CStreamingFontDataBank_PrepareDataForInclusion_00a5f0f0.cpp',symbol='?PrepareDataForInclusion@CStreamingFontDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z',address=0xa5f0f0,size=32,instructions=13,relocations=1),
 dict(name='entry_can_unload',source=D/'CStreamingFontDataBankEntry_CanUnload_00a5f230.cpp',symbol='?CanUnload@CStreamingFontDataBankEntry@@UBE_NXZ',address=0xa5f230,size=27,instructions=13,relocations=0),
 dict(name='state_get_default',source=D/'CStreamingFontBankStateBlock_GetDefault_00a5ee20.cpp',symbol='?GetDefault@CStreamingFontBankStateBlock@@UAEXAAVCBankStateBlock@@@Z',address=0xa5ee20,size=25,instructions=7,relocations=0),
 dict(name='state_set',source=D/'CStreamingFontBankStateBlock_Set_00a5ee40.cpp',symbol='?Set@CStreamingFontBankStateBlock@@MAEXPBVCBankStateBlock@@@Z',address=0xa5ee40,size=31,instructions=10,relocations=0),
 # Deleting destructors (vtable slot 0 of 0x0129D5CC and 0x0129D594), emitted with the vtables in the
 # constructor's unit; the state block's inlines its empty destructor and CBankStateBlock's (31 B: the
 # manifest's 30 stops before the final byte of `ret 4`).
 dict(name='deleting_dtor',source=D/'CStreamingFontDataBank_CStreamingFontDataBank_00a5f290.cpp',symbol='??_GCStreamingFontDataBank@@UAEPAXI@Z',address=0xa5fa30,size=30,instructions=11,relocations=2),
 dict(name='state_deleting_dtor',source=D/'CStreamingFontDataBank_CStreamingFontDataBank_00a5f290.cpp',symbol='??_GCStreamingFontBankStateBlock@@UAEPAXI@Z',address=0xa5ee70,size=31,instructions=11,relocations=2),
 dict(name='is_disk_file',source=D/'CStreamingFontDataBank_IsDiskFile_00a5f9c0.cpp',symbol='?IsDiskFile@CStreamingFontDataBank@@MAE_NKK@Z',address=0xa5f9c0,size=5,instructions=2,relocations=0),
 dict(name='last_modified_time',source=D/'CStreamingFontDataBank_GetLastModifiedTime_00a5f9d0.cpp',symbol='?GetLastModifiedTime@CStreamingFontDataBank@@MAE?AVCDateAndTime@@ABVCCharString@@@Z',address=0xa5f9d0,size=32,instructions=14,relocations=1),
 dict(name='state_create_crc',source=D/'CStreamingFontBankStateBlock_CreateCRC_00a5f6a0.cpp',symbol='?CreateCRC@CStreamingFontBankStateBlock@@UAEKXZ',address=0xa5f6a0,size=12,instructions=5,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-streaming-font-databank.json',[],
  'CStreamingFontDataBank lifetime/close/entry lookup/bank-file overrides, entry AcquireFont/CanUnload and the streaming font state block')
 print('NATIVE_STREAMING_FONT_DATABANK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
