"""Fresh VC71 parity gate: errordialog.cpp localized string/dialog helpers (Ego_r names and parameters; gApplicationLanguage then 0x409 then the plain user32 call). RetrieveEnglishString 0099D430 is absent from the manifest."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?RetrieveStringByLanguage@@YIHPAUHINSTANCE__@@KPADKG@Z':0x99d3a0,  # derived from retail site; not in symbol_index
 '?LocalizedDialogBoxByLanguage@@YIHPAUHINSTANCE__@@PBDPAUHWND__@@P6GH2IIJ@ZG@Z':0x99d4d0,  # derived from retail site; not in symbol_index
 '?gApplicationLanguage@@3GA':0x138e060,  # derived from retail site; not in symbol_index
 '__imp__FindResourceExA@16':0x143fd8c,  # derived from retail site; not in symbol_index
 '__imp__LoadResource@8':0x143fd90,  # derived from retail site; not in symbol_index
 '__imp__LockResource@4':0x143fd94,  # derived from retail site; not in symbol_index
 '__imp__WideCharToMultiByte@32':0x143fd98,  # derived from retail site; not in symbol_index
 '__imp__LoadStringA@16':0x14402d4,  # derived from retail site; not in symbol_index
 '__imp__DialogBoxIndirectParamA@20':0x14402d8,  # derived from retail site; not in symbol_index
 '__imp__DialogBoxParamA@20':0x14402dc,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/ErrorDialog_LocalizedResources_0099d3a0.cpp'
ROOTS=[
 dict(name='retrieve_by_language',source=F,symbol='?RetrieveStringByLanguage@@YIHPAUHINSTANCE__@@KPADKG@Z',address=0x99d3a0,size=142,instructions=62,relocations=4),
 dict(name='retrieve_english',source=F,symbol='@RetrieveEnglishString@16',address=0x99d430,size=49,instructions=24,relocations=2),
 dict(name='retrieve',source=F,symbol='@RetrieveString@16',address=0x99d470,size=84,instructions=36,relocations=5),
 dict(name='dialog_by_language',source=F,symbol='?LocalizedDialogBoxByLanguage@@YIHPAUHINSTANCE__@@PBDPAUHWND__@@P6GH2IIJ@ZG@Z',address=0x99d4d0,size=76,instructions=32,relocations=4),
 dict(name='dialog',source=F,symbol='@LocalizedDialogBox@16',address=0x99d520,size=130,instructions=55,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-error-dialog-resources.json',[],
  'errordialog.cpp localized string/dialog helpers (Ego_r names and parameters; gApplicationLanguage then 0x409 then the plain user32 call). RetrieveEnglishString 0099D430 is absent from the manifest')
 print('NATIVE_ERROR_DIALOG_RESOURCES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
