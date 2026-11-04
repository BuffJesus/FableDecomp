"""Fresh VC71 parity gate: lib_file_abstract small path helpers: CAFile::GetHDSectorSize/GetDVDSectorSize, IsForbiddenSymbolInComponent."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?FindCharPos@CCharString@@QBEJD@Z':0x99e5a0,
 '?g_FableForbiddenComponentSymbols_0129A79C@@3QBDB':0x129a79c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_PathHelpers_009975b0.cpp'
ROOTS=[
 dict(name='hd_sector',source=F,symbol='?GetHDSectorSize@CAFile@@SIJXZ',address=0x9975b0,size=6,instructions=2,relocations=0),
 dict(name='dvd_sector',source=F,symbol='?GetDVDSectorSize@CAFile@@SIJXZ',address=0x9975c0,size=6,instructions=2,relocations=0),
 dict(name='forbidden_symbol',source=F,symbol='?IsForbiddenSymbolInComponent@CAFile@@CI_NABVCCharString@@@Z',address=0x9975e0,size=55,instructions=27,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-path-helpers.json',[],
  'lib_file_abstract small path helpers: CAFile::GetHDSectorSize/GetDVDSectorSize, IsForbiddenSymbolInComponent')
 print('NATIVE_FILE_PATH_HELPERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
