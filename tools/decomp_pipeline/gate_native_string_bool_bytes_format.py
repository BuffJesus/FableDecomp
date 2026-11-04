"""Fresh VC71 parity gate: GFBoolToCharString (TRUE/FALSE) and GFByteSizeToCharString (bytes / KB / MB; Ego_r unused local str); CCharString lifetime members and AllocStringData expanded."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?UnassignString@CCharString@@AAEXXZ':0x99e9b0,
 '??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,
 '?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,
 '??0?$CBasicString@D@@QAE@PBD@Z':0x9a0590,
 '?GFFloatToCharString@@YI?AVCCharString@@NJ@Z':0x9a08b0,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
 '??_C@_05MAJJAKPI@FALSE?$AA@':0x122d6cc,
 '??_C@_04HCDDPBNL@TRUE?$AA@':0x122d6d4,
 '__real@4f800000':0x122dcb4,
 '__real@35800000':0x122ed60,  # derived from retail site; not in symbol_index
 '__real@3a800000':0x1230008,  # derived from retail site; not in symbol_index
 '??_C@_06LCIDCBMH@?5bytes?$AA@':0x129ab0c,  # derived from retail site; not in symbol_index
 '??_C@_03MAAJMAHL@?5KB?$AA@':0x129ab14,  # derived from retail site; not in symbol_index
 '??_C@_03MEIELMMJ@?5MB?$AA@':0x129ab18,  # derived from retail site; not in symbol_index
 '?g_CCharStringInstanceCount_013BD800@@3HA':0x13bd800,
}
F=R/'rebuild/src/compiled/00/99/Global_GFBoolToCharString_0099f9b0.cpp'
ROOTS=[
 dict(name='bool',source=F,symbol='?GFBoolToCharString@@YI?AVCCharString@@_N@Z',address=0x99f9b0,size=145,instructions=54,relocations=10),
 dict(name='byte_size',source=F,symbol='?GFByteSizeToCharString@@YI?AVCCharString@@KJ@Z',address=0x99fa90,size=203,instructions=52,relocations=17),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-bool-bytes-format.json',[],
  'GFBoolToCharString (TRUE/FALSE) and GFByteSizeToCharString (bytes / KB / MB; Ego_r unused local str); CCharString lifetime members and AllocStringData expanded')
 print('NATIVE_STRING_BOOL_BYTES_FORMAT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
