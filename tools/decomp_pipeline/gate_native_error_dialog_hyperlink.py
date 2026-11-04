"""Fresh VC71 parity gate: errordialog.cpp hyperlink statics (FableWin _HyperlinkParentProc / _HyperlinkProc / ConvertStaticToHyperlink; the Microsoft static-to-hyperlink sample in ANSI with the Ego_r MouseOver global and WM_SETFONT)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_HyperlinkParentProc@@YGJPAUHWND__@@IIJ@Z':0x99d5b0,
 '?_HyperlinkProc@@YGJPAUHWND__@@IIJ@Z':0x99d660,
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '??_C@_06KJMEDNIB@Static?$AA@':0x129a96c,  # derived from retail site; not in symbol_index
 '??_C@_08OJMGFDAC@Old_Proc?$AA@':0x129a974,  # derived from retail site; not in symbol_index
 '??_C@_04IEDBBIIG@Font?$AA@':0x129a980,  # derived from retail site; not in symbol_index
 '??_C@_08KAFEFAMD@Old_Font?$AA@':0x129a988,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
 '?MouseOver@@3HA':0x13bd6f8,  # derived from retail site; not in symbol_index
 '__imp__GetObjectA@12':0x143fc6c,  # derived from retail site; not in symbol_index
 '__imp__CreateFontIndirectA@4':0x143fc70,  # derived from retail site; not in symbol_index
 '__imp__DeleteObject@4':0x143fc74,  # derived from retail site; not in symbol_index
 '__imp__SetTextColor@8':0x143fc7c,  # derived from retail site; not in symbol_index
 '__imp__RemovePropA@8':0x14402e0,  # derived from retail site; not in symbol_index
 '__imp__SetWindowLongA@12':0x14402e4,  # derived from retail site; not in symbol_index
 '__imp__CallWindowProcA@20':0x14402e8,  # derived from retail site; not in symbol_index
 '__imp__GetPropA@8':0x14402ec,  # derived from retail site; not in symbol_index
 '__imp__SendMessageA@16':0x14402f0,  # derived from retail site; not in symbol_index
 '__imp__SetCursor@4':0x14402f4,  # derived from retail site; not in symbol_index
 '__imp__LoadCursorA@8':0x14402f8,  # derived from retail site; not in symbol_index
 '__imp__ReleaseCapture@0':0x14402fc,  # derived from retail site; not in symbol_index
 '__imp__PtInRect@12':0x1440300,  # derived from retail site; not in symbol_index
 '__imp__ClientToScreen@8':0x1440304,  # derived from retail site; not in symbol_index
 '__imp__GetWindowRect@8':0x1440308,  # derived from retail site; not in symbol_index
 '__imp__SetCapture@4':0x144030c,  # derived from retail site; not in symbol_index
 '__imp__InvalidateRect@12':0x1440310,  # derived from retail site; not in symbol_index
 '__imp__GetCapture@0':0x1440314,  # derived from retail site; not in symbol_index
 '__imp__GetWindowLongA@8':0x1440318,  # derived from retail site; not in symbol_index
 '__imp__SetPropA@12':0x144031c,  # derived from retail site; not in symbol_index
 '__imp__GetParent@4':0x1440320,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/ErrorDialog_Hyperlink_0099d5b0.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='parent_proc',source=F,symbol='?_HyperlinkParentProc@@YGJPAUHWND__@@IIJ@Z',address=0x99d5b0,size=176,instructions=70,relocations=11,flags=GS),
 dict(name='proc',source=F,symbol='?_HyperlinkProc@@YGJPAUHWND__@@IIJ@Z',address=0x99d660,size=363,instructions=120,relocations=25,flags=GS),
 dict(name='convert',source=F,symbol='?ConvertStaticToHyperlink@@YIHPAUHWND__@@@Z',address=0x99d7d0,size=231,instructions=91,relocations=18,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-error-dialog-hyperlink.json',[],
  'errordialog.cpp hyperlink statics (FableWin _HyperlinkParentProc / _HyperlinkProc / ConvertStaticToHyperlink; the Microsoft static-to-hyperlink sample in ANSI with the Ego_r MouseOver global and WM_SETFONT)')
 print('NATIVE_ERROR_DIALOG_HYPERLINK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
