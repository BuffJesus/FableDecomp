"""Fresh VC71 parity gate: lib_definition_types text TransferOut for C2DVector/C3DVector (C2DCoordF/C3DCoordF), C3DAngles, CRGBColour."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??YCCharString@@QAEABV0@ABV0@@Z':0x99f0a0,
 '??YCCharString@@QAEABV0@PBD@Z':0x99f100,
 '?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,
 '?GFFloatToCharString@@YI?AVCCharString@@NJ@Z':0x9a08b0,  # derived from retail site; not in symbol_index
 '??_C@_01PKGAHCOL@?$CJ?$AA@':0x122e020,
 '??_C@_01IHBHIGKO@?0?$AA@':0x122e024,
 '??_C@_0L@JPIPJFN@C2DCoordF?$CI?$AA@':0x129c238,  # derived from retail site; not in symbol_index
 '??_C@_0L@OGDKJCGD@C3DCoordF?$CI?$AA@':0x129c244,  # derived from retail site; not in symbol_index
 '??_C@_0L@KLPHHFDK@C3DAngles?$CI?$AA@':0x129c250,  # derived from retail site; not in symbol_index
 '??_C@_0M@IHHGJLKI@CRGBColour?$CI?$AA@':0x129c25c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CPersistTraits_TransferOut_009e7f00.cpp'
ROOTS=[
 dict(name='vec2',source=F,symbol='?TransferOut@?$CPersistTraits@VC2DVector@@@@QAEXAAVCPersistContext@@AAVCCharString@@AAVC2DVector@@@Z',address=0x9e7f00,size=130,instructions=38,relocations=12),
 dict(name='vec3',source=F,symbol='?TransferOut@?$CPersistTraits@VC3DVector@@@@QAEXAAVCPersistContext@@AAVCCharString@@AAVC3DVector@@@Z',address=0x9e7f90,size=182,instructions=52,relocations=17),
 dict(name='angles',source=F,symbol='?TransferOut@?$CPersistTraits@VC3DAngles@@@@QAEXAAVCPersistContext@@AAVCCharString@@AAVC3DAngles@@@Z',address=0x9e8050,size=182,instructions=52,relocations=17),
 dict(name='colour',source=F,symbol='?TransferOut@?$CPersistTraits@VCRGBColour@@@@QAEXAAVCPersistContext@@AAVCCharString@@AAVCRGBColour@@@Z',address=0x9e8110,size=194,instructions=54,relocations=22),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-definition-traits-out.json',[],
  'lib_definition_types text TransferOut for C2DVector/C3DVector (C2DCoordF/C3DCoordF), C3DAngles, CRGBColour')
 print('NATIVE_DEFINITION_TRAITS_OUT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
