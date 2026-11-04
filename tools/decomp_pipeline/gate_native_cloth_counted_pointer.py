"""Fresh VC71 parity gate: CCountedPointer<C3DClothPrimitive> DeleteData 0x00A93A60, ctor 0x00A94D60, destructor 0x00A94DB0, Reset 0x00A94DF0, exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??2@YAPAXI@Z':0xbfea1a,
 '??3@YAXPAX@Z':0xbfe9bc,
 '?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@CIXPAX@Z':0xa93a60,
}
ROOTS=[
 dict(name='deletedata_c3dclothprimitive',source=R/'rebuild/src/compiled/00/a9/CCountedPointer_ClothPrimitive_00a93a60.cpp',symbol='?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@CIXPAX@Z',address=0xa93a60,size=11,instructions=6,relocations=0),
 dict(name='dtor_c3dclothprimitive',source=R/'rebuild/src/compiled/00/a9/CCountedPointer_ClothPrimitive_00a93a60.cpp',symbol='??1?$CCountedPointer@VC3DClothPrimitive@@@@QAE@XZ',address=0xa94db0,size=53,instructions=19,relocations=1),
 dict(name='ctor_c3dclothprimitive',source=R/'rebuild/src/compiled/00/a9/CCountedPointer_ClothPrimitive_00a93a60.cpp',symbol='??0?$CCountedPointer@VC3DClothPrimitive@@@@QAE@PAVC3DClothPrimitive@@@Z',address=0xa94d60,size=78,instructions=28,relocations=2),
 dict(name='reset_c3dclothprimitive',source=R/'rebuild/src/compiled/00/a9/CCountedPointer_ClothPrimitive_00a93a60.cpp',symbol='?Reset@?$CCountedPointer@VC3DClothPrimitive@@@@QAEXPAVC3DClothPrimitive@@@Z',address=0xa94df0,size=103,instructions=36,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-cloth-counted-pointer.json',[],
  'CCountedPointer<C3DClothPrimitive> DeleteData 0x00A93A60, ctor 0x00A94D60, destructor 0x00A94DB0, Reset 0x00A94DF0, exact after relocation')
 print('NATIVE_CLOTH_COUNTED_POINTER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
