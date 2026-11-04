"""Fresh VC71 parity gate: CAFile::GetCopyFilename (Ego_r locals; trailing number via digit * (float)pow(10.0, (double)n), _CIpow + _ftol)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GFIsCharStringNumber@@YI_NABVCCharString@@@Z':0x99e620,
 '?GFCharStringToInt@@YIJABVCCharString@@@Z':0x99e7f0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '?GetSubString@CCharString@@QBE?AV1@KK@Z':0x99ec70,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??YCCharString@@QAEABV0@ABV0@@Z':0x99f0a0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,
 '?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,
 '__ftol2':0xbfea70,  # derived from retail site; not in symbol_index
 '__CIpow':0xbfeb40,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '__real@4024000000000000':0x129a7b0,
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_GetCopyFilename_00997bb0.cpp'
ROOTS=[
 dict(name='copy_filename',source=F,symbol='?GetCopyFilename@CAFile@@SI?AVCCharString@@ABV2@@Z',address=0x997bb0,size=495,instructions=165,relocations=31),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-copy-filename.json',[],
  'CAFile::GetCopyFilename (Ego_r locals; trailing number via digit * (float)pow(10.0, (double)n), _CIpow + _ftol)')
 print('NATIVE_FILE_COPY_FILENAME_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
