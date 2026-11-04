"""Fresh VC71 parity gate: CSymbolMap::LoadFromFile (Ego_r locals success/file/text; Clear, UpdateDirty and the vector<char>(n) constructor expanded; parse-file CWideString static 0x013CAA34)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_erase@?$_Rb_tree@VCCharString@@V1@U?$_Identity@VCCharString@@@_STL@@U?$less@VCCharString@@@3@V?$allocator@VCCharString@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@VCCharString@@@2@@Z':0x4aba70,
 '??0CAFile@@QAE@XZ':0x99a2f0,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z':0x99a6a0,
 '?Close@CDiskFileWin32@@UAEXXZ':0x99a920,
 '?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z':0x99a960,
 '??0CWideString@@QAE@XZ':0x99aed0,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,
 '??4CWideString@@QAEABV0@PBG@Z':0x99b800,
 '?ParseBuffer@CSymbolMap@@IAE_NPBD@Z':0x9e5ad0,  # derived from retail site; not in symbol_index
 '?DeallocateMemory@?$CVectorMap@VCCharString@@VCSymbolInfo@CSymbolMap@@V?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@@QAEXXZ':0x9e79e0,
 '?DeallocateUnusedMemory@?$CVectorMap@VCCharString@@VCSymbolInfo@CSymbolMap@@V?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@@QAEXXZ':0x9e7c70,
 '??$sort@PAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@_STL@@V?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@_STL@@YIXPAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@0@0V?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@Z':0x9e7e40,
 '?GFRemoveComments@@YIXPAD00@Z':0xa60410,
 '_malloc':0xbfea0e,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??_7CDiskFileWin32@@6B@':0x122d06c,
 '??_C@_11LOCGONAA@?$AA?$AA@':0x122d70c,
 '??_C@_02FMMEOLBI@?1?$CK?$AA@':0x129b1f8,
 '??_C@_02CHHINNLG@?$CK?1?$AA@':0x129b1fc,
 '??_C@_01LIIJDEN@?$AN?$AA@':0x129b200,
 '??_C@_02CBLDBPFN@?1?1?$AA@':0x129b204,
 '??_C@_01EEMJAFIK@?6?$AA@':0x129b208,
 '?g_FableSymbolMapParseFile_013CAA34@@3VCWideString@@A':0x13caa34,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CSymbolMap_LoadFromFile_009e5c90.cpp'
ROOTS=[
 dict(name='load_from_file',source=F,symbol='?LoadFromFile@CSymbolMap@@QAE_NABVCWideString@@@Z',address=0x9e5c90,size=373,instructions=121,relocations=30),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-symbol-map-load.json',[],
  'CSymbolMap::LoadFromFile (Ego_r locals success/file/text; Clear, UpdateDirty and the vector<char>(n) constructor expanded; parse-file CWideString static 0x013CAA34)')
 print('NATIVE_SYMBOL_MAP_LOAD_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
