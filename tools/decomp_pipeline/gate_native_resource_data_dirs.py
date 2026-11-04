"""Fresh VC71 parity gate: NResourceDirectoryNames::GetBrushDir / GetTattooDataDir (one literal each)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CWideString@@QAE@PBG@Z':0x99b6b0,
 '??_C@_1BM@KJHCGKAN@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AAB?$AAr?$AAu?$AAs?$AAh?$AAe?$AAs?$AA?2?$AA?$AA@':0x122f400,  # derived from retail site; not in symbol_index
 '??_C@_1BM@NMMJAPLE@?$AAD?$AAa?$AAt?$AAa?$AA?2?$AAT?$AAa?$AAt?$AAt?$AAo?$AAo?$AAs?$AA?2?$AA?$AA@':0x122f578,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/41/NResourceDirectoryNames_DataDirs_0041a0e0.cpp'
ROOTS=[
 dict(name='brush',source=F,symbol='?GetBrushDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a0e0,size=17,instructions=7,relocations=2),
 dict(name='tattoo_data',source=F,symbol='?GetTattooDataDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ',address=0x41a6a0,size=17,instructions=7,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-resource-data-dirs.json',[],
  'NResourceDirectoryNames::GetBrushDir / GetTattooDataDir (one literal each)')
 print('NATIVE_RESOURCE_DATA_DIRS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
