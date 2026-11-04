"""Fresh VC71 parity gate: CSystemRegistry value accessors (Ego_r members; GetString wchar_t[512] under /GS; advapi32 called as dllimports; GetBinary uses new[]/delete[]). GetBOOL 0099CFE0 supersedes the fake CEngineMap::PeekWaterHasType landing."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetAsCharArray@CWideString@@QBEPBGXZ':0x99af10,
 '?GetLength@CWideString@@QBEJXZ':0x99b220,
 '??0CWideString@@QAE@PBG@Z':0x99b6b0,
 '??0CWideString@@QAE@ABV0@@Z':0x99b720,
 '?GetSLONG@CSystemRegistry@@QBEJABVCWideString@@J@Z':0x99cf80,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??_V@YAXPAX@Z':0xbfeb1c,
 '??_U@YAPAXI@Z':0xbfeb22,
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
 '__imp__RegSetValueExW@24':0x143fbe8,  # derived from retail site; not in symbol_index
 '__imp__RegQueryValueExW@24':0x143fc00,  # derived from retail site; not in symbol_index
 '__imp__RegDeleteValueW@8':0x143fc04,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CSystemRegistry_Values_0099cf80.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='get_slong',source=F,symbol='?GetSLONG@CSystemRegistry@@QBEJABVCWideString@@J@Z',address=0x99cf80,size=86,instructions=30,relocations=2,flags=GS),
 dict(name='get_bool',source=F,symbol='?GetBOOL@CSystemRegistry@@QBEHABVCWideString@@H@Z',address=0x99cfe0,size=26,instructions=9,relocations=1,flags=GS),
 dict(name='get_string',source=F,symbol='?GetString@CSystemRegistry@@QBE?AVCWideString@@ABV2@0@Z',address=0x99d000,size=177,instructions=47,relocations=7,flags=GS),
 dict(name='write_slong',source=F,symbol='?WriteSLONG@CSystemRegistry@@QBEXABVCWideString@@J@Z',address=0x99d0c0,size=53,instructions=20,relocations=2,flags=GS),
 dict(name='write_string',source=F,symbol='?WriteString@CSystemRegistry@@QBEXABVCWideString@@0@Z',address=0x99d140,size=61,instructions=25,relocations=4,flags=GS),
 dict(name='write_binary',source=F,symbol='?WriteBinary@CSystemRegistry@@QBEXABVCWideString@@PAXK@Z',address=0x99d180,size=48,instructions=19,relocations=2,flags=GS),
 dict(name='get_size',source=F,symbol='?GetSize@CSystemRegistry@@QBEKABVCWideString@@@Z',address=0x99d1b0,size=42,instructions=16,relocations=2,flags=GS),
 dict(name='delete_value',source=F,symbol='?DeleteValue@CSystemRegistry@@QBEXABVCWideString@@@Z',address=0x99d1e0,size=27,instructions=10,relocations=2,flags=GS),
 dict(name='get_binary',source=F,symbol='?GetBinary@CSystemRegistry@@QBEXABVCWideString@@PAXK@Z',address=0x99d200,size=184,instructions=77,relocations=5,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-system-registry-values.json',[],
  'CSystemRegistry value accessors (Ego_r members; GetString wchar_t[512] under /GS; advapi32 called as dllimports; GetBinary uses new[]/delete[]). GetBOOL 0099CFE0 supersedes the fake CEngineMap::PeekWaterHasType landing')
 print('NATIVE_SYSTEM_REGISTRY_VALUES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
