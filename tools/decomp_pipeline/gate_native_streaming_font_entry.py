"""Fresh VC71 parity gate: CStreamingFontDataBank::NotifyEntryAdded (grow Fonts, type 0/1/2 entries get CStreamingFontDataBankEntry, base notify) and CStreamingFontDataBankEntry Unload (inline ReleaseData), IsLoaded, destructor and scalar deleting destructor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '?NotifyEntryAdded@CBankFile@@MAEXKKABV?$CArray@D@@@Z':0x9cbc20,
 '??1CStreamingFontDataBankEntry@@UAE@XZ':0xa5f150,
 '?_M_insert_overflow@?$vector@PAVCStreamingFontDataBankEntry@@V?$allocator@PAVCStreamingFontDataBankEntry@@@_STL@@@_STL@@QAEXPAPAVCStreamingFontDataBankEntry@@ABQAV3@ABU__true_type@2@I_N@Z':0xa5f8e0,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '??_7CStreamingFontDataBankEntry@@6B@':0x129d5b4,  # derived from retail site; not in symbol_index
}
A=R/'rebuild/src/compiled/00/a5/CStreamingFontDataBank_NotifyEntryAdded_00a5f430.cpp'
U=R/'rebuild/src/compiled/00/a5/CStreamingFontDataBankEntry_Unload_00a5f250.cpp'
ROOTS=[
 dict(name='entry_added',source=A,symbol='?NotifyEntryAdded@CStreamingFontDataBank@@MAEXKKABV?$CArray@D@@@Z',address=0xa5f430,size=213,instructions=78,relocations=5),
 dict(name='unload',source=U,symbol='?Unload@CStreamingFontDataBankEntry@@UAEXXZ',address=0xa5f250,size=49,instructions=22,relocations=1),
 dict(name='dtor',source=U,symbol='??1CStreamingFontDataBankEntry@@UAE@XZ',address=0xa5f150,size=100,instructions=39,relocations=4),
 dict(name='is_loaded',source=U,symbol='?IsLoaded@CStreamingFontDataBankEntry@@UBE_NXZ',address=0xa5f220,size=11,instructions=5,relocations=0),
 dict(name='deleting_dtor',source=U,symbol='??_GCStreamingFontDataBankEntry@@UAEPAXI@Z',address=0xa5f800,size=30,instructions=11,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-streaming-font-entry.json',[],
  'CStreamingFontDataBank::NotifyEntryAdded and CStreamingFontDataBankEntry Unload, IsLoaded, destructor and scalar deleting destructor')
 print('NATIVE_STREAMING_FONT_ENTRY_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
