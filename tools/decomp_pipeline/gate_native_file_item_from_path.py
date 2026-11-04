"""Fresh VC71 parity gate: NFile::GetItemFromPath CWideString form (t_out[260] through the character form)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetItemFromPath@NFile@@YI_NPBGKPAG@Z':0x997880,  # derived from retail site; not in symbol_index
 '?GetAsCharArray@CWideString@@QBEPBGXZ':0x99af10,
 '??4CWideString@@QAEABV0@PBG@Z':0x99b800,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/NFile_GetItemFromPath_00997b40.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='item_from_path',source=F,symbol='?GetItemFromPath@NFile@@YI_NABVCWideString@@KAAV2@@Z',address=0x997b40,size=107,instructions=28,relocations=6,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-item-from-path.json',[],
  'NFile::GetItemFromPath CWideString form (t_out[260] through the character form)')
 print('NATIVE_FILE_ITEM_FROM_PATH_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
