"""Fresh VC71 parity gate: CFontDataBank::NotifyEntryAdded (grow Fonts, type-0 entries get CFontDataBankEntry, base notify)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
 '?NotifyEntryAdded@CBankFile@@MAEXKKABV?$CArray@D@@@Z':0x9cbc20,
 '?_M_insert_overflow@?$vector@PAVCFontDataBankEntry@@V?$allocator@PAVCFontDataBankEntry@@@_STL@@@_STL@@QAEXPAPAVCFontDataBankEntry@@ABQAV3@ABU__true_type@2@I_N@Z':0xa639e0,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_7CFontDataBankEntry@@6B@':0x129da70,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a6/CFontDataBank_NotifyEntryAdded_00a636c0.cpp'
ROOTS=[
 dict(name='entry_added',source=F,symbol='?NotifyEntryAdded@CFontDataBank@@MAEXKKABV?$CArray@D@@@Z',address=0xa636c0,size=203,instructions=74,relocations=5),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-font-bank-entry-added.json',[],
  'CFontDataBank::NotifyEntryAdded (grow Fonts, type-0 entries get CFontDataBankEntry, base notify)')
 print('NATIVE_FONT_BANK_ENTRY_ADDED_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
